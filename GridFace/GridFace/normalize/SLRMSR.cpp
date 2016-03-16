#include "SLRMSR.h"

using namespace std;
using namespace cv;



void ApplyIllumination(Mat source, Mat illumination, Mat &destination, double upperBound){
	vector<Mat> destinationChannels;
	split(source, destinationChannels);
	vector<Mat> illuminationChannels;
	split(illumination, illuminationChannels);
	for (int i = 0; i<destinationChannels.size(); ++i){
		destinationChannels[i].convertTo(destinationChannels[i], CV_64FC1);
		divide(destinationChannels[i], illuminationChannels[i], destinationChannels[i]);
	}

	merge(destinationChannels, destination);

	double *check = (double *)destination.data;
	for (int i = 0; i<destination.rows*destination.cols * 3; ++i){
		if (check[i] >= upperBound){
			check[i] = upperBound - 1;
		}
	}

	destination.convertTo(destination, source.type());

}

void CombineImages(Mat source1, Mat source2, Mat &destination, Mat map){

	Mat mapD;
	map.convertTo(mapD, CV_64F);

	Mat img1;
	Mat img2;

	source1.convertTo(img1, CV_64F);
	source2.convertTo(img2, CV_64F);

	int rows = source1.rows;
	int cols = source1.cols;

	Mat result = Mat(rows, cols, CV_64FC3);

	Mat g1;
	source1.convertTo(g1, CV_8U);
	cvtColor(g1, g1, CV_BGR2GRAY);
	Mat g2;
	source2.convertTo(g2, CV_8U);
	cvtColor(g2, g2, CV_BGR2GRAY);

	g1.convertTo(g1, CV_64F);
	g2.convertTo(g2, CV_64F);

	for (int i = 0; i<rows; ++i){
		for (int j = 0; j<cols; ++j){
			double &a = *(((double *)(g2.data)) + i*cols + j);
			double &b = *(((double *)(g1.data)) + i*cols + j);

			double f = *(((double *)(map.data)) + i*cols + j);
			Vec3d &pr = *(((Vec3d *)(result.data)) + i*cols + j);
			Vec3d &p1 = *(((Vec3d *)(img1.data)) + i*cols + j);
			Vec3d &p2 = *(((Vec3d *)(img2.data)) + i*cols + j);
			pr = p1*f + p2*(1 - f);

			double pg = b*f + a*(1 - f);

			double t = 1700;
			if (pg>t*b){
				pr *= b*t / pg;
			}
		}
	}

	result.convertTo(destination, source1.type());
}

void GuidedImageFilterC1(Mat img, Mat guidance, Mat &result, int r, double epsilon){

	Mat p;
	img.convertTo(p, CV_64F);

	Mat I;
	guidance.convertTo(I, CV_64F);

	Mat meanI;
	boxFilter(I, meanI, -1, Size(r, r));

	Mat meanP;
	boxFilter(p, meanP, -1, Size(r, r));

	Mat corrI;
	boxFilter(I.mul(I), corrI, -1, Size(r, r));

	Mat corrIp;
	boxFilter(I.mul(p), corrIp, -1, Size(r, r));

	Mat varI = corrI - meanI.mul(meanI);
	Mat covIp = corrIp - meanI.mul(meanP);

	Mat a;
	divide(covIp, varI + epsilon, a);
	Mat b = meanP - a.mul(meanI);

	Mat meanA;
	boxFilter(a, meanA, -1, Size(r, r));

	Mat meanB;
	boxFilter(b, meanB, -1, Size(r, r));

	Mat q = meanA.mul(I) + meanB;

	q.convertTo(result, img.type());
}

void GuidedImageFilterC3(Mat img, Mat guidance, Mat &result, int r, double epsilon){

	vector<Mat> imgChannels;
	vector<Mat> guidanceChannels;

	split(img, imgChannels);
	split(guidance, guidanceChannels);

	vector<Mat> resultChannels;

	for (int i = 0; i<imgChannels.size(); ++i){
		Mat channelResult;
		GuidedImageFilterC1(imgChannels[i], guidanceChannels[i], channelResult, r, epsilon);
		resultChannels.push_back(channelResult);
	}

	merge(resultChannels, result);

}

void Normalize(Mat &image){
	vector<Mat> channels;

	split(image, channels);
	Mat illuminantAverage = (channels[0] + channels[1] + channels[2]) / 3;
	for (int i = 0; i<3; ++i){
		divide(channels[i], illuminantAverage, channels[i]);
	}
	merge(channels, image);

}

void RandomMemorySpraysRetinexPerformWhiteBalance(Mat source, Mat &destination, int N, int n, double upperBound, int rowsStep, int colsStep, double rFactor){

	int rows = source.rows;
	int cols = source.cols;

	int R = rFactor*sqrt((double)(rows*rows + cols*cols)) + 0.5;

	Mat normalized;
	source.convertTo(normalized, CV_64FC3);

	int outputRows = rows / rowsStep;
	int outputCols = cols / colsStep;
	destination = Mat(outputRows, outputCols, CV_64FC3);

	Vec3d *input = (Vec3d *)normalized.data;
	Vec3d *inputPoint = input;
	Vec3d *output = (Vec3d *)destination.data;
	Vec3d *outputPoint = output;

	RNG random;

	CartesianTree<double> **qhs;

	qhs = new CartesianTree<double>*[N];
	for (int i = 0; i<N; ++i){
		qhs[i] = new CartesianTree<double>[3];
	}

	for (int outputRow = 0; outputRow<outputRows; ++outputRow){
		for (int outputCol = 0; outputCol<outputCols; ++outputCol){

			int row = outputRow*rowsStep;
			int col = outputCol*colsStep;

			inputPoint = input + row*cols + col;
			outputPoint = output + outputRow*outputCols + outputCol;

			Vec3d &currentPoint = *inputPoint;
			Vec3d &finalPoint = *outputPoint;
			finalPoint = Vec3d(0, 0, 0);

			for (int i = 0; i<N; ++i){

				Vec3d max = Vec3d(0, 0, 0);

				while (qhs[i][0].Size()<n){
					double angle = 2 * CV_PI*random.uniform(0.0, 1.0);
					double r = R*random.uniform(0.0, 1.0);

					int newRow = row + r*sin(angle);
					int newCol = col + r*cos(angle);

					if (newRow >= 0 && newRow<rows && newCol >= 0 && newCol<cols){
						Vec3d &newPoint = input[newRow*cols + newCol];
						for (int k = 0; k<3; ++k){
							qhs[i][k].Push(newPoint[k]);
						}
					}
				}

				for (int k = 0; k<3; ++k){
					if (max[k]<qhs[i][k].Max()){
						max[k] = qhs[i][k].Max();
					}

					qhs[i][k].Pop();

				}

				for (int k = 0; k<3; ++k){
					if (max[k] == 0){
						max[k] = 1;
					}
					finalPoint[k] += currentPoint[k] / max[k];
				}

			}

			finalPoint /= N;

			for (int i = 0; i<3; ++i){
				if (finalPoint[i]>1){
					finalPoint[i] = 1;
				}
			}

		}
	}

	for (int i = 0; i<N; ++i){
		delete[] qhs[i];
	}
	delete[] qhs;

	double scaleFactor = upperBound;

	if (rowsStep>1 || colsStep>1){
		resize(destination, destination, source.size());
	}

	destination = destination*scaleFactor - 1;

	destination.convertTo(destination, source.type());

}

void GuidedLightRandomMemorySpraysRetinexEstimateLocalIlumination(Mat source, Mat &destination, int N, int n, int inputKernelSize, int illuminantKernelSize, bool normalizeIlluminant = false, int rowsStep = 1, int colsStep = 1, double upperBound = 256.0, double rFactor = 1.0){

	Mat retinex;
	RandomMemorySpraysRetinexPerformWhiteBalance(source, retinex, N, n, upperBound, rowsStep, colsStep, rFactor);

	Mat inputSource;
	Mat inputRetinex;

	source.convertTo(inputSource, CV_64FC3);
	retinex.convertTo(inputRetinex, CV_64FC3);

	Mat guidance;
	inputSource.copyTo(guidance);

	if (normalizeIlluminant){
		Mat illuminant;
		divide(inputSource, inputRetinex, illuminant);
		vector<Mat> illuminantChannels;

		split(illuminant, illuminantChannels);
		Mat illuminantAverage = (illuminantChannels[0] + illuminantChannels[1] + illuminantChannels[2]) / 3;
		for (int i = 0; i<3; ++i){
			divide(illuminantChannels[i], illuminantAverage, illuminantChannels[i]);
		}
		merge(illuminantChannels, illuminant);

		inputSource = inputRetinex.mul(illuminant);
	}

	double value = 40;
	double epsilon = value*value;

	if (inputKernelSize>1){
		GuidedImageFilterC3(inputSource, guidance, inputSource, inputKernelSize, epsilon);
		GuidedImageFilterC3(inputRetinex, guidance, inputRetinex, inputKernelSize, epsilon);
	}

	Mat illuminant;
	divide(inputSource, inputRetinex, illuminant);
	vector<Mat> illuminantChannels;

	if (illuminantKernelSize>1){
		GuidedImageFilterC3(illuminant, guidance, illuminant, illuminantKernelSize, epsilon);
	}

	illuminant.copyTo(destination);

}

void LightRandomMemorySpraysRetinexEstimateLocalIlumination(Mat source, Mat &destination, int N, int n, int inputKernelSize, int illuminantKernelSize, bool normalizeIlluminant = false, int rowsStep = 1, int colsStep = 1, double upperBound = 256.0, double rFactor = 1.0){

	Mat retinex;
	RandomMemorySpraysRetinexPerformWhiteBalance(source, retinex, N, n, upperBound, rowsStep, colsStep, rFactor);

	Mat inputSource;
	Mat inputRetinex;

	source.convertTo(inputSource, CV_64FC3);
	retinex.convertTo(inputRetinex, CV_64FC3);

	Mat guidance;
	inputSource.copyTo(guidance);

	if (normalizeIlluminant){
		Mat illuminant;
		divide(inputSource, inputRetinex, illuminant);
		vector<Mat> illuminantChannels;

		split(illuminant, illuminantChannels);
		Mat illuminantAverage = (illuminantChannels[0] + illuminantChannels[1] + illuminantChannels[2]) / 3;
		for (int i = 0; i<3; ++i){
			divide(illuminantChannels[i], illuminantAverage, illuminantChannels[i]);
		}
		merge(illuminantChannels, illuminant);

		inputSource = inputRetinex.mul(illuminant);
	}

	if (inputKernelSize>1){
		Mat averaging = Mat::ones(inputKernelSize, inputKernelSize, CV_64FC1) / (double)(inputKernelSize*inputKernelSize);
		boxFilter(inputSource, inputSource, -1, Size(inputKernelSize, inputKernelSize));
		boxFilter(inputRetinex, inputRetinex, -1, Size(inputKernelSize, inputKernelSize));
	}

	Mat illuminant;
	divide(inputSource, inputRetinex, illuminant);
	vector<Mat> illuminantChannels;

	if (illuminantKernelSize>1){
		Mat averaging = Mat::ones(illuminantKernelSize, illuminantKernelSize, CV_64FC1) / (double)(illuminantKernelSize*illuminantKernelSize);
		boxFilter(illuminant, illuminant, -1, Size(illuminantKernelSize, illuminantKernelSize));
	}

	illuminant.copyTo(destination);

}

void Adjust(Mat source, Mat &destination, bool adjustBrightness = true, bool adjustColors = true, int n = 3, bool filterOnlyIllumination = false, bool guidedFilter = false, double adjustmentExponent = 1.0 / 2.0, int kernelSize = 25, int finalFilterSize = 5, double upperBound = 255.0){

	if (adjustBrightness == false && adjustColors == false){
		source.copyTo(destination);
		return;
	}

	Mat gray;
	cvtColor(source, gray, COLOR_BGR2GRAY);

	int filter = kernelSize;

	Mat illumination;
	if (guidedFilter){
		GuidedLightRandomMemorySpraysRetinexEstimateLocalIlumination(source, illumination, 1, n, kernelSize, kernelSize, false, 1, 1, upperBound);
	}
	else{
		LightRandomMemorySpraysRetinexEstimateLocalIlumination(source, illumination, 1, n, kernelSize, kernelSize, false, 1, 1, upperBound);
	}

	Mat result;
	if (adjustBrightness == true){
		Mat lrsr;
		int prefilter = filter;
		if (filterOnlyIllumination){
			prefilter = 1;
		}

		ApplyIllumination(source, illumination, lrsr, upperBound);

		Mat lrsrGray;
		cvtColor(lrsr, lrsrGray, COLOR_BGR2GRAY);

		gray.convertTo(gray, CV_64F);
		lrsrGray.convertTo(lrsrGray, CV_64F);

		Mat f;
		divide(lrsrGray, gray, f);

		vector<Mat> fChannels;
		fChannels.push_back(f);
		fChannels.push_back(f);
		fChannels.push_back(f);

		Mat imgD;
		source.convertTo(imgD, CV_64F);
		Mat f3;
		merge(fChannels, f3);
		Mat result1 = imgD.mul(f3);

		Mat grayMap = gray / upperBound;
		pow(grayMap, adjustmentExponent, grayMap);

		CombineImages(source, result1, result, grayMap);

		Mat gray1;
		Mat gray2;

		cvtColor(source, gray1, COLOR_BGR2GRAY);
		cvtColor(result, gray2, COLOR_BGR2GRAY);

		Mat f2;
		gray1.convertTo(gray1, CV_64F);
		gray2.convertTo(gray2, CV_64F);
		divide(gray2, gray1, f2);

		if (guidedFilter){
			double value = 30;
			double epsilon = value*value;
			GuidedImageFilterC1(f2, gray, f2, finalFilterSize, epsilon);
		}
		else{
			boxFilter(f2, f2, -1, Size(finalFilterSize, finalFilterSize));
		}
		vector<Mat> channels;
		channels.push_back(f2);
		channels.push_back(f2);
		channels.push_back(f2);
		Mat m2;
		merge(channels, m2);

		Mat result2;
		Mat mi;
		source.convertTo(mi, CV_64F);
		result2 = mi.mul(m2);
		result2.convertTo(result, source.type());

	}
	else{
		source.copyTo(result);
	}

	if (adjustColors == true){
		Normalize(illumination);
		result.convertTo(result, CV_64F);
		divide(result, illumination, result);
		double *check = (double *)destination.data;
		for (int i = 0; i<destination.rows*destination.cols * 3; ++i){
			if (check[i] >= upperBound){
				check[i] = upperBound;
			}
		}

	}

	result.convertTo(result, source.type());
	result.copyTo(destination);

}

cv::Mat SLRMSR_process(cv::Mat source)
{

	int n = 4;
	double a = 0.55;
	int brightnessAdjustment = true;
	int colorCorrection = true;
	int gif = true;


	if (source.rows*source.cols == 0){
		exit(0);
	}

	cv::Mat result;
	Adjust(source, result, brightnessAdjustment, colorCorrection, n, false, gif, a, (int)25, (int)5, (double)255.0);
	result.convertTo(result, CV_8U);

	return result; 
}


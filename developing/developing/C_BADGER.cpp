#include "C_BADGER.h"

using namespace std;
using namespace cv;


void Filter64F(cv::Mat img, cv::Mat &result, int k){

	int rows = img.rows;
	int cols = img.cols;

	cv::Mat currentResult = cv::Mat::zeros(rows, cols, CV_64FC1);

	double *data = (double *)img.data;

	double *s = new double[(rows + 1)*(cols + 1)];

	s[1 * (cols + 1) + 1 + 0] = (*data);

	for (int i = 0; i<rows + 1; ++i){
		s[i*(cols + 1)] = 0;
	}

	for (int i = 0; i<cols + 1; ++i){
		s[0 * (cols + 1) + i] = 0;
	}

	for (int i = 0; i<rows; ++i){
		for (int j = 0; j<cols; ++j){
			double pixel = *(data + i*cols + j);
			s[(i + 1)*(cols + 1) + (j + 1)] = pixel - s[i*(cols + 1) + j] + s[i*(cols + 1) + (j + 1)] + s[(i + 1)*(cols + 1) + j];
		}
	}

	double *output = (double *)currentResult.data;
	for (int i = 0; i<rows; ++i){
		int row = i + 1;

		int startRow = row - (k - 1) / 2 - 1;
		if (startRow<0){
			startRow = 0;
		}

		int endRow = row + k / 2;
		if (endRow>rows){
			endRow = rows;
		}

		for (int j = 0; j<cols; ++j){
			int col = j + 1;
			int startCol = col - (k - 1) / 2 - 1;
			if (startCol<0){
				startCol = 0;
			}

			int endCol = col + k / 2;
			if (endCol>cols){
				endCol = cols;
			}
			double &r = *(output + i*cols + j);
			r = (s[endRow*(cols + 1) + endCol] - s[endRow*(cols + 1) + startCol] - s[startRow*(cols + 1) + endCol] + s[startRow*(cols + 1) + startCol]) / ((endRow - startRow)*(endCol - startCol));
		}

	}

	currentResult.convertTo(result, currentResult.type());

	delete[] s;

}

cv::Point2i **CreateSprays(int spraysCount, int n, int R){

	cv::RNG random;

	cv::Point2i **sprays = new cv::Point2i*[spraysCount];
	for (int i = 0; i<spraysCount; ++i){
		sprays[i] = new cv::Point2i[n];
		for (int j = 0; j<n; ++j){

			double angle = 2 * CV_PI*random.uniform(0.0, 1.0);
			double r = R*random.uniform(0.0, 1.0);

			sprays[i][j].x = r*cos(angle);
			sprays[i][j].y = r*sin(angle);
		}
	}

	return sprays;
}

void DeleteSprays(cv::Point2i **sprays, int spraysCount){

	for (int i = 0; i<spraysCount; ++i){
		delete[] sprays[i];
	}

	delete[] sprays;

}

void PerformRSR(cv::Mat source, cv::Mat &destination, int N, int n, int rowsStep, int colsStep, double upperBound, double rFactor){
	int rows = source.rows;
	int cols = source.cols;

	int R = rFactor*sqrt((double)(rows*rows + cols*cols)) + 0.5;

	int spraysCount = 1000 * N;
	cv::Point2i **sprays = CreateSprays(spraysCount, n, R);

	cv::Mat normalized;
	source.convertTo(normalized, CV_64FC1);

	int outputRows = rows / rowsStep;
	int outputCols = cols / colsStep;
	destination = cv::Mat(outputRows, outputCols, CV_64FC1);

	double *input = (double *)normalized.data;
	double *inputPoint = input;
	double *output = (double *)destination.data;
	double *outputPoint = output;

	cv::RNG random;

	for (int outputRow = 0; outputRow<outputRows; ++outputRow){
		for (int outputCol = 0; outputCol<outputCols; ++outputCol){

			int row = outputRow*rowsStep;
			int col = outputCol*colsStep;

			inputPoint = input + row*cols + col;
			outputPoint = output + outputRow*outputCols + outputCol;

			double &currentPoint = *inputPoint;
			double &finalPoint = *outputPoint;
			finalPoint = 0.0;

			for (int i = 0; i<N; ++i){

				double max = 0.0;
				double min = upperBound;

				int selectedSpray = random.uniform(0, spraysCount);

				for (int j = 0; j<n; ++j){

					int newRow = row + sprays[selectedSpray][j].y;
					int newCol = col + sprays[selectedSpray][j].x;

					if (newRow >= 0 && newRow<rows && newCol >= 0 && newCol<cols){

						double &newPoint = input[newRow*cols + newCol];

						if (max<newPoint){
							max = newPoint;
						}
						if (newPoint<min){
							min = newPoint;
						}

					}

				}

				if (max<min){
					--i;
					continue;
				}
				else if (max - min == 0){
					if (currentPoint<max){
						finalPoint += max;
					}
					else{
						finalPoint += currentPoint;
					}
				}
				else{
					if (currentPoint<min){
						--i;
						continue;
					}
					finalPoint += currentPoint / max;

				}

			}

			finalPoint /= N;

			if (finalPoint>1){
				finalPoint = 1;
			}

			if (finalPoint<0){
				finalPoint = 0;
			}

		}
	}

	double scaleFactor = upperBound;

	if (rowsStep>1 || colsStep>1){
		resize(destination, destination, source.size());
	}

	destination = destination*scaleFactor;

	destination.convertTo(destination, source.type());

	DeleteSprays(sprays, spraysCount);
}

void ProcessChannel(cv::Mat source, cv::Mat &destination, int N, int n, int inputKernelSize, int inputFilteringIterations, int illuminantKernelSize, int illuminantFilteringIterations, int rowsStep, int colsStep, double upperBound, double rFactor){

	cv::Mat inputSource;
	cv::Mat inputRetinex;
	cv::Mat retinex;

	PerformRSR(source, retinex, N, n, rowsStep, colsStep, upperBound, rFactor);

	source.convertTo(inputSource, CV_64FC1);
	retinex.convertTo(retinex, source.type());
	retinex.convertTo(inputRetinex, CV_64FC1);

	cv::Mat illuminant;

	if (inputKernelSize>1){
		for (int i = 0; i<inputFilteringIterations; ++i){
			Filter64F(inputSource, inputSource, inputKernelSize);
			Filter64F(inputRetinex, inputRetinex, inputKernelSize);
		}
	}

	cv::divide(inputSource, inputRetinex, illuminant);

	if (illuminantKernelSize>1){
		for (int i = 0; i<illuminantFilteringIterations; ++i){
			Filter64F(illuminant, illuminant, illuminantKernelSize);
		}
	}

	cv::divide(source, illuminant, destination);

	double *check = (double *)destination.data;
	for (int i = 0; i<destination.rows*destination.cols; ++i){
		if (check[i] >= upperBound){
			check[i] = upperBound - 1;
		}
	}

	destination.convertTo(destination, source.type());

}

double min2(cv::Mat source){
	double max;
	cv::Mat img;
	source.convertTo(img, CV_64F);
	cv::minMaxLoc(img, NULL, &max);
	double min = max;

	double *data = (double *)img.data;
	for (int i = 0; i<img.rows*img.cols*img.channels(); ++i){
		double value = *(data + i);
		if (value>0.0 && value<min){
			min = value;
		}
	}

	return min;
}

void ProcessLightnessChannel(cv::Mat initialSource, cv::Mat &destination, double downP = 1.0, double upP = 1.0, int N = 1, int n = 16, int inputKernelSize = 25, int inputFilteringIterations = 1, int illuminantKernelSize = 25, int illuminantFilteringIterations = 1, int rowsStep = 1, int colsStep = 1, double upperBound = 255.0, double rFactor = 1.0){
	cv::Mat source;
	initialSource.convertTo(source, CV_32F);

	double sourceMin, sourceMax;
	minMaxLoc(source, &sourceMin, &sourceMax);
	sourceMin = min2(source);
	source /= sourceMin;
	sourceMax /= sourceMin;
	double calculatedUpperBound = sourceMax;

	int rows = source.rows;
	int cols = source.cols;

	cv::Mat initialLightness;
	cv::cvtColor(source, initialLightness, CV_BGR2GRAY);

	initialLightness.convertTo(initialLightness, CV_64F);

	cv::Mat lightness;
	initialLightness.copyTo(lightness);
	lightness.convertTo(lightness, CV_64F);

	double min, max;
	cv::pow(lightness, 1.0 / downP, lightness);
	minMaxLoc(lightness, &min, &max);
	lightness *= calculatedUpperBound / max;

	ProcessChannel(lightness, lightness, N, n, inputKernelSize, inputFilteringIterations, illuminantKernelSize, illuminantFilteringIterations, rowsStep, colsStep, calculatedUpperBound, rFactor);

	cv::pow(lightness, upP, lightness);
	cv::minMaxLoc(lightness, &min, &max);
	lightness *= calculatedUpperBound / max;

	lightness.convertTo(lightness, CV_64F);

	cv::Mat sourceD;
	source.convertTo(sourceD, CV_64F);

	cv::Vec3d *input = (cv::Vec3d *)sourceD.data;
	double *initial = (double *)initialLightness.data;
	double *modified = (double *)lightness.data;
	for (int i = 0; i<rows; ++i){
		for (int j = 0; j<cols; ++j){
			cv::Vec3d &point = *(input + i*cols + j);
			double a = *(modified + i*cols + j);
			double b = *(initial + i*cols + j);

			double f = 1.0;
			if (b<1.0){
				b = 1.0;
			}
			if (a<1.0){
				a = 1.0;
			}
			f = a / b;

			double m = calculatedUpperBound;
			for (int k = 1; k<3; ++k){
				if (point[k] >= 1.0){
					double c = calculatedUpperBound / point[k];
					if (c<m){
						m = c;
					}
				}
			}

			if (m<f){
				f = m;
			}

			point *= f;

		}
	}

	sourceD *= upperBound / calculatedUpperBound;
	sourceD.convertTo(destination, initialSource.type());
}

void ProcessRGBChannels(cv::Mat initialSource, cv::Mat &destination, double downP = 1.0, double upP = 1.0, int N = 1, int n = 16, int inputKernelSize = 55, int inputFilteringIterations = 1, int illuminantKernelSize = 55, int illuminantFilteringIterations = 1, int rowsStep = 1, int colsStep = 1, double upperBound = 255.0, double rFactor = 1.0){
	cv::Mat source;
	initialSource.convertTo(source, CV_64F);

	double sourceMin, sourceMax;
	cv::minMaxLoc(source, &sourceMin, &sourceMax);
	sourceMin = min2(source);
	source /= sourceMin;
	sourceMax /= sourceMin;
	double calculatedUpperBound = sourceMax;

	std::vector<cv::Mat> channels;
	cv::split(source, channels);

	for (int i = 0; i<(int)channels.size(); ++i){
		double min, max;

		cv::pow(channels[i], 1.0 / downP, channels[i]);
		cv::minMaxLoc(channels[i], &min, &max);
		channels[i] *= calculatedUpperBound / max;

		ProcessChannel(channels[i], channels[i], N, n, inputKernelSize, inputFilteringIterations, illuminantKernelSize, illuminantFilteringIterations, rowsStep, colsStep, calculatedUpperBound, rFactor);
		cv::pow(channels[i], upP, channels[i]);
		cv::minMaxLoc(channels[i], &min, &max);
		channels[i] *= calculatedUpperBound / max;

		channels[i].convertTo(channels[i], CV_64F);

		channels[i] *= upperBound / calculatedUpperBound;
	}

	cv::merge(channels, destination);
	destination.convertTo(destination, initialSource.type());

}

cv::Mat badger_process(cv::Mat source)
{
	double downP = 1.0;
	double upP = 1.0;
	int N = 1;
	int n = 16;
	int inputKernelSize = 25;
	int inputFilteringIterations = 1;
	int illuminantKernelSize = 25;
	int illuminantFilteringIterations = 1;
	int rowsStep = 1;
	int colsStep = 1;
	double rFactor = 1.0;
	double upperBound = 255.0;
	int mode = 1;

	if (source.rows*source.cols == 0){
		exit(0);
	}

	cv::Mat destination;
	if (mode == 0){
		ProcessLightnessChannel(source, destination, downP, upP, N, n, inputKernelSize, inputFilteringIterations, illuminantKernelSize, illuminantFilteringIterations, rowsStep, colsStep, upperBound, rFactor);
	}
	else{
		ProcessRGBChannels(source, destination, downP, upP, N, n, inputKernelSize, inputFilteringIterations, illuminantKernelSize, illuminantFilteringIterations, rowsStep, colsStep, upperBound, rFactor);
	}
	destination.convertTo(destination, CV_8U);

	return destination; 
}
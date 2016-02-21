#include "facegrid.h"


FaceGrid::FaceGrid(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

FaceGrid::~FaceGrid()
{

}

void detectFaceInImage(IplImage *orig, IplImage* input, cv::CascadeClassifier* cascade, FLANDMARK_Model *model, int *bbox, double *landmarks)
{
	// Smallest face size.
	CvSize minFeatureSize = cvSize(40, 40);
	int flags = CV_HAAR_DO_CANNY_PRUNING;
	// How detailed should the search be.
	float search_scale_factor = 1.1f;
	CvMemStorage* storage;
	CvSeq* rects;
	int nFaces;
	cv::Mat test;

	storage = cvCreateMemStorage(0);
	cvClearMemStorage(storage);

	std::vector<cv::Rect> faces;

	//-- Detect faces
	cascade->detectMultiScale(test, faces, search_scale_factor, 2, 0 | flags, minFeatureSize);

	// Detect all the faces in the greyscale image.
	//rects = cvHaarDetectObjects(input, cascade, storage, search_scale_factor, 2, flags, minFeatureSize);
	nFaces = rects->total;

	double t = (double)cvGetTickCount();
	for (int iface = 0; iface < (rects ? nFaces : 0); ++iface)
	{
		CvRect *r = (CvRect*)cvGetSeqElem(rects, iface);

		bbox[0] = r->x;
		bbox[1] = r->y;
		bbox[2] = r->x + r->width;
		bbox[3] = r->y + r->height;

		flandmark_detect(input, bbox, model, landmarks);

		// display landmarks
		cvRectangle(orig, cvPoint(bbox[0], bbox[1]), cvPoint(bbox[2], bbox[3]), CV_RGB(255, 0, 0));
		cvRectangle(orig, cvPoint(model->bb[0], model->bb[1]), cvPoint(model->bb[2], model->bb[3]), CV_RGB(0, 0, 255));
		cvCircle(orig, cvPoint((int)landmarks[0], (int)landmarks[1]), 3, CV_RGB(0, 0, 255), CV_FILLED);
		for (int i = 2; i < 2 * model->data.options.M; i += 2)
		{
			cvCircle(orig, cvPoint(int(landmarks[i]), int(landmarks[i + 1])), 3, CV_RGB(255, 0, 0), CV_FILLED);

		}
	}
	t = (double)cvGetTickCount() - t;
	int ms = cvRound(t / ((double)cvGetTickFrequency() * 1000.0));

	if (nFaces > 0)
	{
		printf("Faces detected: %d; Detection of facial landmark on all faces took %d ms\n", nFaces, ms);
	}
	else {
		printf("NO Face\n");
	}

	cvReleaseMemStorage(&storage);
}


void FaceGrid::on_btn_run_clicked()
{
	//cv::Mat org_img = cv::imread("C:\\Users\\arman\\Desktop\\bilder\\lena.png");
	cv::namedWindow("MainWindow", CV_WINDOW_KEEPRATIO);
	//cv::imshow("MainWindow", org_img);

	// Haar Cascade file, used for Face Detection.
	char faceCascadeFilename[] = "C:\\Users\\arman\\Desktop\\Exjobb_NFC\\Exjobb\\FaceGrid\\FaceGrid\\haarcascade_frontalface_alt.xml";
	// Load the HaarCascade classifier for face detection.
	cv::CascadeClassifier faceCascade;
	faceCascade.load("C:\\Users\\arman\\Desktop\\Exjobb_NFC\\Exjobb\\FaceGrid\\FaceGrid\\haarcascade_frontalface_alt.xml");
	/*
	if (!faceCascade)
	{
		printf("Couldnt load Face detector '%s'\n", faceCascadeFilename);
		exit(1);
	}
	*/

		// load flandmark model structure and initialize
		FLANDMARK_Model * model = flandmark_init("flandmark_model.dat");
		// load input image
		IplImage *img = cvLoadImage("C:\\Users\\arman\\Desktop\\bilder\\lenna.jpg");
		// convert image to grayscale
		IplImage *img_grayscale = cvCreateImage(cvSize(img->width, img->height), IPL_DEPTH_8U, 1);
		cvCvtColor(img, img_grayscale, CV_BGR2GRAY);
		// bbox with detected face (format: top_left_col top_left_row bottom_right_col bottom_right_row)
		//int bbox[] = { 85, 78, 160, 170 };
		// detect facial landmarks (output are x, y coordinates of detected landmarks)
		double * landmarks = (double*)malloc(2 * model->data.options.M*sizeof(double));
		//flandmark_detect(img_grayscale, bbox, model, landmarks);

		int *bbox = (int*)malloc(4 * sizeof(int));
		//detectFaceInImage(img, img_grayscale, faceCascade, model, bbox, landmarks);

		//cvCircle(img, cvPoint((int)landmarks[0], (int)landmarks[1]), 3, CV_RGB(0, 0, 255), CV_FILLED);
		//for (int i = 2; i < 2 * model->data.options.M; i += 2)
		//{
		//	cvCircle(img, cvPoint(int(landmarks[i]), int(landmarks[i + 1])), 3, CV_RGB(255, 0, 0), CV_FILLED);
				//}
		cvShowImage("MainWindow", img);
}

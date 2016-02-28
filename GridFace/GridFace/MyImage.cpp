#include "MyImage.h"


MyImage::MyImage()
{
}


MyImage::~MyImage()
{
}

void MyImage::init()
{
	// Haar Cascade file, used for Face Detection.
	char faceCascadeFilename[] = "SVM_landmarks\\haarcascade_frontalface_alt.xml";
	// Load the HaarCascade classifier for face detection.
	if (!faceCascade.load(faceCascadeFilename))
	{
		// ERROR
		exit(1);
	}

	model_fland = flandmark_init("SVM_landmarks\\flandmark_model.dat"); // load flandmark model structure and initialize
}

QString MyImage::get_name(int option)
{
	switch (option)
	{
	case 1: 
		return file_name;
		break;
	case 2:
		return file_path;
		break;
	default: 
		return QString("Error");
		// EROOR 
		break; 

	}

}

void MyImage::set_name(QString new_name, int option)
{
	switch (option)
	{
	case 1:
		file_name = new_name;
		break;
	case 2:
		file_path = new_name; 
		break;
	default:
		
		// EROOR 
		break;

	}
}

void MyImage::insert_selection(cv::Point point1, cv::Point point2, int class_type)
{
	point1_vec.push_back(point1);
	point2_vec.push_back(point2);
	class_type_vec.push_back(class_type);

}

void MyImage::detect_landmarks(cv::Mat orig, IplImage* input, cv::CascadeClassifier cascade, FLANDMARK_Model *model)
{

	double *landmarks = (double*)malloc(2 * model->data.options.M*sizeof(double)); // Landmarks points 
	int *bbox = (int*)malloc(4 * sizeof(int)); // Bonding box

	// Face detectopn parameters
	CvSize minFeatureSize = cvSize(20, 20);	 // Smallest face size.
	int flags = CV_HAAR_DO_CANNY_PRUNING;
	float search_scale_factor = 1.1f;	// How detailed should the search be.


	//-- Detect faces
	cascade.detectMultiScale(orig, faces, search_scale_factor, 2, 0 | flags, minFeatureSize);

	for (int iface = 0; iface < faces.size(); ++iface)
	{
		// Insert bonding box
		bbox[0] = faces[iface].x;
		bbox[1] = faces[iface].y;
		bbox[2] = faces[iface].x + faces[iface].width;
		bbox[3] = faces[iface].y + faces[iface].height;

		// Detect landmarks
		flandmark_detect(input, bbox, model, landmarks);

		// Display landmarks
		cv::rectangle(orig, cvPoint(bbox[0], bbox[1]), cvPoint(bbox[2], bbox[3]), CV_RGB(0, 0, 255), 2);
		cv::rectangle(orig, cvPoint(model->bb[0], model->bb[1]), cvPoint(model->bb[2], model->bb[3]), CV_RGB(0, 255, 0), 2);
		cv::circle(orig, cvPoint((int)landmarks[0], (int)landmarks[1]), 5, CV_RGB(0, 255, 0), CV_FILLED);
		cv::putText(orig, std::to_string(0), cvPoint(int(landmarks[0]) + 5 , int(landmarks[1]) + 5),
			CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0, 255, 0), 2);
		for (int i = 2; i < 2 * model->data.options.M; i += 2)
		{
			cv::circle(orig, cvPoint(int(landmarks[i]), int(landmarks[i + 1])), 5, CV_RGB(0, 0, 255), CV_FILLED);
			cv::putText(orig, std::to_string(i/2), cvPoint(int(landmarks[i]) + 5 , int(landmarks[i + 1]) + 5),
				CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0, 0, 255), 2);

		}
	}

}

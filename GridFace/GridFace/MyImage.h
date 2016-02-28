#ifndef MYIMAGE_H
#define MYIMAGE_H

#include <qstring.h>
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define DLIB_JPEG_SUPPORT
#define DLIB_PNG_SUPPORT
#include <../image_processing/frontal_face_detector.h>
#include <../image_processing/render_face_detections.h>
#include <../image_processing.h>
#include <../gui_widgets.h>
#include <../image_io.h>

#include "SVM_landmarks\flandmark_detector.h"



 
class MyImage
{


public:
	MyImage();
	~MyImage();

	// Public funtions 

	void init(); 

	/*
	option 1 = name of file
	option 2 = name of full path
	*/
	QString get_name(int option);  // Get filename or path 
	void set_name(QString new_name, int option); // Set filename or path 

	void insert_selection(cv::Point point1, cv::Point point2, int class_type); // Set point and class of facial marks
	
	void detect_landmarks(cv::Mat orig, IplImage* input, cv::CascadeClassifier cascade, FLANDMARK_Model *model);
	//void dlib_landmarks(cv::Mat orig);


	// Public data members

	cv::CascadeClassifier faceCascade;
	FLANDMARK_Model *model_fland;
	dlib::frontal_face_detector model_dlib; 

	std::vector<cv::Point> point1_vec; // Vector with points of facial marks 
	std::vector<cv::Point> point2_vec; // Vector with points of facial marks
	std::vector<int> class_type_vec; // 1 is permanent, 2 is non-permanent 
	std::vector<cv::Rect> faces; // Vector rects containing faces 


private:
	// Data members
	QString file_path; // Whole path 
	QString file_name; // File name 

	// Functions 
	
};

#endif
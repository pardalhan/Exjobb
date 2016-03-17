#ifndef MYIMAGE_H
#define MYIMAGE_H

#define _USE_MATH_DEFINES

#include <qstring.h>
#include <vector>
#include <cmath>
#include <math.h> 
#include <QMessageBox>

#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <../opencv/cv_image.h>

//#include "SVM_landmarks\flandmark_detector.h"

#include <../image_processing/frontal_face_detector.h>
#include <../image_processing/render_face_detections.h>
#include <../image_processing.h>
#include <../image_io.h>

#include "normalize/SLRMSR.h"
#include "normalize/C_BADGER.h"
#include "normalize/MSR.h"


 
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
	/*
	option 1 = name of file
	option 2 = name of full path
	*/
	void set_name(QString new_name, int option); // Set filename or path 

	void insert_selection(cv::Point point1, cv::Point point2, int class_type); // Set point and class of facial marks
	
	void detect_faces(cv::Mat orig);
	void detect_landmarks(cv::Mat orig_img);
	cv::Mat MyImage::rotate_img(cv::Mat orig_img);
	cv::Rect enlarg_face_rect(cv::Rect face, double procent, cv::Size img_size);
	std::pair<cv::Mat, cv::Mat> MyImage::face_segment(cv::Mat cv_img, std::vector<cv::Point> landmark_vec, bool down_sample);
	cv::Mat MyImage::close_open(cv::Mat mask, int iterations = 7, int kernels_size = 5);

	void display_landmarks(cv::Mat orig_img);
	void draw_grid(cv::Mat img);

	void clear_data(); // Clear all data in object

	// Public data members

	cv::CascadeClassifier face_Cascade;
	//FLANDMARK_Model *model_fland;
	dlib::shape_predictor model_dlib;

	std::vector<cv::Point> point1_vec; // Vector with points of facial marks 
	std::vector<cv::Point> point2_vec; // Vector with points of facial marks
	std::vector<int> class_type_vec; // 1 is permanent, 2 is non-permanent 
	std::vector<cv::Rect> faces_vec; // Vector rects containing faces 
	std::vector<cv::Point> landmark_vec; // Vector with landmark points
	QMessageBox message;


private:
	// Data members
	QString file_path; // Whole path 
	QString file_name; // File name 

	// Functions 
	//void landmarks_SVM(IplImage *grey_img, int *bbox, FLANDMARK_Model *model, double *landmarks);

	void display_message(QString msg); 
	double calc_angle(cv::Point a, cv::Point b, cv::Point c);

};

#endif
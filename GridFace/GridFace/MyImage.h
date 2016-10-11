#ifndef MYIMAGE_H
#define MYIMAGE_H

#define _USE_MATH_DEFINES

#include <qstring.h>
#include <vector>
#include <cmath>
#include <math.h> 
#include <QMessageBox>
#include <Qfiledialog.h>

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
	std::string get_name(int option);  // Get filename or path 
	/*
	option 1 = name of file
	option 2 = name of full path
	*/
	void set_name(std::string new_name, int option); // Set filename or path 

	void insert_selection(cv::Point point1, cv::Point point2, int class_type); // Set point and class of facial marks
	
	void detect_faces(cv::Mat orig);
	void detect_landmarks(cv::Mat orig_img);
	void calc_angle();
	cv::Rect MyImage::enlarg_rect_to(cv::Rect rect);
	cv::Mat MyImage::rotate_img(cv::Mat orig_img);
	std::vector<cv::Point>  MyImage::rotate_points(std::vector<cv::Point> point_vec);
	cv::Rect enlarg_face_rect(cv::Rect face, double procent, cv::Size img_size);
	std::pair<cv::Mat, cv::Mat> MyImage::face_segment(cv::Mat cv_img, std::vector<cv::Point> landmark_vec, bool down_sample);
	cv::Mat MyImage::close_open(cv::Mat mask, int iterations = 7, int kernels_size = 5);
	cv::Mat MyImage::true_colours(cv::Mat image, int colour);

	void display_landmarks(cv::Mat orig_img);
	void draw_grid(cv::Mat img);
	void MyImage::draw_record(cv::Mat img);

	void clear_data(); // Clear all data in object

	void MyImage::intersecting_areas(MyImage& actual, MyImage& predicted);
	void MyImage::rezize_points(MyImage& my_image);

	void MyImage::post_processing(cv::Mat cv_img);

	void MyImage::process_image(MyImage& my_image);

	void MyImage::create_areas();
	void MyImage::calc_freq(MyImage& my_imgae);

	// SVM 
	std::vector<MyImage> extract_from_record(std::string file_path);
	std::vector<int> extract_lables(std::vector<MyImage> record);
	cv::Mat extract_features(std::vector<MyImage> record, int nr_samples);
	cv::Ptr<cv::ml::SVM> train_SVM(cv::Mat training_data, cv::Mat training_labels);
	cv::Mat calc_confusion(cv::Mat predicted, cv::Mat actual);


	cv::Mat MyImage::FRS(cv::Mat cv_img);
	std::pair<std::vector<std::vector<cv::Point>>, std::vector<cv::Rect>> MyImage::get_candidates(cv::Mat cv_img, cv::Mat mask, cv::Mat frs_img, double threshold);
	
	// Public data members
	cv::CascadeClassifier face_Cascade;
	//FLANDMARK_Model *model_fland;
	dlib::shape_predictor model_dlib;

	cv::Ptr<cv::SimpleBlobDetector> blob_detector;
	cv::Mat w2c;

	std::vector<cv::Point> point1_vec; // Vector with points of facial marks 
	std::vector<cv::Point> point2_vec; // Vector with points of facial marks
	std::vector<int> class_type_vec; // 1 is permanent, 2 is non-permanent 
	std::vector<cv::Rect> faces_vec; // Vector rects containing faces 
	std::vector<cv::Point> landmark_vec; // Vector with landmark points
	std::vector<std::vector<cv::Point>> areas_vec;

	std::vector<int> index_checked_detections;
	float resize_factor;
	float rotate_angle; 
	cv::Size img_size; 
	std::vector<cv::Rect> candidates; 
	std::vector<int> frequency;

private:
	// Data members
	std::string file_path; // Whole path 
	std::string file_name; // File name 

	// Functions 

	void display_message(QString msg);
	float MyImage::my_angle_calculater(cv::Point a, cv::Point b, cv::Point c);


};

#endif
#ifndef MYIMAGE_H
#define MYIMAGE_H

#include <qstring.h>
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

class MyImage
{

 
public:
	MyImage();
	~MyImage();

	// Public funtions 
	/* 
	option 1 = name of file
	option 2 = name of full path
	*/
	QString get_name(int option);
	void set_name(QString new_name, int option);
	void insert_selection(cv::Point point1, cv::Point point2, int class_type);


	// Public data members
	std::vector<cv::Point> point1_vec;
	std::vector<cv::Point> point2_vec;
	std::vector<int> class_type_vec; // 1 is permanent, 2 is non-permanent 

private: 
	// Data members
	QString file_path; 
	QString file_name; 

};

#endif
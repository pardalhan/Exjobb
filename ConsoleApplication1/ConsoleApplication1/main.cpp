
#include "conio.h"
#include <iostream>

#include <opencv2\opencv.hpp>

using namespace cv;
/*
void MouseHandler(int event, int x, int y, int flags, void* userdata)
{
	if (event == CV_EVENT_LBUTTONDOWN && !drag)
	{
		// left button clicked. ROI selection begins 
		point1 = cv::Point(x, y);
		drag = 1;
		std::cout << "Nu?" << std::endl;
	}

	if (event == CV_EVENT_MOUSEMOVE && drag)
	{
		// mouse dragged. ROI being selected 
		cv::Mat img1 = img.clone();
		point2 = cv::Point(x, y);
		cv::rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 3, 8, 0);
		cv::imshow("MyWindow", img1);
	}

	if (event == CV_EVENT_LBUTTONUP && drag)
	{
		point2 = cv::Point(x, y);
		cv::Rect rect = Rect(5, 5, 100, 100);
		drag = 0;
		roiImg = img(rect);
		cv::namedWindow("ROI", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
		cv::imshow("ROI", roiImg); 
	}

	if (event == CV_EVENT_LBUTTONUP)
	{
		// ROI selected 
		
		select_flag = 1;
		drag = 0;

		if (select_flag == 1)
		{
			cv::imshow("ROI", roiImg); // show the image bounded by the box 
		}
		cv::rectangle(img, rect, CV_RGB(255, 0, 0), 3, 8, 0);
		cv::imshow("MyWindow", img);
		cv::waitKey(0); 
	}
}
*/
Mat img, roiImg;
Point point1, point2;
cv::Rect rect;
int drag, select_flag;

void main(void)
{
	
	Mat img = cv::imread("C:\\Users\\arman\\Desktop\\Exjobb_NFC\\lena.png");
	namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
	//setMouseCallback("MyWindow", MouseHandler, NULL);
	imshow("MyWindow", img); //display the image which is stored in the 'img' in the "MyWindow" window
	
	point1 = Point(50, 50);
	point2 = Point(150, 150);
	Mat img1 = img.clone();
	cv::rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 3, 8, 0);
	rect = cv::Rect(point1, point2);
	roiImg = img(rect);

	imshow("MyWindow", img1);
	cvNamedWindow("Roi", CV_WINDOW_KEEPRATIO);
	imshow("Roi", roiImg);
	
	waitKey(0);
	destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"
	std::cout << "Hej hej";

	_getch();
}
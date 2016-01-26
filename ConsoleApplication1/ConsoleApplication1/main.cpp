
#include "conio.h"
#include <iostream>

#include <opencv2\opencv.hpp>

using namespace cv;
void main(void)
{
	Mat img = cv::imread("D:\Testdata\\lena.png");
	namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
	imshow("MyWindow", img); //display the image which is stored in the 'img' in the "MyWindow" window
	waitKey(0);
	destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"
	std::cout << "Hej hej";

	_getch();
}
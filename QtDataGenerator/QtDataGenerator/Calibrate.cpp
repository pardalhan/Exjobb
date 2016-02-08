#include "Calibrate.h"
#include "MyImage.h"
#include <QtWidgets/QMainWindow>
#include "ui_qtdatagenerator.h"
#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Qfiledialog.h>
//#include <QDialog>
#include <QPainter>
//#include <QRect>
//#include <QBrush>
//#include <QApplication>
#include <QMouseEvent>
#include <QCursor>
#include <QMessageBox>
#include <vector>
#include <string>

Calibrate::Calibrate()
{
}


Calibrate::~Calibrate()
{
}


void Calibrate::MouseHandler(int event, int x, int y, int flags, void* userdata)
{
	if (event == cv::EVENT_LBUTTONDOWN)
	{
		//ui.lbl_2->setText("Left");
		std::string test = "HEHE";

	}
	else if (event == cv::EVENT_RBUTTONDOWN)
	{
		//ui.lbl_2->setText("Right");
	}
	else if (event == cv::EVENT_MBUTTONDOWN)
	{
		//ui.lbl_2->setText("Middle");
	}
	else if (event == cv::EVENT_MOUSEMOVE)
	{
		//ui.lbl_2->setText("Move");
	}
}

void Calibrate::mouseWrapper(int event, int x, int y, int flags, void *param)
{
	Calibrate* cal = (Calibrate*)param; // cast back to 'this'
	// now call your member-function.
	cal->MouseHandler(event, x, y, flags, 0);
}

void Calibrate::init()
{
	// call the wrapper function instead, and
	// pass this pointer for later redirection
	cv::setMouseCallback("MyWindow", mouseWrapper, this);
}
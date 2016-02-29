#ifndef GRIDFACE_H
#define GRIDFACE_H
//#define DLIB_JPEG_SUPPORT
//#define DLIB_PNG_SUPPORT

#include <QtWidgets/QMainWindow>
#include "ui_gridface.h"
#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Qfiledialog.h>
//#include <qdebug.h>
//#include <qdiriterator.h>
//#include <QMouseEvent>
#include <QMessageBox>
#include <vector>
#include <string>
#include <qmessagebox.h>
//#include <iostream>
//#include <fstream>

#include "MyImage.h"


 

class GridFace : public QMainWindow
{
	Q_OBJECT

public:
	GridFace(QWidget *parent = 0);
	~GridFace();

private slots:
	void on_btn_run_clicked();
	void on_btn_second_clicked();
	void on_btn_browser_clicked();

private:
	Ui::GridFaceClass ui;

	void detectFaceInImage(cv::Mat orig, IplImage* input, cv::CascadeClassifier cascade, FLANDMARK_Model *model, int *bbox, double *landmarks);


	// Data members 
	int file_counter;
	MyImage *curr_img;
	QStringList list_of_files;
	bool my_switch;
	QMessageBox message; 
};

#endif // GRIDFACE_H

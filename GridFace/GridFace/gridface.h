#ifndef GRIDFACE_H
#define GRIDFACE_H

#include <QtWidgets/QMainWindow>
#include "ui_gridface.h"
#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Qfiledialog.h>
//#include <qdebug.h>
//#include <qdiriterator.h>
//#include <QMouseEvent>

#include <vector>
#include <string>
#include <QMessageBox>
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
	void on_btn_next_run_clicked();
	void on_btn_train_clicked();
	void on_btn_validate_clicked();
	void on_btn_browser_clicked();
	void on_btn_next_validate_clicked();

	void run_image();
	void validate_image();

private:
	Ui::GridFaceClass ui;

	//void detectFaceInImage(cv::Mat orig, IplImage* input, cv::CascadeClassifier cascade, FLANDMARK_Model *model, int *bbox, double *landmarks);

	// Data members 
	MyImage *curr_img;
	int file_counter = 0;
	//QFileInfoList list_of_files;
	QStringList list_of_files;
	QMessageBox message;
	cv::Ptr<cv::ml::SVM> my_svm = cv::ml::SVM::create();	
	std::string dir_path; 
	
	std::vector<MyImage> processed_img_vec;
	std::vector<MyImage> record_vec;
};

#endif // GRIDFACE_H

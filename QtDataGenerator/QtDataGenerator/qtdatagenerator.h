#ifndef QTDATAGENERATOR_H
#define QTDATAGENERATOR_H


#include "MyImage.h"
#include <QtWidgets/QMainWindow>
#include "ui_qtdatagenerator.h"
#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Qfiledialog.h>
#include <qdebug.h>
#include <qdiriterator.h>
#include <QMouseEvent>
#include <QMessageBox>
#include <vector>
#include <string>
#include <qmessagebox.h>
#include <iostream>
#include <fsteam>

class QtDataGenerator : public QMainWindow
{
	Q_OBJECT

public:

	QtDataGenerator(QWidget *parent = 0);
	~QtDataGenerator();



private slots:
	void on_btn_next_clicked();
	void on_btn_browser_clicked();

private:
	std::vector<MyImage> myimage_vec;

	// Mouse handler 
	void MouseHandler(int event, int x, int y, int flags, void* userdata);
	static void mouseWrapper(int event, int x, int y, int flags, void *param);
	void display_image(QString filepath);


	// GUI
	Ui::QtDataGeneratorClass ui;

	// Data members
	QStringList list_of_files;
	cv::Point point1, point2; 
	int drag, file_counter = 0;	
	cv::Rect rect; 
	cv::Mat org_img, roi_img = cv::Mat::zeros(20, 20, CV_8UC3);
	QString folder_path;


protected: 


};

#endif // QTDATAGENERATOR_H

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
#include <fstream>
 
class QtDataGenerator : public QMainWindow
{
	Q_OBJECT

public:

	QtDataGenerator(QWidget *parent = 0);
	~QtDataGenerator();



private slots:
	// Buttons 
	void on_btn_next_clicked();
	void on_btn_browser_clicked();

private:
	

	// Mouse handler 
	void MouseHandler(int event, int x, int y, int flags, void* userdata);
	static void mouseWrapper(int event, int x, int y, int flags, void *param);

	// Display image 
	void display_image(QString filepath);

	// Writing to file
	bool if_file_exist(std::string fileName);
	void pre_writing(); 
	void wite_all_selections(); 
	
	// GUI
	Ui::QtDataGeneratorClass ui;

	// Data members
	QStringList list_of_files;
	QString folder_path;

	cv::Point point1, point2; 
	int drag, file_counter = 0;	
	cv::Rect rect; 
	cv::Mat org_img, roi_img;
	
	std::ofstream outPutStream;

	std::vector<MyImage> myimage_vec;
	MyImage *curr_image;

protected: 


};

#endif // QTDATAGENERATOR_H

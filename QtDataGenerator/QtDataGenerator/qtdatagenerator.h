#ifndef QTDATAGENERATOR_H
#define QTDATAGENERATOR_H


#include "MyImage.h"
//#include "Calibrate.h"
#include <QtWidgets/QMainWindow>
#include "ui_qtdatagenerator.h"
#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Qfiledialog.h>
#include <QPainter>
#include <qdebug.h>
#include <qdiriterator.h>
#include <QMouseEvent>
#include <QCursor>
#include <QMessageBox>
#include <vector>
#include <string>

class QtDataGenerator : public QMainWindow
{
	Q_OBJECT

public:

	QtDataGenerator(QWidget *parent = 0);
	QtDataGenerator(cv::Mat image);
	~QtDataGenerator();



private slots:
	void on_btn_1_clicked();
	void on_btn_browser_clicked();

private:
	std::vector<MyImage> myimage_vec;

	void mouseMoveEvent(QMouseEvent *ev); //Moving the mouse while pressed
	void mousePressEvent(QMouseEvent * ev); // Pressed mouse
	void mouseDoubleClickEvent(QMouseEvent * ev); // Doubble pressed

	// Mouse handler 
	void MouseHandler(int event, int x, int y, int flags, void* userdata);
	static void mouseWrapper(int event, int x, int y, int flags, void *param);


	void MYtester();

	// GUI
	Ui::QtDataGeneratorClass ui;

	// Data members
	cv::Point point1, point2; 
	int drag = 0;	
	cv::Rect rect; 
	cv::Mat org_img, roi_img, altered_image; 
	QString folder_path; 


protected: 


};

#endif // QTDATAGENERATOR_H

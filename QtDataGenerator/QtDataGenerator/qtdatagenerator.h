#ifndef QTDATAGENERATOR_H
#define QTDATAGENERATOR_H


#include "MyImage.h"
//#include "Calibrate.h"
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

class QtDataGenerator : public QMainWindow
{
	Q_OBJECT

public:

	QtDataGenerator(QWidget *parent = 0);
	~QtDataGenerator();



private slots:
	void on_btn_1_clicked();
	void on_btn_browser_clicked();

private:
	std::vector<MyImage> myimage_vec;

	void mouseMoveEvent(QMouseEvent *ev); //Moving the mouse while pressed
	void mousePressEvent(QMouseEvent * ev); // Pressed mouse
	void mouseDoubleClickEvent(QMouseEvent * ev); // Doubble pressed

	void MouseHandler(int event, int x, int y, int flags, void* userdata);
	static void mouseWrapper(int event, int x, int y, int flags, void *param);
	void init();

	void MYtester();

	Ui::QtDataGeneratorClass ui;

	int tester = 9;
	cv::Point point1, point2; /* vertical points of the bounding box */
	int drag = 0;
	cv::Rect rect; /* bounding box */
	cv::Mat img, roiImg; /* roiImg - the part of the image in the bounding box */
	int select_flag = 0;

protected: 


};

#endif // QTDATAGENERATOR_H

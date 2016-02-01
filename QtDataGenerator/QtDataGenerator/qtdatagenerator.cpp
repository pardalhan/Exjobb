#include "qtdatagenerator.h"


QtDataGenerator::QtDataGenerator(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

QtDataGenerator::~QtDataGenerator()
{

}

void QtDataGenerator::on_btn_1_clicked()
{
	// Read image to MAT
	cv::Mat CVimg = cv::imread("D:\Testdata\\lena.png");

	// Convert image from MAT to Q-image
	cv::Mat dest;
	cv::cvtColor(CVimg, dest, CV_BGR2RGB);
	QImage Qimage((uchar*)dest.data, dest.cols, dest.rows, QImage::Format_RGB888);


	// Display image on label
	//ui.lbl_1->setPixmap(QPixmap::fromImage(Qimage));
	ui.lbl_1->setPixmap(QPixmap::fromImage(Qimage).scaled(ui.lbl_1->width(), ui.lbl_1->height(), Qt::KeepAspectRatio));
	ui.lbl_1->setScaledContents(true);
	ui.lbl_1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

}

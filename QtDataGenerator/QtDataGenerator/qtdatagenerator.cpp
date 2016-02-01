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
	
	cv::Mat CVimg = cv::imread("D:\Testdata\\lena.png");
	ui.imageWidget->showImage(CVimg);

	cv::Mat dest;
	cv::cvtColor(CVimg, dest, CV_BGR2RGB);
	QImage Qimage((uchar*)dest.data, dest.cols, dest.rows, QImage::Format_RGB888);

	//ui.pictureBox->sceneRect(Qimage.rect());
	ui.lbl_1->setPixmap(QPixmap::fromImage(Qimage));

}

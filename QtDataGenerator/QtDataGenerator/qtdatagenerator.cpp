#include "qtdatagenerator.h"


QtDataGenerator::QtDataGenerator(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	setMouseTracking(true);
}

QtDataGenerator::~QtDataGenerator()
{

}

void QtDataGenerator::on_btn_1_clicked()
{
	int clickcount = 0;

	if (clickcount == 0)
	{
	//// Read image to MAT
	//cv::Mat CVimg = cv::imread("D:\Testdata\\lena.png");

	//// Convert image from MAT to Q-image
	//cv::Mat dest;
	//cv::cvtColor(CVimg, dest, CV_BGR2RGB);
	//QImage Qimage((uchar*)dest.data, dest.cols, dest.rows, QImage::Format_RGB888);


		//ui.lbl_2->setText(QString::number(QCursor::pos()));
		ui.lbl_2->setText(QString::number(height()));
	}
	else
	{
		ui.lbl_2->setText("DÅ DÅ DÅ");
	}
	clickcount = clickcount + 1;




}
void QtDataGenerator::on_btn_browser_clicked()
{
	QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));

	if (QString::compare(filename, QString()) != 0) // Check if the QString is empty 
	{
		QImage image;
		bool valid_image = image.load(filename); // Make sure that a image is loaded

		if (valid_image)
		{
			// Display image on label
			ui.lbl_1->setPixmap(QPixmap::fromImage(image).scaled(ui.lbl_1->width(), ui.lbl_1->height(), Qt::KeepAspectRatio));
			ui.lbl_1->setScaledContents(true);
			ui.lbl_1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		}
		else
		{
			// ERROR!
		}
	}
}

void QtDataGenerator::mouseMoveEvent(QMouseEvent *ev)
{
	QPoint pos = ev->pos();
	ui.lbl_2->setText(QString("%1, %2").arg(pos.x()).arg(pos.y()));
}


void QtDataGenerator::paintEvent(QPaintEvent *e)
{
	QPainter painter(this);

	QRect r1(0, 0, 200, 200);

	painter.setPen(Qt::blue);
	painter.drawRect(r1);
	painter.setPen(Qt::red);
	painter.drawEllipse(r1);
}
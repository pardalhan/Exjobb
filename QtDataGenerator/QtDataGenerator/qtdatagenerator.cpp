#include "qtdatagenerator.h"



QtDataGenerator::QtDataGenerator(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	setMouseTracking(true);
}
QtDataGenerator::QtDataGenerator(cv::Mat image)
{
	altered_image = image;
	org_img = image;
	
}

QtDataGenerator::~QtDataGenerator()
{

}

void QtDataGenerator::on_btn_1_clicked()
{

	//ui.lbl_2->setText(QString::number(QCursor::pos()));

	ui.lbl_2->setText("DÅ DÅ DÅ");	
	qDebug() << "HEHE"; 

}

void QtDataGenerator::on_btn_browser_clicked()
{
	/*
	QString folder_path = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ReadOnly);
	//QString folder_path = "D:\Testdata\ref_bilder";
	QDirIterator dir_iter(folder_path, QStringList() << "*.png *.jpg *.jpeg *.bmp *.gif", QDir::Files, QDirIterator::Subdirectories);
	QString filename = dir_iter.filePath();
	*/
	QString filename = QFileDialog::getOpenFileName(this, tr("Choose"), "", tr("Images (*.png *.jpg *.jpeg *.bmp *.gif)"));

	if (QString::compare(filename, QString()) != 0) // Check if the QString is empty 
	{
		QImage image;
		bool valid_image = image.load(filename); // Make sure that a image is loaded

		if (valid_image)
		{
			// Display image on label
			ui.lbl_1->setPixmap(QPixmap::fromImage(image).scaled(ui.lbl_1->width(), ui.lbl_1->height(), Qt::KeepAspectRatio));

		}
		else
		{
			// ERROR!
		}
	}
	// Create a image object and put it into the image vector vvv
	MyImage curr_image;
	curr_image.set_filename(filename);
	myimage_vec.push_back(curr_image);

	// Read image to MAT
	org_img = cv::imread(filename.toStdString().c_str()); // Convert QString to string for loading image
	cv::namedWindow("MyWindow", CV_WINDOW_KEEPRATIO); //create a window with the name "MyWindow"
	cv::imshow("MyWindow", org_img); // Show image in window 


	//cv::setMouseCallback("MyWindow", CallBackFunc, NULL);
	QtDataGenerator test(org_img);
	cv::setMouseCallback("MyWindow", mouseWrapper, this);



}

void QtDataGenerator::mouseMoveEvent(QMouseEvent *ev)
{
	//QPoint pos = ev->pos();
	//ui.lbl_2->setText(QString("%1, %2").arg(pos.x()).arg(pos.y()));
}
void QtDataGenerator::mousePressEvent(QMouseEvent *ev)
{
	if (ev->button() == Qt::LeftButton)
	{
		QPoint pos = ev->pos();
		ui.lbl_2->setText(QString::number(ev->x()));

	}

	if (ev->button() == Qt::RightButton)
	{
		QPoint pos = ui.lbl_2->mapFrom(this, ev->pos());
		ui.lbl_2->setText(QString::number(pos.x()));


	}
}
void QtDataGenerator::mouseDoubleClickEvent(QMouseEvent *ev)
{

		QPoint pos = ev->pos();
		ui.lbl_2->setText("Double");
}

void QtDataGenerator::MYtester()
{

}

void QtDataGenerator::MouseHandler(int event, int x, int y, int flags, void* userdata)
{
	// LEFT BUTTOM ----------------------------------------
	if (event == CV_EVENT_LBUTTONDOWN && !drag)
	{
		// left button clicked. ROI selection begins 
		point1 = cv::Point(x, y);
		drag = 1;
	}
	if (event == CV_EVENT_MOUSEMOVE && drag)
	{
		// mouse dragged. ROI being selected 
		cv::Mat img1 = org_img.clone();
		point2 = cv::Point(x, y);
		cv::rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 1, 8, 0);
		cv::imshow("MyWindow", img1);
	}
	if (event == CV_EVENT_LBUTTONUP && drag)
	{
		// left button up. ROI selection ended
		point2 = cv::Point(x, y);
		rect = cv::Rect(point1, point2);
		drag = 0;

		// OBS DET ÄR HÄR SOM VARJE SUBWINDOW INTE VISAR SIG 
		if (roi_img.empty())
		{
			roi_img = org_img(rect);
			cv::namedWindow("ROI", CV_WINDOW_AUTOSIZE);
			cv::imshow("ROI", roi_img);
		}
	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		// ROI selected 
		cv::rectangle(org_img, rect, CV_RGB(0, 255, 0), 2, 8, 0);
		cv::imshow("MyWindow", org_img);

	}
	if (event == CV_EVENT_LBUTTONDBLCLK)
	{

	}
	// RIGHT BUTTOM ---------------------------------
	// LEFT BUTTOM ----------------------------------------
	if (event == CV_EVENT_RBUTTONDOWN && !drag)
	{
		// left button clicked. ROI selection begins 
		point1 = cv::Point(x, y);
		drag = 1;
	}
	if (event == CV_EVENT_MOUSEMOVE && drag)
	{
		// mouse dragged. ROI being selected 
		cv::Mat img1 = org_img.clone();
		point2 = cv::Point(x, y);
		cv::rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 1, 8, 0);
		cv::imshow("MyWindow", img1);
	}
	if (event == CV_EVENT_RBUTTONUP && drag)
	{
		// left button up. ROI selection ended
		point2 = cv::Point(x, y);
		rect = cv::Rect(point1, point2);
		drag = 0;

		// OBS DET ÄR HÄR SOM VARJE SUBWINDOW INTE VISAR SIG 
		if (roi_img.empty())
		{
			roi_img = org_img(rect);
			cv::namedWindow("ROI", CV_WINDOW_AUTOSIZE);
			cv::imshow("ROI", roi_img);
		}
	}
	if (event == CV_EVENT_RBUTTONUP)
	{
		// ROI selected 
		cv::rectangle(org_img, rect, CV_RGB(0, 0, 255), 2, 8, 0);
		cv::imshow("MyWindow", org_img);

	}
	if (event == CV_EVENT_RBUTTONDBLCLK)
	{

	}
}

void QtDataGenerator::mouseWrapper(int event, int x, int y, int flags, void *param)
{
	QtDataGenerator* cal = (QtDataGenerator*)param; // cast back to 'this'
	// now call your member-function.
	cal->MouseHandler(event, x, y, flags, 0);
}









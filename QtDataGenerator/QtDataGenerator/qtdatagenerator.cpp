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

	//ui.lbl_2->setText(QString::number(QCursor::pos()));

	ui.lbl_2->setText("DÅ DÅ DÅ");

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
			//ui.lbl_1->setScaledContents(false);
			//ui.lbl_1->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
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
	img = cv::imread(filename.toStdString().c_str());
	cv::namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
	cv::imshow("MyWindow", img);


	//cv::setMouseCallback("MyWindow", CallBackFunc, NULL);
	QtDataGenerator test;
	test.init();



	//cv::imshow("MyWindow", CVimg); //display the image which is stored in the 'img' in the "MyWindow" window
	//cv::waitKey(0);
	//cv::destroyWindow("MyWindow"); //destroy the window with the name, "MyWindow"
	//// Convert image from MAT to Q-image
	//cv::Mat dest;
	//cv::cvtColor(CVimg, dest, CV_BGR2RGB);
	//QImage Qimage((uchar*)dest.data, dest.cols, dest.rows, QImage::Format_RGB888);

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
	cv::imshow("MyWindow", img);
}

void QtDataGenerator::MouseHandler(int event, int x, int y, int flags, void* userdata)
{
	if (event == CV_EVENT_LBUTTONDOWN && !drag)
	{
		///* left button clicked. ROI selection begins */
		point1 = cv::Point(x, y);
		drag = 1;
	}

	if (event == CV_EVENT_MOUSEMOVE && drag)
	{
		///* mouse dragged. ROI being selected */
		cv::Mat img1 = img.clone();
		point2 = cv::Point(x, y);
		cv::rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 3, 8, 0);
	//	MYtester();
	//	cv::imshow("MyWindow", img);
	}

	if (event == CV_EVENT_LBUTTONUP && drag)
	{
		point2 = cv::Point(x, y);
		rect = cv::Rect(5, 5, 100, 100);
		drag = 0;
		//roiImg = img(rect);
		//cv::namedWindow("ROI", CV_WINDOW_AUTOSIZE); //create a window with the name "MyWindow"
		//cv::imshow("ROI", roiImg);
	}

	if (event == CV_EVENT_LBUTTONUP)
	{
		///* ROI selected */
		select_flag = 1;
		drag = 0;

		//if (select_flag == 1)
		//{
		//	cv::imshow("ROI", roiImg); /* show the image bounded by the box */
		//}
		//cv::rectangle(img, rect, CV_RGB(255, 0, 0), 3, 8, 0);
		//cv::imshow("MyWindow", img);
		//cv::waitKey(0);
	}
}

void QtDataGenerator::mouseWrapper(int event, int x, int y, int flags, void *param)
{
	QtDataGenerator* cal = (QtDataGenerator*)param; // cast back to 'this'
	// now call your member-function.
	cal->MouseHandler(event, x, y, flags, 0);
}

void QtDataGenerator::init()
{
	// call the wrapper function instead, and
	// pass this pointer for later redirection
	cv::setMouseCallback("MyWindow", mouseWrapper, this);
}









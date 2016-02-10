#include "qtdatagenerator.h"
/*
QDir d = QFileInfo(files[0]).absoluteDir();
QString test = d.absolutePath();
*/
QtDataGenerator::QtDataGenerator(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	setMouseTracking(true);
}


QtDataGenerator::~QtDataGenerator()
{

}

void QtDataGenerator::on_btn_next_clicked()
{
	file_counter++;
	if (file_counter < this->list_of_files.size()) // Display image if the filepath is not empty
	{
		this->display_image(list_of_files[file_counter]);
		ui.lbl_2->setText("clicked");
	}
	else
	{
		// ERROR!
	}	

	if ((file_counter + 1) >= this->list_of_files.size()) // Disable next buttom when file list is empty
	{
		ui.btn_next->setEnabled(false);
	}
}

void QtDataGenerator::on_btn_browser_clicked() // Get a list with all the files which should be examined
{
	QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", "D:\\Testdata\\ref_bilder", "*.png *.jpg *.jpeg *.bmp *.gif");
	this->list_of_files = files;
	QString file_path = list_of_files[file_counter]; 
	display_image(file_path); // Display image from filepath 

	cv::setMouseCallback("MyWindow", mouseWrapper, this);
	ui.btn_next->setEnabled(true);
	this->drag = 0;
	/*
	// Create a image object and put it into the image vector
	MyImage curr_image;
	curr_image.set_filename(filepath);
	this->myimage_vec.push_back(curr_image);
	*/
	

}


void QtDataGenerator::display_image(QString filepath)
{
	if (QString::compare(filepath, QString()) != 0) // Check if the QString is empty 
	{
		QImage image;
		bool valid_image = image.load(filepath); // Make sure that a image is loaded

		if (valid_image)
		{
			ui.lbl_1->setPixmap(QPixmap::fromImage(image).scaled(ui.lbl_1->width(), ui.lbl_1->height(), Qt::KeepAspectRatio));// Display image on label
			this->org_img = cv::imread(filepath.toStdString().c_str()); // Convert QString to string for loading image
			cv::namedWindow("MyWindow", CV_WINDOW_KEEPRATIO); //create a window with the name "MyWindow"
			cv::imshow("MyWindow", org_img); // Show image in window 
		}
		else
		{
			// ERROR!
		}
	}
	
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
	cal->MouseHandler(event, x, y, flags, 0);	// now call your member-function.
}









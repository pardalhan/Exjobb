#include "qtdatagenerator.h"

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
	
	// Write to file
	this->pre_writing();
	this->wite_all_selections();
	this->outPutStream.close();

	//this->myimage_vec.push_back(curr_image);
	delete curr_image; 

	file_counter++;
	if (file_counter < this->list_of_files.size()) // Display image if the filepath is not empty
	{
		curr_image = new MyImage;
		this->display_image(list_of_files[file_counter]);
		this->curr_image->set_name(QFileInfo(list_of_files[file_counter]).fileName(), 1);
		this->curr_image->set_name(QFileInfo(list_of_files[file_counter]).absoluteDir().absolutePath(), 2);
		ui.lbl_2->setText("clicked");
	}
	else
	{
		ui.btn_next->setEnabled(false);
		cv::destroyAllWindows();
	}	

	if ((file_counter + 1) >= this->list_of_files.size()) // Disable next buttom when file list is empty
	{
		ui.btn_next->setText("DONE");

	}




}

void QtDataGenerator::on_btn_browser_clicked() // Get a list with all the files which should be examined
{
	QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", "D:\\Testdata\\ref_bilder", "*.png *.jpg *.jpeg *.bmp *.gif");
	this->folder_path = QFileInfo(files[0]).absoluteDir().absolutePath();
	this->list_of_files = files;
	QString file_path = list_of_files[file_counter]; 
	display_image(file_path); // Display image from filepath 

	cv::setMouseCallback("MyWindow", mouseWrapper, this);
	ui.btn_next->setEnabled(true);
	this->drag = 0;

	
	// Put file info in MyImage object 
	curr_image = new MyImage; 
	this->curr_image->set_name(QFileInfo(files[file_counter]).fileName(), 1);
	this->curr_image->set_name(file_path, 2);

}


void QtDataGenerator::display_image(QString filepath)
{
	if (QString::compare(filepath, QString()) != 0) // Check if the QString is empty 
	{
		QImage image;
		bool valid_image = image.load(filepath); // Make sure that a image is loaded

		if (valid_image)
		{
			//ui.lbl_1->setPixmap(QPixmap::fromImage(image).scaled(ui.lbl_1->width(), ui.lbl_1->height(), Qt::KeepAspectRatio));// Display image on label
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
		this->point1 = cv::Point(x, y);
		this->drag = 1;
	}
	if (event == CV_EVENT_MOUSEMOVE && drag)
	{
		// mouse dragged. ROI being selected 
		cv::Mat img1 = this->org_img.clone();
		this->point2 = cv::Point(x, y);
		cv::rectangle(img1, this->point1, this->point2, CV_RGB(255, 0, 0), 1, 8, 0);
		cv::imshow("MyWindow", img1);
	}
	if (event == CV_EVENT_LBUTTONUP && drag)
	{
		// left button up. ROI selection ended
		this->point2 = cv::Point(x, y);
		this->rect = cv::Rect(this->point1, this->point2);
		this->drag = 0;
		curr_image->insert_selection(this->point1, this->point2, 1); //Save selection

		// Display selected region
		if (rect.width != 0 || rect.height != 0)
		{
			this->roi_img = org_img(rect);
			cv::Mat temp; // make the same cv::Mat
			cvtColor(roi_img, temp, CV_BGR2RGB); // cvtColor Makes a copt, that what i need
			QImage dest((const uchar *)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
			//dest.bits(); // enforce deep copy, see documentation 
			ui.lbl_1->setPixmap(QPixmap::fromImage(dest).scaled(ui.lbl_1->width(), ui.lbl_1->height(), Qt::KeepAspectRatio));
		}

	}
	if (event == CV_EVENT_LBUTTONUP)
	{
		// ROI selected 
		cv::rectangle(this->org_img, this->rect, CV_RGB(0, 255, 0), 2, 8, 0);
		cv::imshow("MyWindow", this->org_img);
	}
	if (event == CV_EVENT_LBUTTONDBLCLK)
	{

	}
	// RIGHT BUTTOM ---------------------------------
	if (event == CV_EVENT_RBUTTONDOWN && !drag)
	{
		// left button clicked. ROI selection begins 
		this->point1 = cv::Point(x, y);
		this->drag = 1;
	}
	if (event == CV_EVENT_MOUSEMOVE && drag)
	{
		// mouse dragged. ROI being selected 
		cv::Mat img1 = this->org_img.clone();
		this->point2 = cv::Point(x, y);
		cv::rectangle(img1, this->point1, this->point2, CV_RGB(255, 0, 0), 1, 8, 0);
		cv::imshow("MyWindow", img1);
	}
	if (event == CV_EVENT_RBUTTONUP && drag)
	{
		// left button up. ROI selection ended
		this->point2 = cv::Point(x, y);
		this->rect = cv::Rect(this->point1, this->point2);
		this->drag = 0;

		curr_image->insert_selection(this->point1, this->point2, 2); // Save selection

		// Display selected region
		if (rect.width != 0 || rect.height != 0)
		{
			this->roi_img = org_img(rect);
			cv::Mat temp; // make the same cv::Mat
			cvtColor(roi_img, temp, CV_BGR2RGB); // cvtColor Makes a copt, that what i need
			QImage dest((const uchar *)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
			//dest.bits(); // enforce deep copy, see documentation 
			ui.lbl_1->setPixmap(QPixmap::fromImage(dest).scaled(ui.lbl_1->width(), ui.lbl_1->height(), Qt::KeepAspectRatio));
		}
	}
	if (event == CV_EVENT_RBUTTONUP)
	{
		// ROI selected 
		cv::rectangle(this->org_img, rect, CV_RGB(0, 0, 255), 2, 8, 0);
		cv::imshow("MyWindow", this->org_img);

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

bool QtDataGenerator::if_file_exist(std::string fileName)
{
	std::ifstream infile(fileName.c_str());
	return infile.good();
}

void QtDataGenerator::pre_writing()
{
	std::string path_of_file = this->folder_path.toStdString() + "\\ArmandTest.txt";
	if (this->if_file_exist(path_of_file))
	{
		if (this->outPutStream.is_open())
		{

		}
		else
		{
			outPutStream.open(path_of_file, std::ios::app | std::ios::out);
		}
	}
	else
	{
		this->outPutStream = std::ofstream(path_of_file);
	}
}

void QtDataGenerator::wite_all_selections()
{
	this->outPutStream << "break" << std::endl << curr_image->get_name(1).toStdString() << std::endl;
	
	for (std::vector<int>::iterator it = curr_image->class_type_vec.begin(); it != curr_image->class_type_vec.end(); ++it)
	{
		this->outPutStream << std::to_string(curr_image->point1_vec[*it].x) << " " << std::to_string(curr_image->point1_vec[*it].y)
			<< " " << std::to_string(curr_image->class_type_vec[*it]) << std::endl;
	}
}








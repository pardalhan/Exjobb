#include "qtdatagenerator.h"

QtDataGenerator::QtDataGenerator(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	setMouseTracking(true);
	/*RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	this->screen_w = desktop.right;
	this->screen_h = desktop.bottom;*/

}


QtDataGenerator::~QtDataGenerator()
{

}

void QtDataGenerator::on_btn_zoomout_clicked()
{
	ui.btn_zoomin->setChecked(false);
	this->zoom_point = cv::Point(0,0);
	this->draw_rect(curr_image->point1_vec, curr_image->point2_vec, curr_image->class_type_vec);
}
void QtDataGenerator::on_btn_clearall_clicked()
{
	this->zoom_point = cv::Point(0, 0);
	if (!curr_image->point1_vec.empty())
	{
		this->curr_image->point1_vec.clear();
		this->curr_image->point2_vec.clear();
		this->curr_image->class_type_vec.clear();
		this->altered_img = this->org_img.clone();
		cv::imshow("MyWindow", this->org_img);
		ui.lbl_img->clear();
	}
}
void QtDataGenerator::on_btn_clearlast_clicked()
{	
	this->zoom_point = cv::Point(0, 0);
	if (!curr_image->point1_vec.empty())
	{
		this->curr_image->point1_vec.erase(curr_image->point1_vec.end()-1);
		this->curr_image->point2_vec.erase(curr_image->point2_vec.end()-1);
		this->curr_image->class_type_vec.erase(curr_image->class_type_vec.end()-1);
		this->draw_rect(curr_image->point1_vec, curr_image->point2_vec, curr_image->class_type_vec);
	}
	else
	{
		cv::imshow("MyWindow", this->org_img);
	}
	ui.lbl_img->clear();

}

void QtDataGenerator::on_btn_next_clicked()
{
	// Write to file
	this->pre_writing();
	this->write_all_selections();
	this->outPutStream.close();

	delete curr_image; 

	this->file_counter++;
	if (file_counter < this->list_of_files.size()) // Display image there is a image in the filelist
	{
		this->curr_image = new MyImage;
		this->display_image(list_of_files[file_counter]);
		this->curr_image->set_name(QFileInfo(list_of_files[file_counter]).fileName(), 1);
		this->curr_image->set_name(QFileInfo(list_of_files[file_counter]).absoluteDir().absolutePath(), 2);

	}
	else
	{
		ui.btn_next->setEnabled(false);
		ui.btn_zoomin->setEnabled(false);
		ui.btn_zoomout->setEnabled(false);
		ui.btn_clearall->setEnabled(false);
		ui.btn_clearlast->setEnabled(false);
		ui.btn_browser->setEnabled(true);
		cv::destroyAllWindows();
		ui.lbl_img->clear();
	
	}	

	if ((file_counter + 1) >= this->list_of_files.size()) // Last file in the list
	{
		ui.btn_next->setText("DONE");

	}
}

void QtDataGenerator::on_btn_browser_clicked() // Get a list with all the files which should be examined
{
	this->file_counter = 0;
	ui.btn_next->setEnabled(true);

	QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", "\\home", "*.png *.jpg *.jpeg *.bmp *.gif");
	this->folder_path = QFileInfo(files[0]).absoluteDir().absolutePath();
	this->list_of_files = files;
	QString file_path = list_of_files[file_counter]; 
	display_image(file_path); // Display image from filepath 


	cv::setMouseCallback("MyWindow", mouseWrapper, this);
	
	this->drag = 0;

	if (list_of_files.size() == 1)
	{

		ui.btn_next->setText("DONE");
	}
	else
	{
		ui.btn_next->setText("NEXT IMAGE");
	}

	ui.btn_zoomin->setEnabled(true);
	ui.btn_zoomout->setEnabled(true);
	ui.btn_clearall->setEnabled(true);
	ui.btn_clearlast->setEnabled(true);

	curr_image = new MyImage; 
	this->curr_image->set_name(QFileInfo(files[file_counter]).fileName(), 1);
	this->curr_image->set_name(file_path, 2);
	ui.btn_browser->setEnabled(false);

}

void QtDataGenerator::display_image(QString filepath)
{
	if (QString::compare(filepath, QString()) != 0) // Check if the QString is empty 
	{
		QImage image;
		bool valid_image = image.load(filepath); // Make sure that a image is loaded

		if (valid_image)
		{
			this->org_img = cv::imread(filepath.toStdString().c_str()); // Convert QString to string for loading image
			this->altered_img = org_img.clone();
			cv::namedWindow("MyWindow", CV_WINDOW_KEEPRATIO); //create a window with the name "MyWindow"
			cv::resizeWindow("MyWindow", 800, 800);
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
	if (0 < x && x < this->org_img.size().width && 0 < y && y  < this->org_img.size().height) // Mouse has to inside the window
	{
		if (!ui.btn_zoomin->isChecked())
		{

			// LEFT BUTTOM ----------------------------------------------------------------------------
			if (event == CV_EVENT_LBUTTONDOWN && !drag)
			{
				// left button clicked. ROI selection begins 
				this->point1 = cv::Point(x, y);
				this->drag = 1;
			}
			if (event == CV_EVENT_MOUSEMOVE && drag)
			{
				// mouse dragged. ROI being selected 
				cv::Mat img1 = this->altered_img.clone();
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

				// Display selected region
				if (rect.width != 0 || rect.height != 0)
				{
					this->roi_img = altered_img(rect);
					cv::Mat temp; // make the same cv::Mat
					cvtColor(roi_img, temp, CV_BGR2RGB); // cvtColor Makes a copt, that what i need
					QImage dest((const uchar *)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
					ui.lbl_img->setPixmap(QPixmap::fromImage(dest).scaled(ui.lbl_img->width(), ui.lbl_img->height(), Qt::KeepAspectRatio));

					curr_image->insert_selection(this->point1 + this->zoom_point, this->point2 + this->zoom_point, 1); //Save selection
				}
			}
			if (event == CV_EVENT_LBUTTONUP)
			{
				// ROI selected 
				cv::rectangle(this->altered_img, this->rect, CV_RGB(0, 255, 0), 1, 8, 0);
				cv::imshow("MyWindow", this->altered_img);
			}
			// RIGHT BUTTOM -------------------------------------------------------------------------
			if (event == CV_EVENT_RBUTTONDOWN && !drag)
			{
				// left button clicked. ROI selection begins 
				this->point1 = cv::Point(x, y);
				this->drag = 1;
			}
			if (event == CV_EVENT_MOUSEMOVE && drag)
			{
				// mouse dragged. ROI being selected 
				cv::Mat img1 = this->altered_img.clone();
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

				// Display selected region
				if (rect.width != 0 || rect.height != 0)
				{
					this->roi_img = altered_img(rect);
					cv::Mat temp; // make the same cv::Mat
					cvtColor(roi_img, temp, CV_BGR2RGB); // cvtColor Makes a copt, that what i need
					QImage dest((const uchar *)temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
					ui.lbl_img->setPixmap(QPixmap::fromImage(dest).scaled(ui.lbl_img->width(), ui.lbl_img->height(), Qt::KeepAspectRatio));

					curr_image->insert_selection(this->point1 + this->zoom_point, this->point2 + this->zoom_point, 2); // Save selection
				}
			}
			if (event == CV_EVENT_RBUTTONUP)
			{
				// ROI selected 
				cv::rectangle(this->altered_img, rect, CV_RGB(0, 0, 255), 1, 8, 0);
				cv::imshow("MyWindow", this->altered_img);

			}
		}
		else // If zoom in is choosen  ---------------------------------------------------------------------
		{
			if (event == CV_EVENT_LBUTTONDOWN && !drag)
			{
				// left button clicked. ROI selection begins 
				this->point1 = cv::Point(x, y);
				this->drag = 1;
			}
			if (event == CV_EVENT_MOUSEMOVE && drag)
			{
				// mouse dragged. ROI being selected 
				cv::Mat img1 = this->altered_img.clone();
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


			}
			if (event == CV_EVENT_LBUTTONUP)
			{
				// Display selected region
				if (this->rect.width != 0 || this->rect.height != 0)
				{
					this->altered_img = this->altered_img(this->rect);
					cv::imshow("MyWindow", this->altered_img);
				}
				this->zoom_point = this->rect.tl() + this->zoom_point;
			}
		}
	}
	else
	{
		//ERROR
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
	std::string path_of_file = this->folder_path.toStdString() + "\\RecordOfLables.txt";
	if (this->if_file_exist(path_of_file))
	{
		if (!this->outPutStream.is_open())
		{
			outPutStream.open(path_of_file, std::ios::app | std::ios::out);
		}
	}
	else
	{
		this->outPutStream = std::ofstream(path_of_file);
	}
}

void QtDataGenerator::write_all_selections()
{
	this->outPutStream << "break" << std::endl << curr_image->get_name(2).toStdString() <<
		std::endl << curr_image->get_name(1).toStdString() << std::endl;
	
	for (int it = 0; it < curr_image->class_type_vec.size(); ++it)
	{
		this->outPutStream <<
			std::to_string(curr_image->point1_vec[it].x) << " " << std::to_string(curr_image->point1_vec[it].y)<< " " <<
			std::to_string(curr_image->point2_vec[it].x) << " " << std::to_string(curr_image->point2_vec[it].y)<< " " <<
			std::to_string(curr_image->class_type_vec[it]) << std::endl;
	}
}

void QtDataGenerator::draw_rect(std::vector<cv::Point> vec1, std::vector<cv::Point> vec2, std::vector<int> vec3)
{
	this->altered_img = this->org_img.clone();

		for (int it = 0; it < vec1.size(); it++)
		{
			cv::Rect temp = cv::Rect(vec1[it], vec2[it]);
			switch (vec3[it])
			{
			case 1:
				cv::rectangle(this->altered_img, temp, CV_RGB(0, 255, 0), 1, 8, 0);
				break;

			case 2:
				cv::rectangle(this->altered_img, temp, CV_RGB(0, 0, 255), 1, 8, 0);
				break;
			}
		}
	cv::imshow("MyWindow", this->altered_img);

}








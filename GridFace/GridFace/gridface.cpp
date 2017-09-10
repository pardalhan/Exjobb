#include "gridface.h"

GridFace::GridFace(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->curr_img = new MyImage();
	this->curr_img->init();
	ui.btn_next_run->setEnabled(false);
	ui.btn_train->setEnabled(true);
	ui.btn_validate->setEnabled(true);

}

GridFace::~GridFace()
{

}

void GridFace::on_btn_run_clicked()
{
	on_btn_train_clicked();

	ui.lbl_msg->setText("RUN");
	QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", "C:\\Users\\Stubborn\\Dropbox\\Exjobb\\ExjobbBilder", "*.png *.jpg *.jpeg *.bmp *.gif");
	this->dir_path = QFileInfo(files[0]).absoluteDir().absolutePath().toStdString().c_str();

	this->list_of_files = files;

	ui.btn_browser->setEnabled(false);
	ui.btn_next_run->setEnabled(true);
	ui.btn_train->setEnabled(true);
	ui.btn_validate->setEnabled(false);
	ui.lbl_browser->setText(QFileInfo(files[0]).absoluteDir().absolutePath().toStdString().c_str());
}

void GridFace::on_btn_next_run_clicked()
{
	cv::namedWindow("MainWindow", CV_WINDOW_KEEPRATIO);
	cv::resizeWindow("MainWindow", 800, 800);

	if (this->list_of_files.size() >= this->file_counter)
	{
	this->run_image();
	this->file_counter += 1; 
	cv::waitKey(0);
	}
	else
	{
		ui.btn_next_run->setText("DONE");
	}

}

void GridFace::on_btn_next_validate_clicked()
{
	cv::namedWindow("MainWindow", CV_WINDOW_KEEPRATIO);
	cv::resizeWindow("MainWindow", 800, 800);

	if (this->record_vec.size() >= this->file_counter)
	{
		this->validate_image();
		this->file_counter += 1;
		cv::waitKey(0);
	}
	else
	{
		ui.btn_next_validate->setText("DONE");
	}

}


void GridFace::on_btn_train_clicked()
{
	ui.lbl_msg->setText("BEEN TRAINED");

	cv::Mat data, label;
	cv::FileStorage myfile("color_data.yml", cv::FileStorage::READ);
	myfile["svm_data"] >> data;
	myfile["svm_label"] >> label;
	myfile.release();

	curr_img->image_svm = curr_img->train_SVM(data, label, 350, pow(10,-5));
}

void GridFace::on_btn_validate_clicked()
{
	ui.lbl_msg->setText("VALIDATE");

	on_btn_train_clicked();

	QStringList files = QFileDialog::getOpenFileNames(this, "Select record file (.txt)", "C:\\Users\\Stubborn\\Dropbox\\Exjobb\\ExjobbBilder", "*.txt");
	this->dir_path = QFileInfo(files[0]).absoluteDir().absolutePath().toStdString().c_str();

	//std::string file_path = this->dir_path + "\\short.txt";
	std::string file_path = QFileInfo(files[0]).absoluteFilePath().toStdString().c_str();;

	// Get true images
	this->record_vec = curr_img->extract_from_record(file_path);
	
	// Get predicted images
	this->processed_img_vec.reserve(record_vec.size());

	ui.btn_next_validate->setEnabled(true);
	ui.btn_validate->setEnabled(false);
	ui.lbl_browser->setText(QFileInfo(files[0]).absoluteDir().absolutePath().toStdString().c_str());


}


void GridFace::run_image()
{
	ui.lbl_result->setText("RUNNING");
	{
		int num = this->file_counter;
		this->curr_img->clear_data();
		QString file_path = this->list_of_files[num];
		if (QString::compare(file_path, QString()) != 0) // Check if the QString is empty 
		{
			this->curr_img->set_name(QFileInfo(this->list_of_files[num]).absoluteDir().absolutePath().toStdString().c_str(), 2);
			this->curr_img->set_name(QFileInfo(this->list_of_files[num]).fileName().toStdString().c_str(), 1);
		}
		else
		{
			// EROOR 
		}

		double t = (double)cvGetTickCount();
		//------------------------------------------- START PROCESSING ------------------------------------------------

		curr_img->process_image(*curr_img);

		//------------------------------------------- END PROCESSING ------------------------------------------------
		t = (double)cvGetTickCount() - t;
		int ms = cvRound(t / ((double)cvGetTickFrequency() * 1000.0));

		// Write results to file
		std::ofstream outPutStream;
		std::string path_of_file = this->dir_path + "\\OUTPUT.txt";
		std::ifstream infile(path_of_file.c_str());
		if (infile.good())
		{

			if (!outPutStream.is_open())
			{
				outPutStream.open(path_of_file, std::ios::app | std::ios::out);
			}
		}
		else
		{
			outPutStream = std::ofstream(path_of_file);
		}


		outPutStream << this->curr_img->get_name(1) << std::endl; 

		for (int tum = 0; tum <curr_img->frequency.size(); tum++)
		{
			std::string output_sting = "Area " + std::to_string(tum) + " -> " +
				"#: " + std::to_string(curr_img->frequency[tum]) + "  " +
				"Class 1: " + std::to_string(curr_img->predictions_1[tum]) + "  " +
				"Class 2: " + std::to_string(curr_img->predictions_2[tum]);
			
			outPutStream << output_sting << std::endl;
		}
		outPutStream << std::endl;
		outPutStream.close(); 


		ui.lbl_result->setText("Run time [ms]: " + QString::number(t/1000));

	}
}

void GridFace::validate_image()
{
	int kum = this->file_counter; 
	{
		this->curr_img->clear_data();
		this->curr_img->set_name(record_vec[kum].get_name(1), 1);
		this->curr_img->set_name(this->dir_path, 2);

		this->curr_img->validate_process_image(*curr_img, record_vec[kum]);

	}
}
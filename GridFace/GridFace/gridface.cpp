#include "gridface.h"

GridFace::GridFace(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->curr_img = new MyImage();
	this->curr_img->init();
	ui.btn_run->setEnabled(false);
	ui.btn_train->setEnabled(false);
	ui.btn_validate->setEnabled(false);

}

GridFace::~GridFace()
{

}

void GridFace::on_btn_browser_clicked()
{

	ui.btn_run->setEnabled(true);
	//this->list_of_files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", "C:\\Users\\Stubborn\\Dropbox\\Exjobb\\ExjobbBilder", "*.png *.jpg *.jpeg *.bmp *.gif");

	QString dir_path = QFileDialog::getExistingDirectory(this, tr("Select directory"), "C:\\Users\\Stubborn\\Desktop\\small", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	QDir dir(dir_path);
	QStringList name_filters;
	name_filters << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif";
	dir.setNameFilters(name_filters);
	
	ui.lbl_browser->setText(dir_path.toStdString().c_str());
	this->dir_path = dir_path.toStdString().c_str();
	this->list_of_files = dir.entryInfoList();

	/*
	if (list_of_files.size() == 1)
	{
		ui.btn_run->setText("DONE");
	}
	else
	{
		ui.btn_run->setText("NEXT IMAGE");
	}
	*/

	ui.btn_browser->setEnabled(false);
	ui.btn_run->setEnabled(true);
	ui.btn_train->setEnabled(true);
	ui.btn_validate->setEnabled(true);



	//this->on_btn_run_clicked();

}

void GridFace::on_btn_run_clicked()
{

	cv::namedWindow("MainWindow", CV_WINDOW_KEEPRATIO);
	cv::resizeWindow("MainWindow", 800, 800);

	for (int num = 0; num < this->list_of_files.size(); num++)
	{
		this->curr_img->clear_data();
		QString file_path = this->list_of_files[num].absoluteFilePath();
		if (QString::compare(file_path, QString()) != 0) // Check if the QString is empty 
		{
			this->curr_img->set_name(this->list_of_files[num].path().toStdString().c_str(), 2);
			this->curr_img->set_name(this->list_of_files[num].fileName().toStdString().c_str(), 1);
		}
		else
		{
			// EROOR 
		}

		double t = (double)cvGetTickCount();
		//------------------------------------------- START PROCESSING ------------------------------------------------

		curr_img->process_image(*curr_img);


		/*
		cv::imshow("MainWindow", seg_img);

		cv::namedWindow("Grid", CV_WINDOW_KEEPRATIO);
		cv::imshow("Grid", grid);

		std::string file_save = "";
		file_save += "C:\\Users\\Stubborn\\Desktop\\WORK\\Cut_imgaes\\";
		file_save += "mask_img_";
		file_save += QFileInfo(file_path).fileName().toStdString();

		cv::imwrite(file_save, segmentation.first);
		*/

		//------------------------------------------- END PROCESSING ------------------------------------------------
		t = (double)cvGetTickCount() - t;
		int ms = cvRound(t / ((double)cvGetTickFrequency() * 1000.0));

		ui.lbl_result->setText("Area 1: " + QString::number(curr_img->frequency[0]) + "\n" +
			"Area 2: " + QString::number(curr_img->frequency[1]) + "\n" +
			"Area 3: " + QString::number(curr_img->frequency[2]) + "\n" +
			"Area 4: " + QString::number(curr_img->frequency[3]) + "\n" +
			"Area 5: " + QString::number(curr_img->frequency[4]) + "\n" +
			"Area 6: " + QString::number(curr_img->frequency[5]) + "\n" +
			"Area 7: " + QString::number(curr_img->frequency[6]) + "\n" +
			"Area 8: " + QString::number(curr_img->frequency[7]) + "\n" +
			"Area 9: " + QString::number(curr_img->frequency[8]) + "\n" +
			"Area 10: " + QString::number(curr_img->frequency[9]) + "\n" +
			"Area 11: " + QString::number(curr_img->frequency[10]) + "\n" +
			"Area 12: " + QString::number(curr_img->frequency[11]) + "\n" +
			"Area 13: " + QString::number(curr_img->frequency[12]) + "\n" +
			"Area 14: " + QString::number(curr_img->frequency[13]) + "\n" +
			"Area 15: " + QString::number(curr_img->frequency[14]) + "\n" +
			"Area 16: " + QString::number(curr_img->frequency[15]) + "\n");


			/*
			if (curr_img->faces_vec.size() > 0)
			{
				ui.lbl_msg->setText("Faces detected: " + QString::number(curr_img->faces_vec.size()) + "\n" +
					"Detection of facial landmark on all faces took " + QString::number(ms) + "ms \n" +
					QString::number(middle_2.x - middle_1.x) + "\n" +
					QString::number(seg_img.size().width) + " " + QString::number(seg_img.size().height) + "\n");

			}
			else {
				// ERRROR;
			}
			*/
			
			this->processed_img_vec.push_back(*curr_img);

		}

		/*
		else
		{
			ui.btn_browser->setEnabled(true);
			ui.btn_run->setEnabled(false);
			cv::destroyAllWindows();

		}
		*/



}

void GridFace::on_btn_train_clicked()
{
	ui.lbl_msg->setText("train");

	std::string file_path = this->dir_path + "\\short.txt";

	this->record_vec = curr_img->extract_from_record(file_path);



	this->my_svm = curr_img->train_SVM(file_path);
	std::vector<int> labels_vec = curr_img->extract_lables(record_vec);
	cv::Mat training_data = curr_img->extract_features(record_vec, labels_vec.size());
	cv::Mat label_Mat(labels_vec);
	cv::Mat predictions;

	my_svm->predict(training_data, predictions);

	cv::Mat confusion_matrix = curr_img->calc_confusion(predictions, label_Mat);
}

void GridFace::on_btn_validate_clicked()
{
	ui.lbl_msg->setText("validate");
	std::string file_path = this->dir_path + "\\short.txt";

	// Get true images
	this->record_vec = curr_img->extract_from_record(file_path);

	// Get predicted images
	for (MyImage &num: record_vec)
	{
		this->curr_img->clear_data();
		this->curr_img->set_name(num.get_name(1), 1);
		this->curr_img->set_name(num.get_name(2), 2);
		this->curr_img->process_image(*curr_img);

		this->processed_img_vec.push_back(*curr_img);
	}

	// Transfer data from predition to true
	for (int kum = 0; kum < processed_img_vec.size(); kum++)
	{
		record_vec[kum].resize_factor = processed_img_vec[kum].resize_factor;
		record_vec[kum].rotate_angle = processed_img_vec[kum].rotate_angle;
		record_vec[kum].rezize_points(record_vec[kum]);
		record_vec[kum].img_size = processed_img_vec[kum].img_size;
		record_vec[kum].point1_vec = record_vec[kum].rotate_points(record_vec[kum].point1_vec);
		record_vec[kum].point2_vec = record_vec[kum].rotate_points(record_vec[kum].point2_vec);
	}

	// Display resluts
	for (int kum = 0; kum < processed_img_vec.size(); kum++)
	{

		cv::Mat cv_img = cv::imread(processed_img_vec[kum].get_name(2) + "\\" + processed_img_vec[kum].get_name(1)); 
		cv::resize(cv_img, cv_img, cv::Size(cv_img.size().width*processed_img_vec[kum].resize_factor, cv_img.size().height*processed_img_vec[kum].resize_factor), 0, 0, CV_INTER_CUBIC);
		cv_img = processed_img_vec[kum].rotate_img(cv_img);
		
		record_vec[kum].draw_record(cv_img);

		for (int num = 0; num < record_vec.size(); num++)
		{
			record_vec[num].intersecting_areas(record_vec[num], processed_img_vec[num]);
		}
		for (int num = 0; num < processed_img_vec[kum].candidates.size(); num++)
		{
			cv::rectangle(cv_img, processed_img_vec[kum].candidates[num], cv::Scalar(255, 255, 0), 2, 8, 0);
		}

		for (int rum : processed_img_vec[kum].index_checked_detections)
		{
			cv::rectangle(cv_img, processed_img_vec[kum].candidates[rum], cv::Scalar(255, 0, 0), 2, 8, 0);
		}

	
	}
}

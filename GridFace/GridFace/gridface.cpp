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

	QStringList files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", "C:\\Users\\Stubborn\\Dropbox\\Exjobb\\ExjobbBilder", "*.png *.jpg *.jpeg *.bmp *.gif");
	this->dir_path = QFileInfo(files[0]).absoluteDir().absolutePath().toStdString().c_str();
	this->list_of_files = files;

	ui.btn_browser->setEnabled(false);
	ui.btn_run->setEnabled(true);
	ui.btn_train->setEnabled(true);
	ui.btn_validate->setEnabled(true);

}

void GridFace::on_btn_run_clicked()
{

	cv::namedWindow("MainWindow", CV_WINDOW_KEEPRATIO);
	cv::resizeWindow("MainWindow", 800, 800);

	this->processed_img_vec.reserve(this->list_of_files.size());

	for (int num = 0; num < this->list_of_files.size(); num++)
	{
		this->curr_img->clear_data();
		//QString file_path = this->list_of_files[num].absoluteFilePath();
		QString file_path = this->list_of_files[num];
		if (QString::compare(file_path, QString()) != 0) // Check if the QString is empty 
		{
			//this->curr_img->set_name(this->list_of_files[num].path().toStdString().c_str(), 2);
			//this->curr_img->set_name(this->list_of_files[num].fileName().toStdString().c_str(), 1);
			
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
			
			this->processed_img_vec.push_back(*curr_img);
		}

	// Display resluts
	for (int kum = 0; kum < 1; kum++)
	{

		cv::Mat cv_img = cv::imread(processed_img_vec[kum].get_name(2) + "\\" + processed_img_vec[kum].get_name(1));
		cv::resize(cv_img, cv_img, cv::Size(cv_img.size().width*processed_img_vec[kum].resize_factor, cv_img.size().height*processed_img_vec[kum].resize_factor), 0, 0, CV_INTER_CUBIC);
		cv_img = processed_img_vec[kum].rotate_img(cv_img);

		processed_img_vec[kum].draw_grid(cv_img);

		for (int num = 0; num < processed_img_vec[kum].candidates.size(); num++)
		{
			cv::rectangle(cv_img, processed_img_vec[kum].candidates[num], cv::Scalar(0, 0, 255), 2, 8, 0);
		}

		cv::imshow("MainWindow", cv_img);
		cv::waitKey(0);
	}
}

void GridFace::on_btn_train_clicked()
{
	ui.lbl_msg->setText("train");

	std::string file_path = this->dir_path + "\\RecordOfLables5.txt";

	this->record_vec = curr_img->extract_from_record(file_path);
	std::vector<int> labels_vec = curr_img->extract_lables(record_vec);
	cv::Mat data = curr_img->extract_features(record_vec, labels_vec.size());
	cv::Mat labels(labels_vec);
	
	cv::Mat test_data, training_data, test_label, traing_label;
	test_data = data; 
	training_data = data;
	test_label = labels;
	traing_label = labels;
	this->my_svm = curr_img->train_SVM(training_data, traing_label);
	
	cv::Mat predictions;
	my_svm->predict(test_data, predictions);

	cv::Mat confusion_matrix = curr_img->calc_confusion(predictions, test_label);


}

void GridFace::on_btn_validate_clicked()
{
	ui.lbl_msg->setText("validate");

	std::string file_path = this->dir_path + "\\short.txt";

	// Get true images
	this->record_vec = curr_img->extract_from_record(file_path);
	
	// Get predicted images
	this->processed_img_vec.reserve(record_vec.size());

	cv::Mat detected_mat; 
	cv::Mat hits_mat;

	for (int kum = 0; kum < record_vec.size(); kum++)
	{
		this->curr_img->clear_data();
		this->curr_img->set_name(record_vec[kum].get_name(1), 1);
		this->curr_img->set_name(this->dir_path, 2);

		this->curr_img->process_image(*curr_img);
		this->processed_img_vec.push_back(*curr_img);
		// Transfer data from predition to true
		record_vec[kum].resize_factor = curr_img->resize_factor;
		record_vec[kum].rotate_angle = curr_img->rotate_angle;
		record_vec[kum].rezize_points(record_vec[kum]);
		record_vec[kum].img_size = curr_img->img_size;
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
			cv::rectangle(cv_img, processed_img_vec[kum].candidates[num], cv::Scalar(0, 255, 0), 2, 8, 0);
		}

		for (int rum : processed_img_vec[kum].index_checked_detections)
		{
			cv::rectangle(cv_img, processed_img_vec[kum].candidates[rum], cv::Scalar(255, 0, 0), 2, 8, 0);
		}
	}
}

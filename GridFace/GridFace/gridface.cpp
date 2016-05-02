#include "gridface.h"

GridFace::GridFace(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->curr_img = new MyImage();
	this->curr_img->init();
	ui.btn_run->setEnabled(false);

}

GridFace::~GridFace()
{

}

void GridFace::on_btn_browser_clicked()
{

	this->file_counter = 0;
	ui.btn_run->setEnabled(true);
	this->list_of_files = QFileDialog::getOpenFileNames(this, "Select one or more files to open", "D:\\Testdata\\ref_bilder", "*.png *.jpg *.jpeg *.bmp *.gif");

	if (list_of_files.size() == 1)
	{
		ui.btn_run->setText("DONE");
	}
	else
	{
		ui.btn_run->setText("NEXT IMAGE");
	}

	ui.btn_browser->setEnabled(false);
	ui.btn_run->setEnabled(true);

	cv::namedWindow("MainWindow", CV_WINDOW_KEEPRATIO);
	cv::resizeWindow("MainWindow", 800, 800);

	this->my_switch = true;
	this->on_btn_run_clicked();

}

void GridFace::on_btn_run_clicked()
{
	while (my_switch)
	{
		this->curr_img->clear_data();
		if (file_counter < this->list_of_files.size()) // Display image there is a image in the filelist
		{
			QString file_path = this->list_of_files[this->file_counter];
			cv::Mat cv_img;
			if (QString::compare(file_path, QString()) != 0) // Check if the QString is empty 
			{

				this->curr_img->set_name(QFileInfo(file_path).absoluteDir().absolutePath(), 2);
				this->curr_img->set_name(QFileInfo(file_path).fileName(), 1);


				cv_img = cv::imread(file_path.toStdString().c_str()); // load input image
			}
			else
			{
				// EROOR 
			}

			double t = (double)cvGetTickCount();
			//------------------------------------------- START PROCESSING ------------------------------------------------

			// Light normalization
			cv::Mat img_light = badger_process(cv_img);
			//cv::Mat img_light = MSR_process(cv_img);

			// Rotation
			this->curr_img->detect_faces(img_light);
			this->curr_img->detect_landmarks(img_light);
			cv::Mat rotated_img = this->curr_img->rotate_img(img_light);

			// Resize factor 
			cv::Point middle_1 = curr_img->landmark_vec[36] + (curr_img->landmark_vec[39] - curr_img->landmark_vec[36]) / 2;
			cv::Point middle_2 = curr_img->landmark_vec[42] + (curr_img->landmark_vec[45] - curr_img->landmark_vec[42]) / 2;
			double resize_factor = (double)500 / (double)(middle_2.x - middle_1.x);

			// Cut out face
			//cv::Rect face_rect = this->curr_img->enlarg_face_rect(this->curr_img->faces_vec[0], 0.4, rotated_img.size());
			//cv::Mat cut_out_face = rotated_img(face_rect);

			this->curr_img->clear_data();

			// Resize image 

			cv::Mat img_resize;
			cv::resize(rotated_img, img_resize,
				cv::Size(rotated_img.size().width*resize_factor, rotated_img.size().height*resize_factor), 0, 0, CV_INTER_CUBIC);
			this->curr_img->detect_faces(img_resize);
			this->curr_img->detect_landmarks(img_resize);

			std::pair<cv::Mat, cv::Mat> segmentation = this->curr_img->face_segment(img_resize, curr_img->landmark_vec, false);

			// Image normalized 
			cv::Mat grid = img_resize.clone();
			cv::Mat seg_img = segmentation.second.clone();

			// Display
			middle_1 = curr_img->landmark_vec[36] + (curr_img->landmark_vec[39] - curr_img->landmark_vec[36]) / 2;
			middle_2 = curr_img->landmark_vec[42] + (curr_img->landmark_vec[45] - curr_img->landmark_vec[42]) / 2;
			cv::line(grid, middle_1, middle_2, CV_RGB(0, 255, 0), 2);
			this->curr_img->display_landmarks(grid);
			this->curr_img->draw_grid(grid);

			cv::imshow("MainWindow", seg_img);

			cv::namedWindow("Grid", CV_WINDOW_KEEPRATIO);
			cv::imshow("Grid", grid);

			std::string file_save = "";
			file_save += "C:\\Users\\Stubborn\\Desktop\\Cut_imgaes\\";
			file_save += "seg_img_";
			file_save += QFileInfo(file_path).fileName().toStdString();

			cv::imwrite(file_save, seg_img);


			//------------------------------------------- END PROCESSING ------------------------------------------------
			t = (double)cvGetTickCount() - t;
			int ms = cvRound(t / ((double)cvGetTickFrequency() * 1000.0));

			if (curr_img->faces_vec.size() > 0)
			{
				ui.lbl_msg->setText("Faces detected: " + QString::number(curr_img->faces_vec.size()) + "\n" +
					"Detection of facial landmark on all faces took " + QString::number(ms) + "ms \n" +
					QString::number(middle_2.x - middle_1.x) + "\n" +
					QString::number(seg_img.size().width) + " " + QString::number(seg_img.size().height) + "\n");

			}
			else {
				printf("NO Face");
			}

		}
		else
		{
			ui.btn_browser->setEnabled(true);
			ui.btn_run->setEnabled(false);
			cv::destroyAllWindows();
			this->my_switch = false;
		}

		if ((file_counter + 1) >= this->list_of_files.size()) // Last file in the list
		{
			ui.btn_run->setText("DONE");
		}

		this->file_counter++;
		cv::waitKey(0);

	}
}

void GridFace::on_btn_second_clicked()
{

}


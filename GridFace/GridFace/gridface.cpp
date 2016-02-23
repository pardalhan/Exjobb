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

	//display_image(file_path); // Display image from filepath 


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
	//while (my_switch)
	{

	if (file_counter < this->list_of_files.size()) // Display image there is a image in the filelist
	{
		QString file_path = this->list_of_files[this->file_counter];
		cv::Mat img;
		if (QString::compare(file_path, QString()) != 0) // Check if the QString is empty 
		{

			this->curr_img->set_name(QFileInfo(file_path).absoluteDir().absolutePath(), 2);
			this->curr_img->set_name(QFileInfo(file_path).fileName(), 1);


			img = cv::imread(file_path.toStdString().c_str()); // load input image
		}
		else
		{
			//ERROR
		}
		file_path;
		cv::Mat img_grayscale;
		cv::cvtColor(img, img_grayscale, CV_BGR2GRAY);	// convert image to grayscale
		IplImage* img_grayscale_ipl = cvCloneImage(&(IplImage)img_grayscale); // detectFaceInImage need IplImage* as input 


		double t = (double)cvGetTickCount();

		curr_img->detect_landmarks(img, img_grayscale_ipl, curr_img->faceCascade, curr_img->model_fland);

		t = (double)cvGetTickCount() - t;
		int ms = cvRound(t / ((double)cvGetTickFrequency() * 1000.0));

		if (curr_img->faces.size() > 0)
		{
			ui.lbl_msg->setText("Faces detected: " + QString::number(curr_img->faces.size()) + "\n" +
				"Detection of facial landmark on all faces took " + QString::number(ms) + "ms \n");

		}
		else {
			printf("NO Face");
		}

		cv::imshow("MainWindow", img);

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
	cv::waitKey(2 * 1000);

	}
}

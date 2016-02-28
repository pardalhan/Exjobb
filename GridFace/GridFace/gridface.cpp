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

void GridFace::on_btn_second_clicked()
{
	try
	{
		// We need a face detector.  We will use this to get bounding boxes for
		// each face in an image.
		dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
		// And we also need a shape_predictor.  This is the tool that will predict face
		// landmark positions given an image and face bounding box.  Here we are just
		// loading the model from the shape_predictor_68_face_landmarks.dat file you gave
		// as a command line argument.
		dlib::shape_predictor sp;
		dlib::deserialize("C:\\Users\\arman\\Desktop\\shape_predictor_68_face_landmarks.dat") >> sp;


		dlib::image_window win, win_faces;

		dlib::array2d<dlib::rgb_pixel> img;
		//load_image(img, "C:\\Users\\arman\\Desktop\\Cropped_Face.bmp");
		//load_image(img, "C:\\Users\\arman\\Desktop\\010_frontal.JPG");
		dlib::load_image(img, "C:\\Users\\arman\\Desktop\\lena.png");

		// Make the image larger so we can detect small faces.
		dlib::pyramid_up(img);

		// Now tell the face detector to give us a list of bounding boxes
		// around all the faces in the image.
		std::vector<dlib::rectangle> dets = detector(img);

		// Now we will go ask the shape_predictor to tell us the pose of
		// each face we detected.
		std::vector<dlib::full_object_detection> shapes;
		for (unsigned long j = 0; j < dets.size(); ++j)
		{
			dlib::full_object_detection shape = sp(img, dets[j]);
			std::cout << "number of parts: " << shape.num_parts() << std::endl;
			std::cout << "pixel position of first part:  " << shape.part(0) << std::endl;
			std::cout << "pixel position of second part: " << shape.part(1) << std::endl;
			// You get the idea, you can get all the face part locations if
			// you want them.  Here we just store them in shapes so we can
			// put them on the screen.
			shapes.push_back(shape);
		}

		// Now let's view our face poses on the screen.
		win.clear_overlay();
		win.set_image(img);
		win.add_overlay(render_face_detections(shapes));

		// We can also extract copies of each face that are cropped, rotated upright,
		// and scaled to a standard size as shown here:
		dlib::array<dlib::array2d<dlib::rgb_pixel> > face_chips;
		extract_image_chips(img, get_face_chip_details(shapes), face_chips);
		win_faces.set_image(tile_images(face_chips));

		std::cout << "Hit enter to process the next image..." << std::endl;
		std::cin.get();

	}
	catch (std::exception& e)
	{
		std::cout << "\nexception thrown!" << std::endl;
		std::cout << e.what() << std::endl;
	}
}

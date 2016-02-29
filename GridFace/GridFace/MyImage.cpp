#include "MyImage.h"


MyImage::MyImage()
{
}


MyImage::~MyImage()
{
}

void MyImage::init()
{
	// Haar Cascade file, used for Face Detection.
	char faceCascadeFilename[] = "SVM_landmarks\\haarcascade_frontalface_alt.xml";
	// Load the HaarCascade classifier for face detection.
	if (!faceCascade.load(faceCascadeFilename))
	{
		// ERROR
		exit(1);
	}

	model_fland = flandmark_init("SVM_landmarks\\flandmark_model.dat"); // load flandmark model structure and initialize

	dlib::deserialize("dlib\\shape_predictor_68_face_landmarks.dat") >> model_dlib;
}

QString MyImage::get_name(int option)
{
	switch (option)
	{
	case 1: 
		return file_name;
		break;
	case 2:
		return file_path;
		break;
	default: 
		return QString("Error");
		// EROOR 
		break; 

	}

}

void MyImage::set_name(QString new_name, int option)
{
	switch (option)
	{
	case 1:
		file_name = new_name;
		break;
	case 2:
		file_path = new_name; 
		break;
	default:
		
		// EROOR 
		break;

	}
}

void MyImage::insert_selection(cv::Point point1, cv::Point point2, int class_type)
{
	point1_vec.push_back(point1);
	point2_vec.push_back(point2);
	class_type_vec.push_back(class_type);

}

void MyImage::detect_landmarks(cv::Mat orig, cv::CascadeClassifier cascade, FLANDMARK_Model *model)
{

	// Gray image 
	cv::Mat img_grayscale;
	cv::cvtColor(orig, img_grayscale, CV_BGR2GRAY);	// convert image to grayscale
	IplImage* img_grayscale_ipl = cvCloneImage(&(IplImage)img_grayscale); // detectFaceInImage need IplImage* as input 

	double *landmarks = (double*)malloc(2 * model->data.options.M*sizeof(double)); // Landmarks points 
	int *bbox = (int*)malloc(4 * sizeof(int)); // Bonding box

	// Face detector parameters
	CvSize minFeatureSize = cvSize(50, 50);	 // Smallest face size.
	int flags = CV_HAAR_DO_CANNY_PRUNING;
	float search_scale_factor = 1.1f;	// How detailed should the search be.

	// Detect faces
	cascade.detectMultiScale(orig, faces_vec, search_scale_factor, 2, 0 | flags, minFeatureSize);

	//this->landmarks_SVM(img_grayscale_ipl, bbox, model, landmarks);
	this->landmarks_dlib(orig);

	// Display
	this->display_landmarks(orig);
	cv::imshow("MainWindow", orig);

}




//-------------------------------------- PRIVATE FUNCTIONS ----------------------------------------------
void MyImage::landmarks_SVM(IplImage *grey_img, int *bbox, FLANDMARK_Model *model, double *landmarks)
{
	for (int iface = 0; iface < faces_vec.size(); ++iface)
	{
		// Insert bonding box
		bbox[0] = faces_vec[iface].x;
		bbox[1] = faces_vec[iface].y;
		bbox[2] = faces_vec[iface].x + faces_vec[iface].width;
		bbox[3] = faces_vec[iface].y + faces_vec[iface].height;

		// Detect landmarks
		flandmark_detect(grey_img, bbox, model, landmarks);
		
		for (int i = 0; i < 2 * model->data.options.M; i += 2)
		{
			this->landmark_vec.push_back(cv::Point(int(landmarks[i]), int(landmarks[i + 1])));
		}
	}
}


void MyImage::landmarks_dlib(cv::Mat orig_img)
{

	try
	{
		//Convert from mat to dlib
		dlib::array2d<dlib::rgb_pixel> img_dlib;
		dlib::assign_image(img_dlib, dlib::cv_image<dlib::bgr_pixel>(orig_img));

		// Detector for faces  VERY SLOW BUT BETTER THE OPENCV
		//dlib::frontal_face_detector detector = dlib::get_frontal_face_detector();
		// Make the image larger so we can detect small faces.
		//dlib::pyramid_up(img);
		// Vector with faces (bounding boxes)
		//std::vector<dlib::rectangle> dets = detector(img_dlib);
		//this->faces_vec.clear();
		//for (int i = 0; i < dets.size(); i++)
		//{
		//	this->faces_vec.push_back(cv::Rect(cv::Point2i(dets[i].left(), dets[i].top()), cv::Point2i(dets[i].right() + 1, dets[i].bottom() + 1)));
		//}
	

		// Vector with landmarks 
		for (unsigned long i = 0; i < this->faces_vec.size(); ++i)
		{
			dlib::rectangle temp_face((long)faces_vec[i].tl().x, (long)faces_vec[i].tl().y, (long)faces_vec[i].br().x - 1, (long)faces_vec[i].br().y - 1);
			dlib::full_object_detection shape = this->model_dlib(img_dlib, temp_face); // Find landmark in bounding box
			for (int i = 0; i < shape.num_parts(); i++)
			{
				this->landmark_vec.push_back(cv::Point(shape.part(i).x(), shape.part(i).y()));
			}
		}
	
	
	}
	catch (std::exception& e)
	{
		// ERROR
	}
	//}
}

void MyImage::display_landmarks(cv::Mat orig_img)
{
	//cv::rectangle(orig_img, cvPoint(model->bb[0], model->bb[1]), cvPoint(model->bb[2], model->bb[3]), CV_RGB(0, 255, 0), 2);

	for (int i = 0; i < this->landmark_vec.size(); i++)
	{
		cv::circle(orig_img, this->landmark_vec[i], 2, CV_RGB(0, 0, 255), CV_FILLED);
		// Display number 
		//cv::putText(orig_img, std::to_string(i), this->landmark_vec[i] + cv::Point(5,5),		
		//	CV_FONT_HERSHEY_SIMPLEX, 1, CV_RGB(0, 0, 255), 2); 

	}
	for (int i = 0; i < this->faces_vec.size(); i++)
	{
		cv::rectangle(orig_img, this->faces_vec[0], CV_RGB(0, 0, 255), 2);
	}
	
}

void MyImage::clear_data()
{
point1_vec.clear();
point2_vec.clear();
class_type_vec.clear();
faces_vec.clear();
landmark_vec.clear();
file_path.clear(); 
file_name.clear();
}
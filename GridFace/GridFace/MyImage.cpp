#include "MyImage.h"


MyImage::MyImage()
{
}


MyImage::~MyImage()
{
}

void MyImage::init()
{

	// Load the HaarCascade classifier for face detection.
	if (!face_Cascade.load("opencv\\haarcascade_frontalface_alt.xml"))
	{
		// ERROR
		this->display_message("Could not load -haarcascade_frontalface_alt.xml. Exiting!");
		exit(1);
	}

	
	
	//model_fland = flandmark_init("SVM_landmarks\\flandmark_model.dat"); // load flandmark model structure and initialize

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

void MyImage::detect_faces(cv::Mat orig)
{
	// Face detector parameters
	CvSize minFeatureSize = cvSize(200, 200);	 // Smallest face size.
	int flags = CV_HAAR_DO_CANNY_PRUNING;
	float search_scale_factor = 1.1f;	// How detailed should the search be.

	// Detect faces - OPENCV
	face_Cascade.detectMultiScale(orig, this->faces_vec, search_scale_factor, 2, 0 | flags, minFeatureSize);
	
	if (this->faces_vec.size() == 0)
	{
		this->display_message("No face detected! Exiting!");
	}
}



//void MyImage::landmarks_SVM(IplImage *grey_img, int *bbox, FLANDMARK_Model *model, double *landmarks)
//{
//	for (int iface = 0; iface < faces_vec.size(); ++iface)
//	{
//		// Insert bonding box
//		bbox[0] = faces_vec[iface].x;
//		bbox[1] = faces_vec[iface].y;
//		bbox[2] = faces_vec[iface].x + faces_vec[iface].width;
//		bbox[3] = faces_vec[iface].y + faces_vec[iface].height;
//
//		// Detect landmarks
//		flandmark_detect(grey_img, bbox, model, landmarks);
//		
//		for (int i = 0; i < 2 * model->data.options.M; i += 2)
//		{
//			this->landmark_vec.push_back(cv::Point(int(landmarks[i]), int(landmarks[i + 1])));
//		}
//	}
//}


void MyImage::detect_landmarks(cv::Mat orig_img)
{

	try
	{
		//Convert from mat to dlib
		dlib::array2d<dlib::rgb_pixel> img_dlib;
		dlib::assign_image(img_dlib, dlib::cv_image<dlib::bgr_pixel>(orig_img));

		// Vector with landmarks 

			dlib::rectangle temp_face((long)faces_vec[0].tl().x, (long)faces_vec[0].tl().y, (long)faces_vec[0].br().x - 1, (long)faces_vec[0].br().y - 1);
			dlib::full_object_detection shape = this->model_dlib(img_dlib, temp_face); // Find landmark in bounding box
			for (int i = 0; i < shape.num_parts(); i++)
			{
				this->landmark_vec.push_back(cv::Point(shape.part(i).x(), shape.part(i).y()));
			}
	
	
	}
	catch (std::exception& e)
	{
		this->display_message("Error in landmark_dlib");
		// ERROR
	}

}

void MyImage::display_landmarks(cv::Mat orig_img)
{
	for (int i = 0; i < this->landmark_vec.size(); i++)
	{
		cv::circle(orig_img, this->landmark_vec[i], 2, CV_RGB(0, 0, 255), CV_FILLED);
		// Display number 
		//cv::putText(orig_img, std::to_string(i), this->landmark_vec[i] + cv::Point(5,5),		
		//	CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0, 0, 255), 2); 

	}
	for (int i = 0; i < this->faces_vec.size(); i++)
	{
		cv::rectangle(orig_img, this->faces_vec[0], CV_RGB(0, 0, 255), 2);
	}
	
}

void MyImage::draw_grid(cv::Mat img)
{
	std::vector<cv::Point> vec = this->landmark_vec;
	int height = img.size().height;
	int width = img.size().width;
	
	// Horozontal lines
	cv::line(img, cv::Point(0, vec[36].y), vec[36], CV_RGB(255, 0, 0), 2);
	cv::line(img, vec[36], vec[45], CV_RGB(255, 0, 0), 2);
	cv::line(img, vec[45], cv::Point(width, vec[45].y), CV_RGB(255, 0, 0), 2);

	cv::line(img, cv::Point(0, vec[48].y), vec[48], CV_RGB(255, 0, 0), 2);
	cv::line(img, vec[48], vec[54], CV_RGB(255, 0, 0), 2);
	cv::line(img, vec[54], cv::Point(width, vec[54].y), CV_RGB(255, 0, 0), 2);

	cv::line(img, cv::Point(0, vec[8].y), vec[8], CV_RGB(255, 0, 0), 2);
	cv::line(img, vec[8], cv::Point(width, vec[8].y), CV_RGB(255, 0, 0), 2);


	// Vertical lines
	cv::line(img, cv::Point(vec[36].x, 0), vec[36], CV_RGB(255, 0, 0), 2);
	cv::line(img, vec[36], cv::Point(vec[36].x, height), CV_RGB(255, 0, 0), 2);

	cv::line(img, cv::Point(vec[27].x, 0), vec[27], CV_RGB(255, 0, 0), 2);
	cv::line(img, vec[27], vec[30], CV_RGB(255, 0, 0), 2);
	cv::line(img, vec[30], cv::Point(vec[30].x, height), CV_RGB(255, 0, 0), 2);

	cv::line(img, cv::Point(vec[45].x, 0), vec[45], CV_RGB(255, 0, 0), 2);
	cv::line(img, vec[45], cv::Point(vec[45].x, height), CV_RGB(255, 0, 0), 2);
}

cv::Mat MyImage::rotate_img(cv::Mat orig_img)
{
	cv::Mat rotated_img;

	cv::Point left_eye_1 = landmark_vec[36];
	cv::Point left_eye_2 = landmark_vec[39];
	cv::Point right_eye_1 = landmark_vec[42];
	cv::Point right_eye_2 = landmark_vec[45];
	cv::Point straight_point_1(left_eye_1.x + right_eye_1.x, left_eye_1.y);
	cv::Point straight_point_2(left_eye_2.x + right_eye_2.x, left_eye_2.y);

	double angle_1 = this->calc_angle(left_eye_1, straight_point_1, right_eye_1);
	double angle_2 = this->calc_angle(left_eye_2, straight_point_2, right_eye_2);
	double angle = (angle_1 + angle_2) / 2;

	cv::Point2f pt(orig_img.cols / 2., orig_img.rows / 2.);
	cv::Mat r = getRotationMatrix2D(pt, -angle , 1.0);
	warpAffine(orig_img, rotated_img, r, cv::Size(orig_img.cols, orig_img.rows));


	return rotated_img;
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

void MyImage::display_message(QString msg)
{
	this->message.setText(msg);
	message.exec();
	exit(1);
}

double MyImage::calc_angle(cv::Point a, cv::Point b, cv::Point c)
{

	cv::Point diff_ab = b - a;
	cv::Point diff_ac = c - a;
	double dist_ab = sqrt(std::pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
	double dist_ac = sqrt(std::pow((c.x - a.x), 2) + pow((c.y - a.y), 2));
	double dotprod = diff_ab.dot(diff_ac);
	double angle = acos(dotprod / (dist_ab*dist_ac));
	angle = angle * (double)180 / M_PI;
	if (c.y > b.y)
	{
		angle = -angle;
	}
	return angle;
}

cv::Rect MyImage::enlarg_face_rect(cv::Rect face, double procent, cv::Size img_size)
{

	cv::Rect larg_face;
	double img_height_diff = face.height*(1 + procent) - face.height;
	double img_width_diff = face.width*(1 + procent) - face.width;

	larg_face.x = (int)round(face.x - img_width_diff / 2);
	larg_face.y = (int)round(face.y - img_height_diff / 2);
	larg_face.height = (int)round(face.height*(1 + procent));
	larg_face.width = (int)round(face.width*(1 + procent));

	if (larg_face.x < 0)
	{
		larg_face.x = 0;
	}
	if (larg_face.y < 0)
	{
		larg_face.y = 0;
	}
	if (larg_face.height > img_size.height)
	{
		larg_face.height = img_size.height;
	}
	if (larg_face.width > img_size.width)
	{
		larg_face.width = img_size.width;
	}

	return larg_face;
}

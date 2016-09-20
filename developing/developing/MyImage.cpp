#include "MyImage.h"


MyImage::MyImage()
{
}


MyImage::~MyImage()
{
}
std::string MyImage::get_name(int option)
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
		return std::string("Error");
		// EROOR 
		break;

	}

}
void MyImage::set_name(std::string new_name, int option)
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

void MyImage::init()
{

	// Load the HaarCascade classifier for face detection.
	if (!face_Cascade.load("opencv\\haarcascade_frontalface_alt.xml"))
	{
		// ERROR
		exit(1);
	}

	
	
	//model_fland = flandmark_init("SVM_landmarks\\flandmark_model.dat"); // load flandmark model structure and initialize

	dlib::deserialize("dlib\\shape_predictor_68_face_landmarks.dat") >> model_dlib;
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

	cv::rectangle(orig_img, this->faces_vec[0], CV_RGB(0, 0, 255), 2);

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

std::pair<cv::Mat, cv::Mat> MyImage::face_segment(cv::Mat cv_img, std::vector<cv::Point> landmark_vec, bool down_sample)
{

	int down_iter = 0;
	cv::Mat bgModel, fgModel, down_img, end_mask;
	cv::Mat foreground = cv::Mat(cv_img.size(), CV_8UC1, cv::Scalar(255, 255, 255));
	cv::Mat mask = cv::Mat::ones(cv_img.size(), CV_8U) * cv::GC_PR_BGD;

	std::vector<cv::Point> lower_face_roi;
	std::vector<cv::Point> eye_1_vec;
	std::vector<cv::Point> eye_2_vec;
	std::vector<cv::Point> nose_vec;
	std::vector<cv::Point> lower_face_vec;
	std::vector<cv::Point> mouth_vec;
	std::vector<cv::Point> nostrils_vec;

	nose_vec.push_back(landmark_vec[21]);
	nose_vec.push_back(landmark_vec[22]);
	nose_vec.push_back(landmark_vec[42]);
	nose_vec.push_back(landmark_vec[35]);
	nose_vec.push_back(landmark_vec[33]);
	nose_vec.push_back(landmark_vec[31]);
	nose_vec.push_back(landmark_vec[39]);
	for (int i = 36; i <= 41; i++)
	{
		eye_1_vec.push_back(landmark_vec[i]);
	}
	for (int i = 42; i <= 47; i++)
	{
		eye_2_vec.push_back(landmark_vec[i]);
	}
	for (int i = 17; i <= 26; i++)
	{
		lower_face_vec.push_back(landmark_vec[i]);
	}
	for (int i = 16; i >= 0; i--)
	{
		lower_face_vec.push_back(landmark_vec[i]);
	}
	for (int i = 48; i <= 64; i++)
	{
		mouth_vec.push_back(landmark_vec[i]);
	}
	for (int i = 30; i <= 35; i++)
	{
		nostrils_vec.push_back(landmark_vec[i]);
	}

	int head_width = landmark_vec[24].x - landmark_vec[19].x;
	cv::Rect forehead(landmark_vec[19].x, landmark_vec[19].y - (int)round(head_width / 2), head_width, (int)round(head_width / 3));

	int mouth_chin = landmark_vec[8].y - landmark_vec[57].y;
	cv::Rect throat(landmark_vec[7].x, landmark_vec[7].y,
		landmark_vec[9].x - landmark_vec[7].x, mouth_chin);

	cv::RotatedRect eye_rec_1 = cv::fitEllipse(eye_1_vec);
	cv::RotatedRect eye_rec_2 = cv::fitEllipse(eye_2_vec);
	cv::RotatedRect mouth_rec = cv::fitEllipse(mouth_vec);
	cv::RotatedRect nostril_rec = cv::fitEllipse(nostrils_vec);
	cv::approxPolyDP(lower_face_vec, lower_face_roi, 1.0, true);

	cv::fillConvexPoly(mask, &lower_face_roi[0], lower_face_roi.size(), cv::Scalar(cv::GC_PR_FGD), 8, 0);
	rectangle(mask, forehead, cv::Scalar(cv::GC_PR_FGD), -1, 8, 0);
	rectangle(mask, throat, cv::Scalar(cv::GC_PR_FGD), -1, 8, 0);
	//cv::ellipse(mask, eye_rec_1, cv::Scalar(cv::GC_BGD), -1);
	//cv::ellipse(mask, eye_rec_2, cv::Scalar(cv::GC_BGD), -1);
	//cv::ellipse(mask, mouth_rec, cv::Scalar(cv::GC_BGD), -1);
	

	/*
	// BACK GOUND
	int thickness = 200;
	cv::Rect backg_1(0, 0, cv_img.size().width, thickness);
	cv::Rect backg_2(0, 0, thickness, cv_img.size().height);
	cv::Rect backg_3(cv_img.size().width - thickness, 0, thickness, cv_img.size().height);
	cv::Rect backg_4(0, cv_img.size().height - thickness, cv_img.size().width, 10);
	rectangle(mask, backg_1, cv::Scalar(cv::GC_BGD), -1);
	rectangle(mask, backg_2, cv::Scalar(cv::GC_BGD), -1);
	rectangle(mask, backg_3, cv::Scalar(cv::GC_BGD), -1);
	rectangle(mask, backg_4, cv::Scalar(cv::GC_BGD), -1);


	--------------------------------------------------------------------------
	// FIRST SETT OF FOREGROUND

	int head_width = landmark_vec[24].x - landmark_vec[19].x;
	cv::Rect forehead(landmark_vec[19].x, landmark_vec[19].y - (int)round(head_width / 2), head_width, (int)round(head_width / 3));

	int nose_mouth = landmark_vec[51].y - landmark_vec[33].y;
	int cheek_width_1 = landmark_vec[31].x - landmark_vec[3].x;
	cv::Rect cheek_1(landmark_vec[31].x - (int)round(4 * cheek_width_1 / 5), landmark_vec[31].y - (int)round(nose_mouth / 2),
	(int)round(cheek_width_1 / 2), nose_mouth * 2);


	int cheek_width_2 = landmark_vec[15].x - landmark_vec[35].x;
	cv::Rect cheek_2(landmark_vec[35].x + (int)round(cheek_width_2 / 3), landmark_vec[35].y - (int)round(nose_mouth / 2),
	(int)round(cheek_width_2 / 2), nose_mouth * 2);


	int mouth = landmark_vec[54].x - landmark_vec[48].x;
	int mouth_chin = landmark_vec[8].y - landmark_vec[57].y;
	cv::Rect chin(landmark_vec[48].x, landmark_vec[57].y + (int)round(mouth_chin / 3),
	mouth, (int)round(mouth_chin / 2));

	int btw_eyes = landmark_vec[42].x - landmark_vec[39].x;
	int nose_height = landmark_vec[30].y - landmark_vec[27].y;
	cv::Rect nose(landmark_vec[27].x - (int)round(btw_eyes / 4), landmark_vec[27].y,
	(int)round(btw_eyes / 2), nose_height);

	cv::Rect throat(landmark_vec[7].x, landmark_vec[7].y,
	landmark_vec[9].x - landmark_vec[7].x, mouth_chin);

	rectangle(mask, cheek_2, cv::Scalar(cv::GC_FGD), -1, 8, 0);
	rectangle(mask, cheek_1, cv::Scalar(cv::GC_FGD), -1, 8, 0);
	rectangle(mask, chin, cv::Scalar(cv::GC_FGD), -1, 8, 0);
	rectangle(mask, forehead, cv::Scalar(cv::GC_FGD), -1, 8, 0);
	rectangle(mask, nose, cv::Scalar(cv::GC_FGD), -1, 8, 0);
	rectangle(mask, throat, cv::Scalar(cv::GC_FGD), -1, 8, 0);

	// FACE
	rectangle(mask, face, cv::Scalar(cv::GC_PR_FGD), -1, 8, 0);
	*/



	//--------------------------------------------------------------------------------
	if (down_sample)
	{
		down_img = cv_img;
		while (true)
		{
			cv::Mat temp = down_img;
			cv::Mat mask_temp = mask;
			if ((temp.cols / 2) % 2 == 0 && (temp.rows / 2) % 2 == 0)
			{
				cv::pyrDown(temp, down_img, cv::Size(temp.cols / 2, temp.rows / 2));
				cv::pyrDown(mask_temp, mask, cv::Size(temp.cols / 2, temp.rows / 2));
				down_iter++;
			}
			else
			{
				break;
			}
		}

		cv::grabCut(down_img, mask, cv::Rect(), bgModel, fgModel, 1, cv::GC_INIT_WITH_MASK);
		// Get the pixels marked as likely foreground
		cv::Mat mask_fg = (mask == cv::GC_FGD) | (mask == cv::GC_PR_FGD);

		cv::Mat resultUp = mask_fg;
		for (int i = 0; i < down_iter; i++)
		{
			cv::Mat temp = resultUp;
			cv::pyrUp(temp, resultUp, cv::Size(temp.cols * 2, temp.rows * 2));
		}


		// Close-open 
		end_mask = this->close_open(resultUp);

	}
	else
	{
		cv::grabCut(cv_img, mask, cv::Rect(), bgModel, fgModel, 1, cv::GC_INIT_WITH_MASK);
		cv::Mat mask_fg = (mask == cv::GC_FGD) | (mask == cv::GC_PR_FGD);
		// Close-open 
		end_mask = this->close_open(mask_fg);
	}

	cv::ellipse(end_mask, eye_rec_1, cv::Scalar(0), -1);
	cv::ellipse(end_mask, eye_rec_2, cv::Scalar(0), -1);
	cv::ellipse(end_mask, mouth_rec, cv::Scalar(0), -1);

	cv::Mat const structure_elem = cv::getStructuringElement(
		cv::MORPH_RECT, cv::Size(3, 3));
	cv::morphologyEx(end_mask, end_mask,
		cv::MORPH_ERODE, structure_elem, cv::Point(-1, -1), 25);

	cv::ellipse(end_mask, nostril_rec, cv::Scalar(0), -1);




	cv_img.copyTo(foreground, end_mask); // bg pixels not copied

	return std::make_pair(end_mask, foreground);
}

cv::Mat MyImage::close_open(cv::Mat mask, int iterations, int kernels_size)
{
	cv::Mat const structure_elem = cv::getStructuringElement(
		cv::MORPH_CROSS, cv::Size(kernels_size, kernels_size));
	cv::Mat open_result, close_result, end_result;

	cv::morphologyEx(mask, close_result,
		cv::MORPH_CLOSE, structure_elem, cv::Point(-1, -1), iterations);
	cv::morphologyEx(close_result, end_result,
		cv::MORPH_OPEN, structure_elem, cv::Point(-1, -1), iterations);

	return end_result;

}

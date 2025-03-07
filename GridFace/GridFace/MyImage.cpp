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

	dlib::deserialize("dlib\\shape_predictor_68_face_landmarks.dat") >> model_dlib;


	// Create blob detector

	// set up the parameters (check the defaults in opencv's code in blobdetector.cpp)
	cv::SimpleBlobDetector::Params params;
	params.minDistBetweenBlobs = 1.0f;
	params.filterByInertia = true;
	params.minInertiaRatio = 0;
	params.maxInertiaRatio = 1;
	params.filterByConvexity = true;
	params.maxConvexity = 1;
	params.minConvexity = 0;
	//params.filterByColor = false;
	params.filterByCircularity = true;
	params.maxCircularity = 1;
	params.minCircularity = 0;
	params.filterByArea = true;
	params.minArea = 1.0f;
	params.maxArea = 50000.0f;

	// set up and create the detector using the parameters
	this->blob_detector = cv::SimpleBlobDetector::create(params);

	
	cv::FileStorage fsDemo("w2c.yml", cv::FileStorage::READ);
	fsDemo["w2c"] >> this->w2c;
	fsDemo.release();

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
	
	if (this->faces_vec.size() > 1)
	{
		int temp_area = faces_vec[0].area();

		for (int num = 1; num < faces_vec.size(); num++)
		{
			if (faces_vec[num].area() < temp_area)
			{
				faces_vec.erase(faces_vec.begin() + num);
				num--;
			}
			else
			{
				temp_area = faces_vec[num].area();
				faces_vec.erase(faces_vec.begin() + num - 1);
				num--;
			}
		}

	}

	if (this->faces_vec.size() == 0)
	{
		this->display_message("No face detected! Exiting!");
	}
}

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
		cv::circle(orig_img, this->landmark_vec[i], 4, CV_RGB(0, 0, 255), CV_FILLED);
		//// Display number 
		//cv::putText(orig_img, std::to_string(i), this->landmark_vec[i] + cv::Point(5,5),		
		//	CV_FONT_HERSHEY_SIMPLEX, 0.5, CV_RGB(0, 0, 255), 2); 

	}
}

void MyImage::create_areas()
{
	std::vector<cv::Point> vec = this->landmark_vec;
	int height = this->img_size.height;
	int width = this->img_size.width;

	std::vector<cv::Point> area; 

	area.push_back(cv::Point(0, 0));
	area.push_back(cv::Point(vec[36].x, 0));
	area.push_back(vec[36]);
	area.push_back(cv::Point(0, vec[36].y));
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(cv::Point(vec[36].x, 0));
	area.push_back(cv::Point(vec[27].x, 0));
	area.push_back(vec[27]);
	area.push_back(vec[36]);
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(cv::Point(vec[27].x, 0));
	area.push_back(cv::Point(vec[45].x, 0));
	area.push_back(vec[45]);
	area.push_back(vec[27]);
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(cv::Point(vec[45].x, 0));
	area.push_back(cv::Point(width, 0));
	area.push_back(cv::Point(width,vec[45].y));
	area.push_back(vec[45]);
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(cv::Point(0, vec[36].y));
	area.push_back(vec[36]);
	area.push_back(cv::Point(vec[36].x, vec[48].y));
	area.push_back(cv::Point(0, vec[48].y));
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(vec[36]);
	area.push_back(vec[27]);
	area.push_back(cv::Point(vec[27].x, vec[48].y));
	area.push_back(cv::Point(vec[36].x, vec[48].y));
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(vec[27]);
	area.push_back(vec[45]);
	area.push_back(cv::Point(vec[45].x, vec[54].y));
	area.push_back(cv::Point(vec[27].x, vec[48].y));
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(vec[45]);
	area.push_back(cv::Point(width, vec[45].y));
	area.push_back(cv::Point(width, vec[54].y));
	area.push_back(cv::Point(vec[45].x, vec[54].y));
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(cv::Point(0, vec[48].y));
	area.push_back(cv::Point(vec[36].x, vec[48].y));
	area.push_back(cv::Point(vec[36].x, vec[8].y));
	area.push_back(cv::Point(0, vec[8].y));
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(cv::Point(vec[36].x, vec[48].y));
	area.push_back(cv::Point(vec[27].x, vec[48].y));
	area.push_back(vec[8]);
	area.push_back(cv::Point(vec[36].x, vec[8].y));
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(cv::Point(vec[27].x, vec[48].y));
	area.push_back(cv::Point(vec[45].x, vec[54].y));
	area.push_back(cv::Point(vec[45].x, vec[8].y));
	area.push_back(vec[8]);
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(cv::Point(vec[45].x, vec[54].y));
	area.push_back(cv::Point(width, vec[54].y));
	area.push_back(cv::Point(width, vec[8].y));
	area.push_back(cv::Point(vec[45].x, vec[8].y));
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(cv::Point(0, vec[8].y));
	area.push_back(cv::Point(vec[36].x, vec[8].y));
	area.push_back(cv::Point(vec[36].x, height));
	area.push_back(cv::Point(0, height));
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(cv::Point(vec[36].x, vec[8].y));
	area.push_back(vec[8]);
	area.push_back(cv::Point(vec[8].x, height));
	area.push_back(cv::Point(vec[36].x, height));
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(vec[8]);
	area.push_back(cv::Point(vec[45].x, vec[8].y));
	area.push_back(cv::Point(vec[45].x, height));
	area.push_back(cv::Point(vec[8].x, height));
	this->areas_vec.push_back(area);
	area.clear();

	area.push_back(cv::Point(vec[45].x, vec[8].y));
	area.push_back(cv::Point(width, vec[8].y));
	area.push_back(cv::Point(width, height));
	area.push_back(cv::Point(vec[45].x, height));
	this->areas_vec.push_back(area);
	area.clear();
}

void MyImage::draw_grid(cv::Mat img)
{
	
	if (this->areas_vec.empty())
	{
		this->create_areas();
	}	
	for (int num = 0; num < this->areas_vec.size(); num++)
	{
		cv::drawContours(img, this->areas_vec, num, cv::Scalar(0,0,0), 4, 8);
	}

}

void MyImage::draw_record(cv::Mat img)
{
	std::vector<std::vector<cv::Point>> contours;

	for (int num = 0; num < this->point1_vec.size(); num++)
	{
		int width = this->point2_vec[num].x - this->point1_vec[num].x;
		int height = this->point2_vec[num].y - this->point1_vec[num].y;
		cv::Point center = cv::Point(this->point1_vec[num].x + round(width / 2), this->point1_vec[num].y + round(height / 2));
		cv::RotatedRect	actaul_rect = cv::RotatedRect(center, cv::Size(width, height), this->rotate_angle);
		cv::Point2f vertices[4];
		actaul_rect.points(vertices);
		std::vector<cv::Point> points = std::vector<cv::Point>(vertices, vertices + sizeof vertices / sizeof vertices[0]);
		contours.push_back(points);

	}


	for (int num = 0; num < contours.size(); num++)
	{

		cv::Scalar my_permanent(255, 0, 0);
		cv::Scalar my_non_permanent(0, 0, 255);

		if (this->class_type_vec[num] == 1)
		{
			cv::drawContours(img, contours, num, my_permanent, 4);
		}
		else
		{
			cv::drawContours(img, contours, num, my_non_permanent, 4);
		}

	}


}

cv::Mat MyImage::rotate_img(cv::Mat orig_img)
{
	cv::Mat rotated_img;

	cv::Point2f pt(orig_img.cols / 2., orig_img.rows / 2.);
	cv::Mat r = getRotationMatrix2D(pt, -this->rotate_angle, 1.0);
	warpAffine(orig_img, rotated_img, r, cv::Size(orig_img.cols, orig_img.rows));

	return rotated_img;
}

float MyImage::my_angle_calculater(cv::Point a, cv::Point b, cv::Point c)
{
	cv::Point diff_ab = b - a;
	cv::Point diff_ac = c - a;
	float dist_ab = sqrt(std::pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
	float dist_ac = sqrt(std::pow((c.x - a.x), 2) + pow((c.y - a.y), 2));
	float dotprod = diff_ab.dot(diff_ac);
	float angle = acos(dotprod / (dist_ab*dist_ac));
	angle = angle * (float)180 / M_PI;
	if (c.y > b.y)
	{
		angle = -angle;
	}
	return angle;
}

void MyImage::calc_angle()
{
	if (!landmark_vec.empty())
	{
		cv::Point left_eye_1 = landmark_vec[36];
		cv::Point left_eye_2 = landmark_vec[39];
		cv::Point right_eye_1 = landmark_vec[42];
		cv::Point right_eye_2 = landmark_vec[45];
		cv::Point straight_point_1(left_eye_1.x + right_eye_1.x, left_eye_1.y);
		cv::Point straight_point_2(left_eye_2.x + right_eye_2.x, left_eye_2.y);

		float angle_1 = this->my_angle_calculater(left_eye_1, straight_point_1, right_eye_1);
		float angle_2 = this->my_angle_calculater(left_eye_2, straight_point_2, right_eye_2);
		float angle = (angle_1 + angle_2) / 2;
		this->rotate_angle = angle;
	}
	else
	{
		// ERROR
	}
}

std::vector<cv::Point> MyImage::rotate_points(std::vector<cv::Point> point_vec)
{
	std::vector<cv::Point> rotated_points; 
	cv::Point2f cen_pt(img_size.width / 2., img_size.height / 2.);
	float rand = M_PI*(this->rotate_angle / 180);
	float s = sin(rand);
	float c = cos(rand);
	cv::Point temp_point; 

	for (int num = 0; num < point_vec.size(); num++)
	{
		temp_point = point_vec[num];
		// translate point back to origin:
		temp_point.x -= cen_pt.x;
		temp_point.y -= cen_pt.y;

		// rotate point
		float xnew = temp_point.x * c - temp_point.y * s;
		float ynew = temp_point.x * s + temp_point.y * c;

		// translate point back:
		temp_point.x = xnew + cen_pt.x;
		temp_point.y = ynew + cen_pt.y;

		rotated_points.push_back(temp_point);
	}
	return rotated_points;
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
resize_factor = 0;
rotate_angle = 0;
areas_vec.clear();
index_checked_detections.clear();
img_size = cv::Size(0,0);
candidates.clear();
frequency.clear();
predictions.clear();
}

void MyImage::display_message(QString msg)
{
	QMessageBox message;
	message.setText(msg);
	message.exec();
	exit(1);
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
	cv::Mat remove_mask = cv::Mat::ones(cv_img.size(), CV_8UC1);

	std::vector<cv::Point> lower_face_roi;
	std::vector<cv::Point> below_chin_roi;
	std::vector<cv::Point> eye_1_vec;
	std::vector<cv::Point> eye_2_vec;
	std::vector<cv::Point> nose_vec;
	std::vector<cv::Point> lower_face_vec;
	std::vector<cv::Point> mouth_vec;
	std::vector<cv::Point> nostrils_vec;
	std::vector<cv::Point> below_chin_vec;

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
	for (int i = 2; i <= 14; i++)
	{
		below_chin_vec.push_back(landmark_vec[i]);
	}

	for (int i = 31; i <= 35; i++)
	{
		nostrils_vec.push_back(landmark_vec[i]);
	}
	nostrils_vec.push_back(cv::Point(landmark_vec[34].x, landmark_vec[30].y + (landmark_vec[33].y - landmark_vec[30].y) / 2));
	nostrils_vec.push_back(landmark_vec[30]);
	nostrils_vec.push_back(cv::Point(landmark_vec[32].x, landmark_vec[30].y + (landmark_vec[33].y - landmark_vec[30].y) / 2));



	below_chin_vec.push_back(cv::Point(cv_img.size().width, landmark_vec[14].y));
	below_chin_vec.push_back(cv::Point(cv_img.size().width, cv_img.size().height));
	below_chin_vec.push_back(cv::Point(0, cv_img.size().height));
	below_chin_vec.push_back(cv::Point(0, landmark_vec[2].y));


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
	cv::approxPolyDP(below_chin_vec, below_chin_roi, 1.0, true);

	cv::fillConvexPoly(mask, &lower_face_roi[0], lower_face_roi.size(), cv::Scalar(cv::GC_PR_FGD), 8, 0);
	rectangle(mask, forehead, cv::Scalar(cv::GC_PR_FGD), -1, 8, 0);
	rectangle(mask, throat, cv::Scalar(cv::GC_PR_FGD), -1, 8, 0);

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

	cv::ellipse(remove_mask, eye_rec_1, cv::Scalar(0), -1);
	cv::ellipse(remove_mask, eye_rec_2, cv::Scalar(0), -1);
	cv::ellipse(remove_mask, mouth_rec, cv::Scalar(0), -1);


	cv::Mat const structure_elem = cv::getStructuringElement(
		cv::MORPH_RECT, cv::Size(3, 3));
	cv::morphologyEx(remove_mask, remove_mask,
		cv::MORPH_ERODE, structure_elem, cv::Point(-1, -1), 25);

	std::vector<std::vector<cv::Point>> temp_contours; 
	temp_contours.push_back(below_chin_roi);
	cv::drawContours(remove_mask, temp_contours, 0, cv::Scalar(0), -1);
	
	cv::morphologyEx(remove_mask, remove_mask,
		cv::MORPH_DILATE, structure_elem, cv::Point(-1, -1), 5);
	
	cv::ellipse(remove_mask, nostril_rec, cv::Scalar(0), -1);

	end_mask.setTo(0,remove_mask == 0);

	cv_img.copyTo(foreground, end_mask); // bg pixels not copied

	return std::make_pair(end_mask, foreground);
}

cv::Mat MyImage::FRS(cv::Mat cv_img)
{
	int num_row = cv_img.rows;
	int num_col = cv_img.cols;
	int sigma = 0;
	double k_scal = 9.9;
	double alpha = 2;
	double thresh = 0.001;
	std::vector<int> radii = {2, 4, 6, 8, 10, 12, 14, 16, 18};
	//std::vector<int> radii = { 5, 7, 9, 11, 13, 15, 17, 19, 21, 23 };
	cv::Mat gradient, magnitude, orientation, F_img, S_img;
	cv::Mat S_total = cv::Mat::zeros(num_row, num_col, CV_32F);
	cv::Mat img_gray, blurred;
	// Gaussian blur
	cv::GaussianBlur(cv_img, blurred, cv::Size(3, 3), sigma, sigma, cv::BORDER_DEFAULT);
	/// Convert it to gray
	cv::cvtColor(blurred, img_gray, CV_BGR2GRAY);
	//--------------------------------- gradient 
	cv::Mat grad_x, grad_y;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_32F;
	/// Gradient X
	Sobel(img_gray, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);
	/// Gradient Y
	Sobel(img_gray, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);

	gradient = cv::Mat::zeros(num_row, num_col, CV_32F);
	float *grad_x_ptr = (float*)(grad_x.data);
	float *grad_y_ptr = (float*)(grad_y.data);
	float *grad_ptr = (float*)(gradient.data);
	for (int row = 0; row < grad_x.rows; row++){
		for (int col = 0; col < grad_x.cols; col++){
			float grad_norm = sqrt(pow(grad_x_ptr[grad_x.cols * row + col], 2) + pow(grad_y_ptr[grad_y.cols * row + col], 2));
			grad_ptr[gradient.cols * row + col] = grad_norm + 0.000001;
		}
	}

	//----------------------------------
	for (int rad = 0; rad < radii.size(); rad++)
	{


		magnitude = cv::Mat::zeros(num_row + radii[rad] * 2, num_col + radii[rad] * 2, CV_32F);
		orientation = cv::Mat::zeros(num_row + radii[rad] * 2, num_col + radii[rad] * 2, CV_32F);
		F_img = cv::Mat::zeros(num_row + radii[rad] * 2, num_col + radii[rad] * 2, CV_32F);
		S_img = cv::Mat::zeros(num_row + radii[rad] * 2, num_col + radii[rad] * 2, CV_32F);

		float *magnitude_ptr = (float*)(magnitude.data);
		float *orientation_ptr = (float*)(orientation.data);

		double maxG, minG;
		cv::minMaxLoc(gradient, &minG, &maxG);
		for (int row = 0; row < gradient.rows; row++){
			for (int col = 0; col < gradient.cols; col++){
				cv::Point pos_pix(col, row);
				cv::Point neg_pix(col, row);
				int radius = radii[rad];
				double grad_pix = grad_ptr[gradient.cols * row + col];
				//if (grad_pix > maxG*thresh)
				{
					pos_pix.x += round((grad_x_ptr[grad_x.cols * row + col] / grad_pix) * radius);
					pos_pix.y += round((grad_y_ptr[grad_y.cols * row + col] / grad_pix) * radius);
					neg_pix.x -= round((grad_x_ptr[grad_x.cols * row + col] / grad_pix) * radius);
					neg_pix.y -= round((grad_y_ptr[grad_y.cols * row + col] / grad_pix) * radius);

					magnitude_ptr[magnitude.cols * (pos_pix.y + radius) + (pos_pix.x + radius)] += grad_pix;
					magnitude_ptr[magnitude.cols * (neg_pix.y + radius) + (neg_pix.x + radius)] -= grad_pix;
					orientation_ptr[magnitude.cols * (pos_pix.y + radius) + (pos_pix.x + radius)] += 1;
					orientation_ptr[magnitude.cols * (neg_pix.y + radius) + (neg_pix.x + radius)] -= 1;
				}
			}
		}


		// Manipulate the orientation and magnitude 
		orientation = cv::abs(orientation); // DETTA SKILJER SIG FR�N ORGINAL TEORIN BORDE VARA EFTER
		orientation.setTo(k_scal, orientation > k_scal);

		orientation = orientation / k_scal;
		cv::pow(orientation, alpha, orientation);
		magnitude = magnitude / k_scal;

		// Calculate F
		F_img = magnitude.mul(orientation);

		// Calculate S
		int kern_size = 0;
		switch ((int)round(sqrt(rad)))
		{
		case 1:
			kern_size = 1;
			break;
		case 2:
			kern_size = 3;
			break;
		case 3:
			kern_size = 3;
			break;
		case 4:
			kern_size = 5;
			break;
		case 5:
			kern_size = 5;
			break;
		default:
			kern_size = 7;
		}


		cv::GaussianBlur(F_img, S_img, cv::Size(kern_size, kern_size), sigma, sigma, cv::BORDER_DEFAULT);

		cv::Rect roi(radii[rad] - 1, radii[rad] - 1, num_col, num_row);

		S_total += S_img(roi);
	}

	S_total = S_total / radii.size();

	return S_total;
}

std::pair<std::vector<std::vector<cv::Point>>, std::vector<cv::Rect>> MyImage::get_candidates(cv::Mat cv_img, cv::Mat mask, cv::Mat frs, double threshold)
{

	cv::Mat frs_img = frs.clone(); 
	frs_img = cv::abs(frs_img);	
	// Remove seed outside mask 
	frs_img.setTo(0, mask == 0);
	// Find max value in frs
	double max, min;
	cv::minMaxLoc(frs_img, &min, &max);

	// NORMAL THRESHOLDING
	cv::threshold(frs_img, frs_img, threshold*max, 1, cv::THRESH_BINARY);

	// Draw the background markers
	cv::Mat background;

	cv::Mat const structure_elem = cv::getStructuringElement(
		cv::MORPH_RECT, cv::Size(3, 3));
	cv::morphologyEx(frs_img, background,
		cv::MORPH_DILATE, structure_elem, cv::Point(-1, -1), 10);

	cv::threshold(background, background, 0, 1337, cv::THRESH_BINARY_INV);

	cv::Mat back_32s;
	background.convertTo(back_32s, CV_32SC1);


	// Create the CV_8U version of the distance image It is needed for findContours()
	cv::Mat dist_8u;
	frs_img.convertTo(dist_8u, CV_8U);


	// Find seed counters
	std::vector<std::vector<cv::Point>> contours;
	findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	// Draw the foreground markers
	cv::Mat markers_32s = cv::Mat::zeros(frs_img.size(), CV_32SC1);
	for (int i = 0; i < contours.size(); i++)
		drawContours(markers_32s, contours, i, cv::Scalar::all(i+1), -1);

	//mask.convertTo(mask, CV_32S);


	markers_32s = markers_32s + back_32s;

	// Perform the watershed algorithm

	cv::watershed(cv_img, markers_32s);


	// Connect nearby marks and find new counters
	cv::Mat closed_marks = markers_32s.clone();
	closed_marks.setTo(1, markers_32s >= -1);
	closed_marks.setTo(0, markers_32s >= 1337);

	std::vector<std::vector<cv::Point>> new_contours;
	cv::Mat closed_marks_8u;
	closed_marks.convertTo(closed_marks_8u, CV_8U);
	findContours(closed_marks_8u, new_contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);


	/// Get bounding box 
	std::vector<cv::Rect> boundRects(new_contours.size());


	for (int i = 0; i < new_contours.size(); i++)
	{
		boundRects[i] = cv::boundingRect(cv::Mat(new_contours[i]));
	}
	
	return std::make_pair(new_contours, boundRects);
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

std::vector<MyImage> MyImage::extract_from_record(std::string file_path)
{
	MyImage img;			
	std::vector<MyImage> image_vec;
	std::string line, part;
	int counter = 0;
	int temp_int;
	std::vector<int> temp_vec;

	std::ifstream record_file(file_path);

	QFileInfo info1(QString::fromStdString(file_path));
	
	if (record_file.is_open())
	{
		while (std::getline(record_file, line))
		{


			if (line == "break")
			{
				counter = 0;
				image_vec.push_back(img);
				img.clear_data();

			}
			else if (counter == 2)
			{
				std::istringstream iss(line);
				while (iss >> part)
				{
					temp_int = atoi(part.c_str());
					temp_vec.push_back(temp_int);
				}

				img.insert_selection(cv::Point(temp_vec[0], temp_vec[1]), cv::Point(temp_vec[2], temp_vec[3]), temp_vec[4]);
				temp_vec.clear();
			}

			else if (counter == 1)
			{
				img.set_name(line,1);
				img.set_name(info1.path().toStdString().c_str(), 2);
				counter++;
			}
			else if (counter == 0)
			{
				counter++;
			}
			else
			{
				// ERROR 
			}

		}
		record_file.close();
	}

	else
	{
	}

	return image_vec;
}

std::vector<int> MyImage::extract_lables(std::vector<MyImage> record)
{
	std::vector<int> labels_vec;
	for (int i = 0; i < record.size(); i++)
	{
		labels_vec.insert(labels_vec.end(), record[i].class_type_vec.begin(), record[i].class_type_vec.end());
	}
	return labels_vec;
}

cv::Mat MyImage::extract_training_data(std::vector<MyImage> record, std::string path)
{
	cv::Mat curr_img, img_1, img_2, img_3, img_4, img_5, img_6, img_7, img_8, img_9, img_10, img_11;
	cv::Mat mark, mark_1, mark_2, mark_3, mark_4, mark_5, mark_6, mark_7, mark_8, mark_9, mark_10, mark_11;
	std::vector<float> feature_vec;
	cv::Mat training_data;

	for (int num = 0; num < record.size(); num++)
	{
		curr_img = cv::imread(path + "\\" + record[num].get_name(1));
		img_1 = true_colours(curr_img, 0);
		img_2 = true_colours(curr_img, 1);
		img_3 = true_colours(curr_img, 2);
		img_4 = true_colours(curr_img, 3);
		img_5 = true_colours(curr_img, 4);
		img_6 = true_colours(curr_img, 5);
		img_7 = true_colours(curr_img, 6);
		img_8 = true_colours(curr_img, 7);
		img_9 = true_colours(curr_img, 8);
		img_10 = true_colours(curr_img, 9);
		img_11 = true_colours(curr_img, 10);

		for (int kum = 0; kum < record[num].point1_vec.size(); kum++)
		{

			cv::Rect my_rect = cv::Rect(record[num].point1_vec[kum], record[num].point2_vec[kum]);

			mark = curr_img(my_rect);
			mark_1 = img_1(my_rect);
			mark_2 = img_2(my_rect);
			mark_3 = img_3(my_rect);
			mark_4 = img_4(my_rect);
			mark_5 = img_5(my_rect);
			mark_6 = img_6(my_rect);
			mark_7 = img_7(my_rect);
			mark_8 = img_8(my_rect);
			mark_9 = img_9(my_rect);
			mark_10 = img_10(my_rect);
			mark_11 = img_11(my_rect);

			cv::Scalar mean;
			cv::Scalar stdvar;


			////////// RGB
			//cv::meanStdDev(mark, mean, stdvar);
			//for (int i = 0; i < 3; i++)
			//{
			//	feature_vec.push_back(mean[i]);
			//}
			//for (int i = 0; i < 3; i++)
			//{
			//	feature_vec.push_back(stdvar[i]);
			//}


			/////////////// HSV
			//cv::Mat img_hsv;
			//cv::cvtColor(mark, img_hsv, CV_BGR2HSV);
			//cv::meanStdDev(img_hsv, mean, stdvar);

			//feature_vec.push_back(mean[0]);
			//feature_vec.push_back(mean[1]);
			//feature_vec.push_back(mean[2]);
			//feature_vec.push_back(stdvar[0]);
			//feature_vec.push_back(stdvar[1]);
			//feature_vec.push_back(stdvar[2]);

			/////////////// COLORS
			cv::meanStdDev(mark_1, mean, stdvar);
			feature_vec.push_back(mean[0]);
			feature_vec.push_back(stdvar[0]);

			cv::meanStdDev(mark_2, mean, stdvar);
			feature_vec.push_back(mean[0]);
			feature_vec.push_back(stdvar[0]);

			cv::meanStdDev(mark_3, mean, stdvar);
			feature_vec.push_back(mean[0]);
			feature_vec.push_back(stdvar[0]);

			cv::meanStdDev(mark_4, mean, stdvar);
			feature_vec.push_back(mean[0]);
			feature_vec.push_back(stdvar[0]);

			cv::meanStdDev(mark_5, mean, stdvar);
			feature_vec.push_back(mean[0]);
			feature_vec.push_back(stdvar[0]);

			cv::meanStdDev(mark_6, mean, stdvar);
			feature_vec.push_back(mean[0]);
			feature_vec.push_back(stdvar[0]);

			cv::meanStdDev(mark_7, mean, stdvar);
			feature_vec.push_back(mean[0]);
			feature_vec.push_back(stdvar[0]);

			cv::meanStdDev(mark_8, mean, stdvar);
			feature_vec.push_back(mean[0]);
			feature_vec.push_back(stdvar[0]);

			cv::meanStdDev(mark_9, mean, stdvar);
			feature_vec.push_back(mean[0]);
			feature_vec.push_back(stdvar[0]);

			cv::meanStdDev(mark_10, mean, stdvar);
			feature_vec.push_back(mean[0]);
			feature_vec.push_back(stdvar[0]);

			cv::meanStdDev(mark_11, mean, stdvar);
			feature_vec.push_back(mean[0]);
			feature_vec.push_back(stdvar[0]);


			cv::Mat training_data_row(feature_vec);

			//cv::Mat training_data_row = get_hog(mark);

			training_data.push_back(training_data_row.t());
			feature_vec.clear();
		}
	}
	return training_data;
}

cv::Ptr<cv::ml::SVM> MyImage::train_SVM(cv::Mat training_data, cv::Mat training_labels, float C, float gamma)
{

	// Train the SVM
	cv::Ptr<cv::ml::SVM> my_svm = cv::ml::SVM::create();
	my_svm->setType(cv::ml::SVM::C_SVC);
	my_svm->setKernel(cv::ml::SVM::RBF);  // OBSERVERA FUNKAR INTE ATT LOADA NON-LINEAR KERNELS //
	my_svm->setGamma(gamma); // OBSERVERA FUNKAR INTE ATT LOADA NON-LINEAR KERNELS //
	my_svm->setC(C);
	my_svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 5000, 1e-6));
	my_svm->train(training_data, cv::ml::ROW_SAMPLE, training_labels);

	return my_svm;
}

cv::Mat MyImage::predict_data(cv::Mat image, std::vector<cv::Rect> candidates)
{

	cv::Mat mark_1, mark_2, mark_3, mark_4, mark_5, mark_6, mark_7, mark_8, mark_9, mark_10, mark_11;
	cv::Mat img_1, img_2, img_3, img_4, img_5, img_6, img_7, img_8, img_9, img_10, img_11;

	img_1 = true_colours(image, 0);
	img_2 = true_colours(image, 1);
	img_3 = true_colours(image, 2);
	img_4 = true_colours(image, 3);
	img_5 = true_colours(image, 4);
	img_6 = true_colours(image, 5);
	img_7 = true_colours(image, 6);
	img_8 = true_colours(image, 7);
	img_9 = true_colours(image, 8);
	img_10 = true_colours(image, 9);
	img_11 = true_colours(image, 10);

	cv::Mat predictiondata;

	for (int rum = 0; rum < candidates.size(); rum++)
	{

		std::vector<float> feature_vec;
		cv::Rect temp_rect = candidates[rum];
		temp_rect = enlarg_rect_to_predict(temp_rect, 30, image.size());

		cv::Mat mark = image(temp_rect);

		mark_1 = img_1(temp_rect);
		mark_2 = img_2(temp_rect);
		mark_3 = img_3(temp_rect);
		mark_4 = img_4(temp_rect);
		mark_5 = img_5(temp_rect);
		mark_6 = img_6(temp_rect);
		mark_7 = img_7(temp_rect);
		mark_8 = img_8(temp_rect);
		mark_9 = img_9(temp_rect);
		mark_10 = img_10(temp_rect);
		mark_11 = img_11(temp_rect);




		cv::Scalar mean;
		cv::Scalar stdvar;


		////////// RGB
		//cv::meanStdDev(mark, mean, stdvar);
		//for (int i = 0; i < 3; i++)
		//{
		//	feature_vec.push_back(mean[i]);
		//}
		//for (int i = 0; i < 3; i++)
		//{
		//	feature_vec.push_back(stdvar[i]);
		//}


		/////////////// HSV
		//cv::Mat img_hsv;
		//cv::cvtColor(mark, img_hsv, CV_BGR2HSV);
		//cv::meanStdDev(img_hsv, mean, stdvar);

		//feature_vec.push_back(mean[0]);
		//feature_vec.push_back(mean[1]);
		//feature_vec.push_back(mean[2]);
		//feature_vec.push_back(stdvar[0]);
		//feature_vec.push_back(stdvar[1]);
		//feature_vec.push_back(stdvar[2]);

		/////////////// COLORS
		cv::meanStdDev(mark_1, mean, stdvar);
		feature_vec.push_back(mean[0]);
		feature_vec.push_back(stdvar[0]);

		cv::meanStdDev(mark_2, mean, stdvar);
		feature_vec.push_back(mean[0]);
		feature_vec.push_back(stdvar[0]);

		cv::meanStdDev(mark_3, mean, stdvar);
		feature_vec.push_back(mean[0]);
		feature_vec.push_back(stdvar[0]);

		cv::meanStdDev(mark_4, mean, stdvar);
		feature_vec.push_back(mean[0]);
		feature_vec.push_back(stdvar[0]);

		cv::meanStdDev(mark_5, mean, stdvar);
		feature_vec.push_back(mean[0]);
		feature_vec.push_back(stdvar[0]);

		cv::meanStdDev(mark_6, mean, stdvar);
		feature_vec.push_back(mean[0]);
		feature_vec.push_back(stdvar[0]);

		cv::meanStdDev(mark_7, mean, stdvar);
		feature_vec.push_back(mean[0]);
		feature_vec.push_back(stdvar[0]);

		cv::meanStdDev(mark_8, mean, stdvar);
		feature_vec.push_back(mean[0]);
		feature_vec.push_back(stdvar[0]);

		cv::meanStdDev(mark_9, mean, stdvar);
		feature_vec.push_back(mean[0]);
		feature_vec.push_back(stdvar[0]);

		cv::meanStdDev(mark_10, mean, stdvar);
		feature_vec.push_back(mean[0]);
		feature_vec.push_back(stdvar[0]);

		cv::meanStdDev(mark_11, mean, stdvar);
		feature_vec.push_back(mean[0]);
		feature_vec.push_back(stdvar[0]);


		cv::Mat training_data_row(feature_vec);

		//cv::Mat training_data_row = get_hog(mark);


		predictiondata.push_back(training_data_row.t());
		feature_vec.clear();
	

	}
			return predictiondata;
}

cv::Mat MyImage::calc_confusion(cv::Mat predicted, cv::Mat actual)
{
	actual.convertTo(actual, CV_32FC1);
	cv::Mat confusion_matrix(2, 2, CV_32FC1, cv::Scalar(0));
	float *confusion_matrix_ptr = (float*)(confusion_matrix.data);
	float *predicted_ptr = (float*)(predicted.data);
	float *actual_ptr = (float*)(actual.data);

	for (int num = 0; num < predicted.rows; num++)
	{
		int current_lable_pre = predicted_ptr[num];
		int current_lable_act = actual_ptr[num];
		if (current_lable_pre == current_lable_act)
		{
			if (current_lable_pre == 1)
			{
				confusion_matrix_ptr[0] += 1;
			}
			else
			{
				confusion_matrix_ptr[3] += 1;
			}
		}
		else
		{
			if (current_lable_pre == 1)
			{
				confusion_matrix_ptr[2] += 1;
			}
			else
			{
				confusion_matrix_ptr[1] += 1;
			}
		}
	}

	return confusion_matrix;
}

void MyImage::intersecting_areas(MyImage& actual, MyImage& predicted)
{
	int width, height;
	float predicted_area, actual_area, inter_area;
	cv::Point center, tr;
	cv::RotatedRect actaul_rect, predicted_rect;
	std::vector<cv::Point> inter_points, predicted_points, actual_points;
	cv::Point2f vertices[4];

	for (int num = 0; num < actual.class_type_vec.size(); num++)
	{
		width = actual.point2_vec[num].x - actual.point1_vec[num].x;
		height = actual.point2_vec[num].y - actual.point1_vec[num].y;
		center = cv::Point(actual.point1_vec[num].x + round(width / 2), actual.point1_vec[num].y + round(height / 2));
		actaul_rect = cv::RotatedRect(center, cv::Size(width, height), predicted.rotate_angle); 


		actaul_rect.points(vertices);
		actual_points = std::vector<cv::Point>(vertices, vertices + sizeof vertices / sizeof vertices[0]);
		actual_area = cv::contourArea(actual_points);

		for (int kum = 0; kum < predicted.candidates.size(); kum++)
		{

			tr = cv::Point(predicted.candidates[kum].tl().x + (predicted.candidates[kum].br().x - predicted.candidates[kum].tl().x), predicted.candidates[kum].tl().y);
			predicted_rect = cv::RotatedRect(predicted.candidates[kum].tl(), tr, predicted.candidates[kum].br());

			predicted_rect.points(vertices);
			predicted_points = std::vector<cv::Point>(vertices, vertices + sizeof vertices / sizeof vertices[0]);
			predicted_area = cv::contourArea(predicted_points);

			cv::rotatedRectangleIntersection(actaul_rect, predicted_rect, inter_points);

			if (inter_points.size() > 2)
			{
				inter_area = cv::contourArea(inter_points);
				//if (predicted_area < actual_area * 2)
				{
					predicted.index_checked_detections.push_back(kum);			
					inter_points.clear();
					break;
				}

				inter_points.clear();

			}

		}
	}
}

void MyImage::rezize_points(MyImage& my_image)
{
	for (int num = 0; num < my_image.point1_vec.size(); num++)
	{
		my_image.point1_vec[num].x = round(my_image.point1_vec[num].x * my_image.resize_factor);
		my_image.point1_vec[num].y = round(my_image.point1_vec[num].y * my_image.resize_factor);
		my_image.point2_vec[num].x = round(my_image.point2_vec[num].x * my_image.resize_factor);
		my_image.point2_vec[num].y = round(my_image.point2_vec[num].y * my_image.resize_factor);
	}
	for (int num = 0; num < my_image.landmark_vec.size(); num++)
	{
		my_image.landmark_vec[num].x = round(my_image.landmark_vec[num].x * my_image.resize_factor);
		my_image.landmark_vec[num].y = round(my_image.landmark_vec[num].y * my_image.resize_factor);
	}
	if (my_image.img_size != cv::Size(0,0))
	{
	my_image.img_size.height = round(my_image.img_size.height * my_image.resize_factor);
	my_image.img_size.width = round(my_image.img_size.width * my_image.resize_factor);
	}


}

cv::Mat MyImage::true_colours(cv::Mat image, int colour)
{


	image.convertTo(image, CV_32F);

	std::vector<cv::Mat> chanels;
	split(image, chanels);

	cv::Mat b_mat = chanels[0] / 8;
	cv::Mat g_mat = chanels[1] / 8;
	cv::Mat r_mat = chanels[2] / 8;

	float *b_mat_ptr = (float*)(b_mat.data);
	float *g_mat_ptr = (float*)(g_mat.data);
	float *r_mat_ptr = (float*)(r_mat.data);

	for (int num = 0; num < b_mat.rows*b_mat.cols; num++)
	{
		b_mat_ptr[num] = floor(b_mat_ptr[num]);
		g_mat_ptr[num] = floor(g_mat_ptr[num]);
		r_mat_ptr[num] = floor(r_mat_ptr[num]);
	}

	cv::Mat one_mat = cv::Mat::ones(image.size(), CV_32F);
	cv::Mat index_mat = one_mat + r_mat + (32 * g_mat) + (32 * 32 * b_mat);

	cv::Mat w2c_choice = this->w2c.col(colour).clone();

	float *w2c_choice_ptr = (float*)(w2c_choice.data);
	float *index_mat_ptr = (float*)(index_mat.data);


	for (int num = 0; num < index_mat.rows*index_mat.cols; num++)
	{
		index_mat_ptr[num] = w2c_choice_ptr[(int)index_mat_ptr[num] - 1];
	}

	double mat_min, mat_max;
	cv::minMaxLoc(index_mat, &mat_min, &mat_max);
	index_mat = 255 * index_mat / mat_max;
	index_mat.convertTo(index_mat, CV_8U);

	return index_mat;
}

cv::Mat MyImage::get_hog(cv::Mat mark)
{
	cv::resize(mark, mark, cv::Size(48, 48));
	cv::Mat img_gray, result_descriptor;
	cv::cvtColor(mark, img_gray, CV_BGR2GRAY);

	std::vector< float> descriptorsValues;
	std::vector< cv::Point> locations;

	cv::HOGDescriptor hog_desc(cv::Size(48, 48), cv::Size(8, 8), cv::Size(4, 4), cv::Size(4, 4), 9);
	hog_desc.compute(img_gray, descriptorsValues, cv::Size(0, 0), cv::Size(0, 0), locations);

	cv::Mat row_descriptor(descriptorsValues);

	return row_descriptor.clone();
}

cv::Mat MyImage::get_lbp(cv::Mat mark)
{
	cv::resize(mark, mark, cv::Size(48, 48));
	cv::Mat img_gray;
	cv::cvtColor(mark, img_gray, CV_BGR2GRAY);

	std::vector<int> descriptorsValues;

	cv::Mat row_descriptor(255, 1, CV_32F, cv::Scalar(0));
	float *row_descriptor_ptr = (float*)(row_descriptor.data);

	img_gray.convertTo(img_gray, CV_32F);
	float *img_gray_ptr = (float*)(img_gray.data);

	int temp_lbp = 0;
	float temp_center;

	for (int num = 1; num < img_gray.rows - 1; num++)
	{
		for (int kum = 1; kum < img_gray.cols - 1; kum++)
		{
			temp_center = img_gray_ptr[num* img_gray.cols + kum];

			if (temp_center < img_gray_ptr[(num - 1)* img_gray.cols + kum - 1])
			{
				temp_lbp += pow(2, 0);
			}
			if (temp_center < img_gray_ptr[(num - 1)* img_gray.cols + kum])
			{
				temp_lbp += pow(2, 1);
			}
			if (temp_center < img_gray_ptr[(num - 1)* img_gray.cols + kum + 1])
			{
				temp_lbp += pow(2, 2);
			}
			if (temp_center < img_gray_ptr[(num)* img_gray.cols + kum + 1])
			{
				temp_lbp += pow(2, 3);
			}
			if (temp_center < img_gray_ptr[(num + 1)* img_gray.cols + kum + 1])
			{
				temp_lbp += pow(2, 4);
			}
			if (temp_center < img_gray_ptr[(num + 1)* img_gray.cols + kum])
			{
				temp_lbp += pow(2, 5);
			}
			if (temp_center < img_gray_ptr[(num + 1)* img_gray.cols + kum - 1])
			{
				temp_lbp += pow(2, 6);
			}
			if (temp_center < img_gray_ptr[(num)* img_gray.cols + kum - 1])
			{
				temp_lbp += pow(2, 7);
			}

			row_descriptor_ptr[temp_lbp] += 1;
			temp_lbp = 0;
		}
	}

	return row_descriptor.clone();
}

cv::Rect MyImage::enlarg_rect_to(cv::Rect rect, int size)
{
	if (rect.area() < (double)pow(size, 2))
	{
		cv::Rect larg_rect;
		double img_height_diff = size - rect.height;
		double img_width_diff = size - rect.width;

		larg_rect.x = (int)round(rect.x - img_width_diff / 2);
		larg_rect.y = (int)round(rect.y - img_height_diff / 2);
		larg_rect.height = size;
		larg_rect.width = size;


		return larg_rect;
	}
	else
	{
		return rect;
	}

}

cv::Rect MyImage::enlarg_rect_to_predict(cv::Rect rect, int size, cv::Size img_size)
{
	if (rect.area() < (double)pow(size, 2))
	{
		cv::Rect larg_rect;
		double img_height_diff = size - rect.height;
		double img_width_diff = size - rect.width;

		larg_rect.x = (int)round(rect.x - img_width_diff / 2);
		larg_rect.y = (int)round(rect.y - img_height_diff / 2);
		larg_rect.height = size;
		larg_rect.width = size;

		if (larg_rect.x < 0)
		{
			larg_rect.x = 0;
		}
		if (larg_rect.y < 0)
		{
			larg_rect.y = 0;
		}
		if (larg_rect.height > img_size.height)
		{
			larg_rect.height = img_size.height;
		}
		if (larg_rect.width > img_size.width)
		{
			larg_rect.width = img_size.width;
		}

		return larg_rect;
	}
	else
	{
		return rect;
	}

}

void MyImage::post_processing(cv::Mat cv_img, MyImage& my_image)
{

	// HAIR START
	cv::Mat union_result(cv_img.size(), CV_8UC1, cv::Scalar(1));

	// ELements 
	cv::Mat element_0(cv::Size(1, 6), CV_8UC1, cv::Scalar(1));
	element_0.at<uchar>(0) = 0;
	element_0.at<uchar>(5) = 0;
	cv::Mat element_90 = element_0.t();
	cv::Mat element_45 = cv::Mat::eye(cv::Size(5, 5), CV_8UC1);
	element_45.at<uchar>(0, 0) = 0;
	element_45.at<uchar>(4, 4) = 0;

	cv::Mat cv_img_copy;
	cv_img.convertTo(cv_img_copy, CV_32F);
	std::vector<cv::Mat> chanels;
	split(cv_img_copy, chanels);

	for (int num = 0; num < chanels.size(); num++)
	{
		cv::Mat chanel = chanels[num];

		cv::Mat result_0, result_45, result_90, result(chanel.size(), CV_32F);

		cv::morphologyEx(chanel, result_0,
			cv::MORPH_CLOSE, element_0, cv::Point(-1, -1), 1);
		cv::morphologyEx(chanel, result_45,
			cv::MORPH_CLOSE, element_45, cv::Point(-1, -1), 1);
		cv::morphologyEx(chanel, result_90,
			cv::MORPH_CLOSE, element_90, cv::Point(-1, -1), 1);

		float *result_0_ptr = (float*)(result_0.data);
		float *result_45_ptr = (float*)(result_45.data);
		float *result_90_ptr = (float*)(result_90.data);
		float *result_ptr = (float*)(result.data);

		for (int num = 0; num < chanel.cols*chanel.rows; num++)
		{
			result_ptr[num] = result_0_ptr[num];
			if (result_45_ptr[num] > result_ptr[num])
			{
				result_ptr[num] = result_45_ptr[num];
			}

			if (result_90_ptr[num] >result_ptr[num])
			{
				result_ptr[num] = result_90_ptr[num];
			}
		}

		cv::Mat hair = cv::abs(chanel - result);

		double min, max;
		cv::minMaxLoc(hair, &min, &max);

		double threshold_mask = 0.02;
		cv::Mat hair_mask;
		cv::threshold(hair, hair_mask, max*threshold_mask, 1, cv::THRESH_BINARY);

		hair_mask.convertTo(hair_mask, CV_8UC1);

		cv::bitwise_and(union_result, hair_mask, union_result);
	}

	// HAIR END

	// Eleminate fals candidates

	cv::Mat gray_img;
	cv::cvtColor(cv_img, gray_img, CV_BGR2GRAY);

	cv::Mat target_hair, target_blob;
	std::vector<cv::KeyPoint> keypoints;
	double threshold_target = 0.10;
	
	// Hair
	for (int num = 0; num < this->candidates.size(); num++)
	{
		target_hair = union_result(this->enlarg_rect_to(this->candidates[num],30));

		int hair_count = cv::countNonZero(target_hair);

		if (hair_count >(int)target_hair.rows*target_hair.cols*threshold_target)
		{
			this->candidates.erase(this->candidates.begin() + num);
			num--;
		}
	}

	// Blob
	for (int num = 0; num < this->candidates.size(); num++)
	{
		target_blob = gray_img(this->enlarg_rect_to(this->candidates[num],30));
		this->blob_detector->detect(target_blob, keypoints);

		if (keypoints.size() < 1)
		{
			this->candidates.erase(this->candidates.begin() + num);
			num--;
		}

		keypoints.clear();
	}

	// Size
	for (int num = 0; num < this->candidates.size(); num++)
	{

		if (candidates[num].area() > 1000 || candidates[num].area() < 20)
		{
			this->candidates.erase(this->candidates.begin() + num);
			num--;
		}

	}
}

void MyImage::validate_process_image(MyImage& my_image, MyImage record)
{
	std::string name = my_image.get_name(1); 
	cv::Mat cv_img = cv::imread(my_image.get_name(2) + "\\" + my_image.get_name(1)); // load input image

	my_image.img_size = cv_img.size();

	// Light normalization
	cv::Mat img_light = badger_process(cv_img);

	my_image.detect_faces(img_light); // Face detection
	my_image.detect_landmarks(img_light); // Landmark detection

	my_image.calc_angle();
	cv::Mat rotated_img = my_image.rotate_img(img_light); //Rotate image 
	my_image.landmark_vec = my_image.rotate_points(my_image.landmark_vec);

	//Segmentation mask
	std::pair<cv::Mat, cv::Mat> segmentation = my_image.face_segment(rotated_img, my_image.landmark_vec, true);

	// Resize factor 
	cv::Point middle_1 = my_image.landmark_vec[36] + (my_image.landmark_vec[39] - my_image.landmark_vec[36]) / 2;
	cv::Point middle_2 = my_image.landmark_vec[42] + (my_image.landmark_vec[45] - my_image.landmark_vec[42]) / 2;
	float resize_factor = (float)500 / (float)(middle_2.x - middle_1.x);
	my_image.resize_factor = resize_factor;
	
	// Resize image 
	cv::Mat img_resize;
	cv::Mat img_resize_mask;
	cv::resize(rotated_img, img_resize, cv::Size(rotated_img.size().width*resize_factor, rotated_img.size().height*resize_factor), 0, 0, CV_INTER_CUBIC);
	cv::resize(segmentation.first, img_resize_mask, cv::Size(segmentation.first.size().width*resize_factor, segmentation.first.size().height*resize_factor), 0, 0, CV_INTER_NN);
	my_image.rezize_points(my_image);

	img_resize_mask.setTo(255, img_resize_mask > 125);
	img_resize_mask.setTo(0, img_resize_mask <= 125);

	record.resize_factor = my_image.resize_factor;
	record.rotate_angle = my_image.rotate_angle;
	record.rezize_points(record);
	record.img_size = my_image.img_size;
	record.point1_vec = record.rotate_points(record.point1_vec);
	record.point2_vec = record.rotate_points(record.point2_vec);

	cv::Mat frs = my_image.FRS(img_resize);

	// FRS tr�skelv�rde
	double frs_tresh = 0.10;

	// Get candidates
	std::pair<std::vector<std::vector<cv::Point>>, std::vector<cv::Rect>> candidates = my_image.get_candidates(img_resize, img_resize_mask, frs, frs_tresh);

	my_image.candidates = candidates.second;

	cv_img = cv::imread(my_image.get_name(2) + "\\" + my_image.get_name(1));
	cv::resize(cv_img, cv_img, cv::Size(cv_img.size().width*my_image.resize_factor, cv_img.size().height*my_image.resize_factor), 0, 0, CV_INTER_CUBIC);
	cv_img = my_image.rotate_img(cv_img);

	cv::Mat resulting = cv_img.clone(); 

	my_image.post_processing(img_resize, my_image);

	my_image.intersecting_areas(record, my_image);

	record.draw_record(resulting);

	cv::Mat predicton_data = this->predict_data(cv_img, this->candidates);
	cv::Mat predictions;

	this->image_svm->predict(predicton_data, predictions);

	// Display
	cv::Scalar mark_color;
	float *predictions_ptr = (float*)(predictions.data);
	for (int num = 0; num < this->candidates.size(); num++)
	{
		if (predictions_ptr[num] == 1)
		{
			mark_color = cv::Scalar(255, 124, 124);
		}
		else
		{
			mark_color = cv::Scalar(124, 124, 255);
		}

		cv::Point center_of_candidate;

		center_of_candidate = my_image.candidates[num].tl();
		center_of_candidate.x = center_of_candidate.x + round(my_image.candidates[num].width/2);
		center_of_candidate.y = center_of_candidate.y + round(my_image.candidates[num].height / 2);

		//cv::rectangle(resulting, enlarg_rect_to_predict(my_image.candidates[num], 25, resulting.size()), mark_color, 4, 8, 0);
		cv::circle(resulting, center_of_candidate, 15, mark_color, 4, 8, 0);
	}

	//// Markering f�r de omr�den som �verlappar mellan �kta m�rken och kandidater 
	//for (int rum : my_image.index_checked_detections)
	//{
	//	cv::rectangle(resulting, my_image.candidates[rum], cv::Scalar(0, 255, 0), 4, 8, 0);
	//}

	///////////////////////// Bra st�lle att s�tta en breakpoint d� man kan se alla bilder ImageWatch
	cv::imshow("MainWindow", resulting);
	
	my_image.candidates.clear();
	my_image.index_checked_detections.clear();

	
}

void MyImage::process_image(MyImage& my_image)
{

	cv::Mat cv_img = cv::imread(my_image.get_name(2) + "\\" + my_image.get_name(1)); // load input image
	my_image.img_size = cv_img.size();

	// Light normalization
	cv::Mat img_light = badger_process(cv_img);
	//cv::Mat img_light = MSR_process(cv_img);
	//cv::Mat img_light = cv_img;

	my_image.detect_faces(img_light); // Face detection
	my_image.detect_landmarks(img_light); // Landmark detection
	
	my_image.calc_angle();
	cv::Mat rotated_img = my_image.rotate_img(img_light); //Rotate image 
	my_image.landmark_vec = my_image.rotate_points(my_image.landmark_vec);

	//Segmentation mask
	std::pair<cv::Mat, cv::Mat> segmentation = my_image.face_segment(rotated_img, my_image.landmark_vec, true);

	// Resize factor 
	cv::Point middle_1 = my_image.landmark_vec[36] + (my_image.landmark_vec[39] - my_image.landmark_vec[36]) / 2;
	cv::Point middle_2 = my_image.landmark_vec[42] + (my_image.landmark_vec[45] - my_image.landmark_vec[42]) / 2;
	float resize_factor = (float)500 / (float)(middle_2.x - middle_1.x);
	my_image.resize_factor = resize_factor;
	
	// Resize image 
	cv::Mat img_resize;
	cv::Mat img_resize_mask;
	cv::resize(rotated_img, img_resize, cv::Size(rotated_img.size().width*resize_factor, rotated_img.size().height*resize_factor), 0, 0, CV_INTER_CUBIC);
	cv::resize(segmentation.first, img_resize_mask, cv::Size(segmentation.first.size().width*resize_factor, segmentation.first.size().height*resize_factor), 0, 0, CV_INTER_NN);
	my_image.rezize_points(my_image);
	
	img_resize_mask.setTo(255, img_resize_mask > 125);
	img_resize_mask.setTo(0, img_resize_mask <= 125);
	
	cv::Mat frs = my_image.FRS(img_resize);

	// Get candidates
	std::pair<std::vector<std::vector<cv::Point>>, std::vector<cv::Rect>> candidates = my_image.get_candidates(img_resize, img_resize_mask, frs, 0.11);

	my_image.candidates = candidates.second; 

	my_image.post_processing(img_resize, my_image);

	// Classify
	cv_img = cv::imread(my_image.get_name(2) + "\\" + my_image.get_name(1));
	cv_img = my_image.rotate_img(cv_img);
	cv::resize(cv_img, cv_img, cv::Size(cv_img.size().width*my_image.resize_factor, cv_img.size().height*my_image.resize_factor), 0, 0, CV_INTER_CUBIC);
	
	cv::Mat predicton_data = this->predict_data(cv_img, this->candidates);
	cv::Mat predictions; 

	this->image_svm->predict(predicton_data, predictions);

	std::vector<int> v = predictions.reshape(0, 1);
	my_image.predictions = v; 
	
	// Frequency
	my_image.create_areas();
	my_image.calc_freq(my_image);

	// Display
	cv::Scalar mark_color;
	float *predictions_ptr = (float*)(predictions.data);
	for (int num = 0; num < this->candidates.size(); num++)
	{
		if (predictions_ptr[num] == 1)
		{
			mark_color = cv::Scalar(255, 0, 0);
		}
		else
		{
			mark_color = cv::Scalar(0, 0, 255);
		}
		cv::rectangle(img_resize, enlarg_rect_to_predict(this->candidates[num], 25, img_resize.size()), mark_color, 4, 8, 0);
	}

	///////////////////////// Bra st�lle att s�tta en breakpoint d� man kan se alla bilder ImageWatch
	cv::imshow("MainWindow", img_resize);

}

void MyImage::calc_freq(MyImage& my_imgae)
{
	cv::Rect temp_rect;
	cv::Rect inter_rect;
	my_imgae.frequency = std::vector<int>(my_imgae.areas_vec.size());
	my_imgae.predictions_1 = std::vector<int>(my_imgae.areas_vec.size());
	my_imgae.predictions_2 = std::vector<int>(my_imgae.areas_vec.size());

	for (int num = 0; num < my_imgae.areas_vec.size(); num++)
	{
		temp_rect = cv::boundingRect(my_imgae.areas_vec[num]);

		for (int kum = 0; kum < my_imgae.candidates.size(); kum++)
		{
			inter_rect = temp_rect & my_imgae.candidates[kum];
			if (inter_rect.area() > 0)
			{
				my_imgae.frequency[num]++;

				if (my_imgae.predictions[kum] == 1)
				{
					my_imgae.predictions_1[num]++;
				}
				else
				{
					my_imgae.predictions_2[num]++;
				}

			}
		}

	}

}
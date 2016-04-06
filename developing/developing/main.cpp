#include "Header.h"



std::vector<cv::Mat> get_color_chanels(cv::Mat img)
{
	cv::Mat ch1, ch2, ch3;
	// "channels" is a vector of 3 Mat arrays:
	std::vector<cv::Mat> channels(3);
	// split img:
	split(img, channels);
	ch1 = channels[0];
	ch2 = channels[1];
	ch3 = channels[2];

	return channels;
}

cv::Rect enlarg_face_rect(cv::Rect face, double procent, cv::Size img_size)
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

cv::Mat close_open(cv::Mat mask, int iterations = 7, int kernels_size = 5)
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

std::pair<cv::Mat, cv::Mat> face_segment(cv::Mat cv_img,  std::vector<cv::Point> landmark_vec, bool down_sample)
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

	cv::RotatedRect eye_rec_1 = cv::fitEllipse(eye_1_vec);
	cv::RotatedRect eye_rec_2 = cv::fitEllipse(eye_2_vec);
	cv::RotatedRect mouth_rec = cv::fitEllipse(mouth_vec);
	cv::approxPolyDP(lower_face_vec, lower_face_roi, 1.0, true);

	cv::fillConvexPoly(mask, &lower_face_roi[0], lower_face_roi.size(), cv::Scalar(cv::GC_PR_FGD), 8, 0);
	cv::ellipse(mask, eye_rec_1, cv::Scalar(cv::GC_BGD), -1);
	cv::ellipse(mask, eye_rec_2, cv::Scalar(cv::GC_BGD), -1);
	cv::ellipse(mask, mouth_rec, cv::Scalar(cv::GC_BGD), -1);
	
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


		// Generate output image
		end_mask = close_open(resultUp);
		cv_img.copyTo(foreground, end_mask); // bg pixels not copied

		cv::namedWindow("one", CV_WINDOW_KEEPRATIO);
		cv::imshow("one", end_mask);
		cv::namedWindow("two", CV_WINDOW_KEEPRATIO);
		cv::imshow("two", resultUp);
	}
	else
	{
		cv::grabCut(cv_img, mask, cv::Rect(), bgModel, fgModel, 1, cv::GC_INIT_WITH_MASK);

		cv::Mat mask_fg = (mask == cv::GC_FGD) | (mask == cv::GC_PR_FGD);
		end_mask = close_open(mask_fg);
		cv_img.copyTo(foreground, end_mask); // bg pixels not copied

		cv::namedWindow("one", CV_WINDOW_KEEPRATIO);
		cv::imshow("one", end_mask);
		cv::namedWindow("two", CV_WINDOW_KEEPRATIO);
		cv::imshow("two", mask_fg);
	}

	return std::make_pair(end_mask, foreground);
}

double calc_angle(cv::Point a, cv::Point b, cv::Point c)
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

cv::Mat rotate_img(cv::Mat orig_img, std::vector<cv::Point> landmark_vec)
{
	cv::Mat rotated_img;

	cv::Point left_eye_1 = landmark_vec[36];
	cv::Point left_eye_2 = landmark_vec[39];
	cv::Point right_eye_1 = landmark_vec[42];
	cv::Point right_eye_2 = landmark_vec[45];
	cv::Point straight_point_1(left_eye_1.x + right_eye_1.x, left_eye_1.y);
	cv::Point straight_point_2(left_eye_2.x + right_eye_2.x, left_eye_2.y);

	double angle_1 = calc_angle(left_eye_1, straight_point_1, right_eye_1);
	double angle_2 = calc_angle(left_eye_2, straight_point_2, right_eye_2);
	double angle = (angle_1 + angle_2) / 2;

	cv::Point2f pt(orig_img.cols / 2., orig_img.rows / 2.);
	cv::Mat r = getRotationMatrix2D(pt, -angle, 1.0);
	warpAffine(orig_img, rotated_img, r, cv::Size(orig_img.cols, orig_img.rows));


	return rotated_img;
}

cv::Point RotatePoint(cv::Point cen_pt, cv::Point p, float angle)
{
	float rand = M_PI*(angle / 180);
	float s = sin(rand);
	float c = cos(rand);

	// translate point back to origin:
	p.x -= cen_pt.x;
	p.y -= cen_pt.y;

	// rotate point
	float xnew = p.x * c - p.y * s;
	float ynew = p.x * s + p.y * c;

	// translate point back:
	p.x = xnew + cen_pt.x;
	p.y = ynew + cen_pt.y;
	return p;
}



int main(){
	//--------------------------------------------------------------------------------------------------------------------------------
	//cv::Mat cv_img = cv::imread("C:\\Users\\NFCexjobb\\Desktop\\003_frontal.JPG");
	cv::Mat cv_img = cv::imread("C:\\Users\\Stubborn\\Desktop\\04423d02.JPG");
	cv::CascadeClassifier face_Cascade;
	dlib::shape_predictor model_dlib;
	face_Cascade.load("haarcascade_frontalface_alt.xml");
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> model_dlib;
	// Face detector parameters
	CvSize minFeatureSize = cvSize(200, 200);	 // Smallest face size.
	int flags = CV_HAAR_DO_CANNY_PRUNING;
	float search_scale_factor = 1.1f;	// How detailed should the search be.
	std::vector<cv::Rect>faces;
	// Detect faces - OPENCV
	face_Cascade.detectMultiScale(cv_img, faces, search_scale_factor, 2, 0 | flags, minFeatureSize);

	 
	std::vector<cv::Point> landmark_vec;

	//Convert from mat to dlib
	dlib::array2d<dlib::rgb_pixel> img_dlib;
	dlib::assign_image(img_dlib, dlib::cv_image<dlib::bgr_pixel>(cv_img));

	// Vector with landmarks 

	dlib::rectangle temp_face((long)faces[0].tl().x, (long)faces[0].tl().y, (long)faces[0].br().x - 1, (long)faces[0].br().y - 1);
	dlib::full_object_detection shape = model_dlib(img_dlib, temp_face); // Find landmark in bounding box
	for (int i = 0; i < shape.num_parts(); i++)
	{
		landmark_vec.push_back(cv::Point(shape.part(i).x(), shape.part(i).y()));
	}
	//--------------------------------------------------------------------------------------------------------------------------------


		cv::Mat rotated_img;

		cv::Point left_eye_1 = landmark_vec[36];
		cv::Point left_eye_2 = landmark_vec[39];
		cv::Point right_eye_1 = landmark_vec[42];
		cv::Point right_eye_2 = landmark_vec[45];
		cv::Point straight_point_1(left_eye_1.x + right_eye_1.x, left_eye_1.y);
		cv::Point straight_point_2(left_eye_2.x + right_eye_2.x, left_eye_2.y);

		double angle_1 = calc_angle(left_eye_1, straight_point_1, right_eye_1);
		double angle_2 = calc_angle(left_eye_2, straight_point_2, right_eye_2);
		double angle = (angle_1 + angle_2) / 2;

		cv::Point2f pt(cv_img.cols / 2., cv_img.rows / 2.);
		cv::Mat r = getRotationMatrix2D(pt, -angle, 1.0);
		warpAffine(cv_img, rotated_img, r, cv::Size(cv_img.cols, cv_img.rows));
		
		std::vector<cv::Point> rotated_landmarks; 
		for (int i = 0; i < landmark_vec.size(); i++)
		{
			cv::Point temp_rot = RotatePoint(pt, landmark_vec[i], angle);
			rotated_landmarks.push_back(temp_rot);
		}

		for (int i = 0; i < landmark_vec.size(); i++)
		{
			cv::circle(cv_img, landmark_vec[i], 4, CV_RGB(0, 0, 255), CV_FILLED);
		}

		for (int i = 0; i < rotated_landmarks.size(); i++)
		{
			cv::circle(rotated_img, rotated_landmarks[i], 4, CV_RGB(0, 0, 255), CV_FILLED);
		}
		cv::Point temp = RotatePoint(pt, cv::Point(faces[0].x, faces[0].y),angle);
		cv::Rect rotated_face(temp.x, temp.y, faces[0].width, faces[0].height);

		cv::rectangle(cv_img, faces[0], CV_RGB(0, 0, 255), 2);
		cv::rectangle(rotated_img, rotated_face, CV_RGB(0, 0, 255), 2);
	
	//std::pair<cv::Mat, cv::Mat> segmentation = face_segment(cv_img, landmark_vec, false);
	
	// show it
	//cv::namedWindow("test", CV_WINDOW_KEEPRATIO);
	//cv::imshow("test", rotated_img);
	cv::namedWindow("test", CV_WINDOW_KEEPRATIO);
	cv::imshow("test", rotated_img);
	cv::namedWindow("orig", CV_WINDOW_KEEPRATIO);
	cv::imshow("orig", cv_img);
	cv::waitKey(0);

	//--------------------------------------------
	
	////cv::line(cv_img, landmark_vec[0], landmark_vec[3], CV_RGB(255, 0, 0), 2);
	//for (int i = 0; i < landmark_vec.size(); i++)
	//{
	//	cv::circle(cv_img, landmark_vec[i], 4, CV_RGB(0, 0, 255), CV_FILLED);
	//}
		
	//cv::Mat hsv_img;
	//cv::cvtColor(cv_img, hsv_img, CV_BGR2HSV);
	//std::vector<cv::Mat> hsv_chanels = get_color_chanels(hsv_img);
	
	///*
	//cv::Mat cv_gray, blur, result;
	//cv::cvtColor(cv_img, cv_gray, CV_BGR2GRAY);
	//cv::GaussianBlur(cv_gray, blur, cv::Size(11, 11), 0, 0);
	//cv::threshold(blur, result, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);
	//*/
	
	////cv::cvtColor(cv_img, result, cv::COLOR_BGR2HSV);
	////cv::rectangle(cv_img, face, CV_RGB(255, 0, 0), 2);
	
	//cv::namedWindow("one", CV_WINDOW_KEEPRATIO);
	//cv::imshow("one", hsv_chanels[0]);
	//cv::namedWindow("two", CV_WINDOW_KEEPRATIO);
	//cv::imshow("two", hsv_chanels[1]);
	//cv::namedWindow("three", CV_WINDOW_KEEPRATIO);
	//cv::imshow("three", hsv_chanels[2]);
	//cv::waitKey(0);
	


	return 0;
}

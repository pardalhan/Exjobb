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

cv::Rect enlarg_rect(cv::Rect face, double procent)
{

	cv::Rect larg_face;
	double img_height_diff = face.height*(1 + procent) - face.height;
	double img_width_diff = face.width*(1 + procent) - face.width;

	larg_face.x = (int)round(face.x - img_width_diff / 2);
	larg_face.y = (int)round(face.y - img_height_diff / 2);
	larg_face.height = (int)round(face.height*(1 + procent));
	larg_face.width = (int)round(face.width*(1 + procent));


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

void RotatePoint(cv::Mat img, MyImage& my_image)
{

	cv::Point2f cen_pt(img.cols / 2., img.rows / 2.);
	float rand = M_PI*(my_image.rotate_angle / 180);
	float s = sin(rand);
	float c = cos(rand);

	for (int num = 0; num < my_image.landmark_vec.size(); num++)
	{

	// translate point back to origin:
	my_image.landmark_vec[num].x -= cen_pt.x;
	my_image.landmark_vec[num].y -= cen_pt.y;

	// rotate point
	float xnew = my_image.landmark_vec[num].x * c - my_image.landmark_vec[num].y * s;
	float ynew = my_image.landmark_vec[num].x * s + my_image.landmark_vec[num].y * c;

	// translate point back:
	my_image.landmark_vec[num].x = xnew + cen_pt.x;
	my_image.landmark_vec[num].y = ynew + cen_pt.y;
	}
}

cv::Mat FRS(cv::Mat cv_img)
{
	int num_row = cv_img.rows;
	int num_col = cv_img.cols;
	int sigma = 0;
	double k_scal = 9.9;
	double alpha = 2;
	double thresh = 0.001;
	std::vector<int> radii = { 2, 3, 4, 5 ,7 ,9 , 11, 13};
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
				if (grad_pix > maxG*thresh)
				{
				pos_pix.x += round((grad_x_ptr[grad_x.cols * row + col] / grad_pix) * radius);
				pos_pix.y += round((grad_y_ptr[grad_y.cols * row + col] / grad_pix) * radius);
				neg_pix.x -= round((grad_x_ptr[grad_x.cols * row + col] / grad_pix) * radius);
				neg_pix.y -= round((grad_y_ptr[grad_y.cols * row + col] / grad_pix) * radius);

				magnitude_ptr[magnitude.cols * (pos_pix.y + radius) + (pos_pix.x + radius)] += grad_pix;
				magnitude_ptr[magnitude.cols * (neg_pix.y + radius) + (neg_pix.x + radius)] -= grad_pix;
				orientation_ptr[magnitude.cols * (pos_pix.y+ radius) + (pos_pix.x + radius)] += 1;
				orientation_ptr[magnitude.cols * (neg_pix.y + radius) + (neg_pix.x + radius)] -= 1;
				}
			}
		}


		// Manipulate the orientation and magnitude 
		orientation = cv::abs(orientation); // DETTA SKILJER SIG FRÅN ORGINAL TEORIN BORDE VARA EFTER
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
	
	/*
	int num_row = cv_img.rows;
	int num_col = cv_img.cols;
	int sigma = 1;
	double k_scal = 9.9;
	double alpha = 2;
	double thresh = 0.1;
	std::vector<int> radii = { 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	cv::Mat gradient, magnitude, orientation, F_img, S_img;
	cv::Mat S_total = cv::Mat::zeros(num_row, num_col, CV_32F);
	//--------------------------------- gradient 
	cv::Mat img_gray, blurred;
	cv::GaussianBlur(cv_img, blurred, cv::Size(3, 3), sigma, sigma, cv::BORDER_DEFAULT);
	/// Convert it to gray
	cv::cvtColor(blurred, img_gray, CV_BGR2GRAY);

	cv::Mat grad_x, grad_y;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_32F;
	/// Gradient X
	Sobel(img_gray, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT);

	Sobel(img_gray, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT);



	for (int rad = 0; rad < radii.size(); rad++)
	{

		gradient = cv::Mat(num_row, num_col, CV_32F, cv::Scalar(0.00001));
		magnitude = cv::Mat::zeros(num_row + radii[rad] * 2, num_col + radii[rad] * 2, CV_32F);
		orientation = cv::Mat::zeros(num_row + radii[rad] * 2, num_col + radii[rad] * 2, CV_32F);
		F_img = cv::Mat::zeros(num_row + radii[rad] * 2, num_col + radii[rad] * 2, CV_32F);
		S_img = cv::Mat::zeros(num_row + radii[rad] * 2, num_col + radii[rad] * 2, CV_32F);


		float *grad_x_ptr = (float*)(grad_x.data);
		float *grad_y_ptr = (float*)(grad_y.data);
		float *grad_ptr = (float*)(gradient.data);
		float *magnitude_ptr = (float*)(magnitude.data);
		float *orientation_ptr = (float*)(orientation.data);


		for (int row = 0; row < grad_x.rows; row++){
			for (int col = 0; col < grad_x.cols; col++){

				float grad_norm = sqrt(pow(grad_x_ptr[grad_x.cols * row + col], 2) + pow(grad_y_ptr[grad_y.cols * row + col], 2));

				grad_ptr[gradient.cols * row + col] = grad_norm;
			}
		}

		double maxG, minG;
		cv::minMaxLoc(gradient, &minG, &maxG);
		for (int row = 0; row < gradient.rows; row++){
			for (int col = 0; col < gradient.cols; col++){
				cv::Point pos_pix(row, col);
				cv::Point neg_pix(row, col);
				int radius = radii[rad];
				double grad_pix = grad_ptr[gradient.cols * row + col];
				if (grad_pix > maxG*thresh)
				{
					pos_pix.x += round((grad_x_ptr[grad_x.cols * row + col] / grad_pix) * radius);
					pos_pix.y += round((grad_y_ptr[grad_y.cols * row + col] / grad_pix) * radius);
					neg_pix.x -= round((grad_x_ptr[grad_x.cols * row + col] / grad_pix) * radius);
					neg_pix.y -= round((grad_y_ptr[grad_y.cols * row + col] / grad_pix) * radius);

					magnitude_ptr[magnitude.cols * (pos_pix.x + radius) + (pos_pix.y + radius)] += grad_pix;
					magnitude_ptr[magnitude.cols * (neg_pix.x + radius) + (neg_pix.y + radius)] -= grad_pix;
					orientation_ptr[magnitude.cols * (pos_pix.x + radius) + (pos_pix.y + radius)] += 1;
					orientation_ptr[magnitude.cols * (neg_pix.x + radius) + (neg_pix.y + radius)] -= 1;
				}
			}
		}


		// Manipulate the orientation and magnitude 
		cv::Mat orientation_thresh;
		cv::abs(orientation);
		orientation.convertTo(orientation, CV_8U);
		cv::threshold(orientation, orientation_thresh, k_scal, 0, cv::THRESH_TRUNC);

		orientation_thresh = orientation_thresh / k_scal;
		cv::pow(orientation_thresh, alpha, orientation_thresh);
		magnitude = magnitude / k_scal;

		orientation.convertTo(orientation_thresh, CV_32F);


		// Calculate F
		F_img = magnitude.mul(orientation_thresh);

		// Calculate S
		cv::GaussianBlur(F_img, S_img, cv::Size(3, 3), sigma, sigma, cv::BORDER_DEFAULT);

		cv::Rect roi(radii[rad] - 1, radii[rad] - 1, num_col, num_row);


		S_total += S_img(roi);
	}

	S_total = S_total / radii.size();
	*/


	return S_total;
}

std::pair<std::vector<std::vector<cv::Point>>, std::vector<cv::Rect>> get_candidates(cv::Mat cv_img, cv::Mat mask )
{

	cv::Mat frs_img = FRS(cv_img);

	frs_img = cv::abs(frs_img);
	double max, min;
	cv::minMaxLoc(frs_img, &min, &max);

	/*
	// OTSU THRESHOLDNING
	frs_img.convertTo(frs_img, CV_8U);
	cv::threshold(frs_img, frs_img, 0, 1, cv::THRESH_BINARY | CV_THRESH_OTSU);

	cv::Mat const structure_elem = cv::getStructuringElement(
	cv::MORPH_CROSS, cv::Size(3, 3));
	cv::morphologyEx(frs_img, frs_img,
	cv::MORPH_CLOSE, structure_elem, cv::Point(-1, -1), 1);

	cv::morphologyEx(frs_img, frs_img,
	cv::MORPH_ERODE, structure_elem, cv::Point(-1, -1), 2);
	*/


	// NORMAL THRESHOLDING
	double threshold = 0.08;
	cv::threshold(frs_img, frs_img, threshold*max, 1, cv::THRESH_BINARY);

	// Remove seed outside mask 
	frs_img.setTo(0, mask == 0);

	// Draw the background markers
	cv::Mat background;

	cv::Mat const structure_elem = cv::getStructuringElement(
		cv::MORPH_RECT, cv::Size(3, 3));
	cv::morphologyEx(frs_img, background,
		cv::MORPH_DILATE, structure_elem, cv::Point(-1, -1), 10);

	cv::threshold(background, background, 0, 1337, cv::THRESH_BINARY_INV);

	cv::Mat back_32s;
	background.convertTo(back_32s, CV_32SC1);


	// Create the CV_8U version of the distance image
	// It is needed for findContours()
	cv::Mat dist_8u;
	frs_img.convertTo(dist_8u, CV_8U);


	// Find seed counters
	std::vector<std::vector<cv::Point>> contours;
	findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	// Draw the foreground markers
	cv::Mat markers_32s = cv::Mat::zeros(frs_img.size(), CV_32SC1);
	for (size_t i = 0; i < contours.size(); i++)
		drawContours(markers_32s, contours, static_cast<int>(i), cv::Scalar::all(static_cast<int>(i)+1), -1);

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

std::vector<MyImage> extract_from_record(std::string file_path)
{
	MyImage img;
	std::vector<MyImage> image_vec;
	std::string line, part;
	int counter = 0;
	int temp_int;
	std::vector<int> temp_vec;

	std::ifstream record_file(file_path);

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
				img.set_name(line, 1);
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

std::vector<int> extract_lables(std::vector<MyImage> record)
{
	std::vector<int> all_labels;
	for (int i = 0; i < record.size(); i++)
	{
		all_labels.insert(all_labels.end(), record[i].class_type_vec.begin(), record[i].class_type_vec.end());
	}

	return all_labels;
}

cv::Mat extract_features(std::vector<MyImage> record)
{
	cv::Mat curr_img;
	cv::Mat mark;

	std::vector<int> all_labels = extract_lables(record);

	int nr_features = 6;
	int nr_samples = all_labels.size();

	cv::Mat training_data(nr_samples, nr_features, CV_32FC1);
	float *training_data_ptr = (float*)(training_data.data);


	int temp_int = 0;
	for (int num = 0; num < record.size(); num++)
	{
		curr_img = cv::imread("C:\\Users\\Stubborn\\Dropbox\\Exjobb\\ExjobbBilder\\" + record[num].get_name(1));

		for (int kum = 0; kum < record[num].point1_vec.size(); kum++)
		{
			mark = curr_img(cv::Rect(record[num].point1_vec[kum], record[num].point2_vec[kum]));
			cv::Scalar mean;
			cv::Scalar stdvar;
			cv::meanStdDev(mark, mean, stdvar);
			for (int i = 0; i < 3; i++)
			{
				training_data_ptr[nr_features * kum + nr_features * temp_int + i] = mean[i];
			}
			for (int i = 0; i < 3; i++)
			{
				training_data_ptr[nr_features * kum + nr_features * temp_int + 3 + i] = stdvar[i];
			}

			//rectangle(curr_img, record.first[i].point1_vec[k], record.first[i].point2_vec[k], cv::Scalar(255, 255 * (record.first[i].class_type_vec[k] - 1), 0), 2, 8, 0);
		}
		temp_int += record[num].point1_vec.size();
	}
	return training_data;
}

cv::Ptr<cv::ml::SVM> train_SVM(std::string file_path)
{

	std::vector<MyImage> record = extract_from_record(file_path);

	cv::Mat training_data = extract_features(record);
	std::vector<int> all_labels = extract_lables(record);
	cv::Mat label_Mat(all_labels);

	// Train the SVM
	cv::Ptr<cv::ml::SVM> my_svm = cv::ml::SVM::create();
	my_svm->setType(cv::ml::SVM::C_SVC);
	my_svm->setKernel(cv::ml::SVM::RBF);  // OBSERVERA FUNKAR INTE ATT LOADA NON-LINEAR KERNELS //
	my_svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER, 100, 1e-6));
	my_svm->train(training_data, cv::ml::ROW_SAMPLE, label_Mat);
	my_svm->save("C:\\Users\\Stubborn\\Desktop\\WORK\\MODEL.xml");


	cv::Ptr<cv::ml::SVM> new_svm = cv::ml::SVM::create();
	new_svm = cv::ml::SVM::load<cv::ml::SVM>("C:\\Users\\Stubborn\\Desktop\\WORK\\MODEL.xml");

	return new_svm;
}

cv::Mat calc_confusion(cv::Mat predicted, cv::Mat actual) 
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

// NEEDED TO DRAW INTERSECTING AREA
cv::Point center_of_mass(std::vector<cv::Point> contour)
{
	cv::Point center_mass(0, 0);
	for (int num = 0; num < contour.size(); num++)
	{
		center_mass = center_mass + contour[num];
	}
	center_mass = cv::Point(round(center_mass.x / contour.size()), round(center_mass.y / contour.size()));

	return center_mass;
}

struct MySort {
	MySort(cv::Point center_mass) { this->center_mass = center_mass; }
	bool operator()(const cv::Point p1, const cv::Point p2) 
	{ 
		double alpha1 = atan2(p1.y - center_mass.y, p1.x - center_mass.x);
		double alpha2 = atan2(p2.y - center_mass.y, p2.x - center_mass.x);
		return alpha1 < alpha2;
	}

	cv::Point center_mass;

	/*
	std::sort(foo.begin(), foo.end(), MySort(center_of_mass(foo)));

	for (int num = 0; num < countour_vec.size(); num++)
	{
	cv::drawContours(output, countour_vec, num, cv::Scalar(num*125, 0, 255), -1);
	}
	*/
};
// NEEDED TO DRAW INTERSECTING AREA

void intersecting_areas(MyImage& actual, MyImage& predicted)
{

	int width, height;
	float predicted_area, actual_area, inter_area;
	cv::Point center;
	cv::RotatedRect actaul_rect, predicted_rect;
	std::vector<cv::Point> inter_points, predicted_points, actual_points;
	cv::Point2f vertices[4];

	for (int num = 0; num < actual.class_type_vec.size(); num++)
	{
		width = actual.point2_vec[num].x - actual.point1_vec[num].x;
		height = actual.point2_vec[num].y - actual.point1_vec[num].y;
		center = cv::Point(actual.point1_vec[num].x + round(width / 2), actual.point1_vec[num].y + round(height / 2));
		actaul_rect = cv::RotatedRect(center, cv::Size(width, height), 10); // ANGLE HERE
		
		
		actaul_rect.points(vertices);
		actual_points = std::vector<cv::Point>(vertices, vertices + sizeof vertices / sizeof vertices[0]);
		actual_area = cv::contourArea(actual_points);
		
		for (int kum = 0; kum < predicted.class_type_vec.size(); kum++)
		{
			width = predicted.point2_vec[kum].x - predicted.point1_vec[kum].x;
			height = predicted.point2_vec[kum].y - predicted.point1_vec[kum].y;
			center = cv::Point(predicted.point1_vec[kum].x + round(width / 2), predicted.point1_vec[kum].y + round(height / 2));
			predicted_rect = cv::RotatedRect(center, cv::Size(width, height), 0);

			predicted_rect.points(vertices);
			predicted_points = std::vector<cv::Point>(vertices, vertices + sizeof vertices / sizeof vertices[0]);
			predicted_area = cv::contourArea(predicted_points);

			cv::rotatedRectangleIntersection(actaul_rect, predicted_rect, inter_points);

			if (inter_points.size() > 2)
			{
				inter_area = cv::contourArea(inter_points);
				if (inter_area > actual_area*0.5 && predicted_area < actual_area*2)
				{
					predicted.index_checked_detections.push_back(kum);
				}

				inter_points.clear();
				break;
			}

		}
	}
}

void rezize_points(MyImage& my_image)
{
	for (int num = 0; num < my_image.point1_vec.size(); num++)
	{
		my_image.point1_vec[num].x = round(my_image.point1_vec[num].x * my_image.resize_factor);
		my_image.point1_vec[num].y = round(my_image.point1_vec[num].y * my_image.resize_factor);
		my_image.point2_vec[num].x = round(my_image.point2_vec[num].x * my_image.resize_factor);
		my_image.point2_vec[num].y = round(my_image.point2_vec[num].y * my_image.resize_factor);
	}
}

//void calc_freq(MyImage& my_imgae)
//{
//	cv::Mat area_img = cv::Mat::zeros(200, 200, CV_8UC1);
//	cv::Mat rect_img = cv::Mat::zeros(200, 200, CV_8UC1);
//	cv::Mat intersect_img = cv::Mat::zeros(200, 200, CV_8UC1);
//	int area_size;
//
//	std::vector<int> result(contours.size());
//
//	for (int num = 0; num < contours.size(); num++)
//	{
//		cv::drawContours(area_img, contours, num, cv::Scalar(1), -1);
//		for (int kum = 0; kum < rects.size(); kum++)
//		{
//			cv::rectangle(rect_img, rects[0], cv::Scalar(1), -1);
//			cv::bitwise_and(area_img, rect_img, intersect_img);
//			area_size = cv::countNonZero(intersect_img);
//
//			if (area_size > 0)
//			{
//				result[num]++;
//			}
//			rect_img = cv::Scalar(0);
//		}
//		area_img = cv::Scalar(0);
//	}
//
//}


int main(){
	
	cv::Mat w2c;


	cv::FileStorage fsDemo("C:\\Users\\Stubborn\\desktop\\w2c.yml", cv::FileStorage::READ);
	fsDemo["w2c"] >> w2c;

	fsDemo.release();
	/*
	cv::Mat cv_img = cv::imread("C:\\Users\\Stubborn\\Dropbox\\Exjobb\\ExjobbBilder\\030_frontal.JPG");
	cv::Mat mask = cv::imread("C:\\Users\\Stubborn\\Desktop\\WORK\\Cut_imgaes\\mask_img_030_frontal.JPG", CV_LOAD_IMAGE_GRAYSCALE);

	cv::resize(mask, mask, cv_img.size());
	cv::threshold(mask, mask, 10, 1, cv::THRESH_BINARY);

	std::pair<std::vector<std::vector<cv::Point>>, std::vector<cv::Rect>> candidates = get_candidates(cv_img, mask);

	/// Draw contours and bonding rects
	cv::Mat result = cv_img.clone();
	cv::Mat drawing = cv::Mat::zeros(cv_img.size(), CV_8UC3);
	for (int i = 0; i< candidates.first.size(); i++)
	{
		cv::Scalar color = cv::Scalar(cv::theRNG().uniform(0, 255), cv::theRNG().uniform(0, 255), cv::theRNG().uniform(0, 255));
		rectangle(drawing, candidates.second[i].tl(), candidates.second[i].br(), color, 1, 8, 0);
		drawContours(drawing, candidates.first, static_cast<int>(i), cv::Scalar(255, 255, 255), -1);
		drawContours(result, candidates.first, static_cast<int>(i), cv::Scalar(255, 0, 0), -1);
	}
	*/

	/*
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
	
		*/

	/*
	std::string file_path = "C:\\Users\\Stubborn\\Desktop\\WORK\\short.txt";
	
	std::vector<MyImage> record = extract_from_record(file_path);

	cv::Ptr<cv::ml::SVM> my_svm = cv::ml::SVM::create();
	my_svm = train_SVM(file_path);
	cv::Mat training_data = extract_features(record);
	std::vector<int> all_labels = extract_lables(record);
	cv::Mat label_Mat(all_labels);
	cv::Mat predictions;

	my_svm->predict(training_data, predictions);
	
	cv::Mat confusion_matrix = calc_confusion(predictions, label_Mat);
	*/

	/*
	std::string file_path = "C:\\Users\\Stubborn\\Desktop\\WORK\\short.txt";

	std::vector<MyImage> record = extract_from_record(file_path);

	intersecting_areas(record[0], record[0]);

	cv::Mat cv_img = cv::imread("C:\\Users\\Stubborn\\Dropbox\\Exjobb\\ExjobbBilder\\" + record[0].get_name(1));
	*/



	
	return 0;
}

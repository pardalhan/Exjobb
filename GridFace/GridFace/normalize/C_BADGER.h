#include <cstdio>
#include <cmath>

#if defined(_WIN32) || defined(_WIN64)
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#endif

#define CV_LOAD_IMAGE_ANYDEPTH 2
#define CV_LOAD_IMAGE_ANYCOLOR 4
#define COLOR_BGR2GRAY 6

/**
Filters an image with double precision data using an averagig kernel of given size.

@param[in]	img Image to be filtered.
@param[out] result The filtered image.
@param[in]	k Averaging kernel size.
*/
void Filter64F(cv::Mat img, cv::Mat &result, int k);

/**
Creates random sprays that are used for determining the neighbourhood.

@param[in]	spraysCount Number of sprays to create.
@param[in]	spraySize Size of individual spray in pixels.
@return Returns the pointer to the created sprays.
*/
cv::Point2i **CreateSprays(int spraysCount, int n, int R);

/**
Deletes previously created sprays.

@param[in]	sprays Pointer to the sprays.
@param[in]	spraysCount Number of sprays.
*/
void DeleteSprays(cv::Point2i **sprays, int spraysCount);
/**
Performs the Random Sprays Retinex algorithm on a given image for specified parameters.

@param[in]	source The image to be processed.
@param[out]	destination The resulting image.
@param[in]	N Number of sprays to create.
@param[in]	n Size of individual spray in pixels.
@param[in]	rowsStep Rows counting step.
@param[in]	colsStep Columns counting step.
@param[in]	upperBound Maximal value for a pixel channel.
@param[in]	rFactor Maximum spray radius as part of the image diagonal.
*/
void PerformRSR(cv::Mat source, cv::Mat &destination, int N, int n, int rowsStep, int colsStep, double upperBound, double rFactor);

/**
Performs the slightly modified LRSR algorithm on a given channel with included multiple filtering.

@param[in]	source The image to be processed.
@param[out]	destination The resulting image.
@param[in]	N Number of sprays to create.
@param[in]	n Size of individual spray in pixels.
@param[in]	inputKernelSize The size of the kernel for blurring the original image and the RSR resulting image.
@param[in]	inputFilteringIterations Iterations of input filtering.
@param[in]	intensityChangeKernelSize The size of the kernel for blurring the intensity change.
@param[in]	intensityFilteringIterations Iterations of intensity filtering.
@param[in]	rowsStep Rows counting step.
@param[in]	colsStep Columns counting step.
@param[in]	upperBound Maximal value for a pixel channel.
@param[in]	rFactor Maximum spray radius as part of the image diagonal.
*/
void ProcessChannel(cv::Mat source, cv::Mat &destination, int N, int n, int inputKernelSize, int inputFilteringIterations, int illuminantKernelSize, int illuminantFilteringIterations, int rowsStep, int colsStep, double upperBound, double rFactor);

/**
Searches for the minimal value greater than zero.

@param[in]	source The image to be processed.
@return Returns the minimal value greater than zero.
*/
double min2(cv::Mat source);

/**
Performs image enhancement by processing the lightness channel using the Color Badger algorithm on a given image for specified parameters.

@param[in]	source The image to be processed.
@param[out]	destination The resulting image.
@param[in]	downP Root of the channel values to be calculated before the application of the slightly modified LRSR algorithm.
@param[in]	upP Power of the channel values to be calculated after the application of the slightly modified LRSR algorithm.
@param[in]	N Number of sprays to create.
@param[in]	n Size of individual spray in pixels.
@param[in]	inputKernelSize The size of the kernel for blurring the original image and the RSR resulting image.
@param[in]	inputFilteringIterations Iterations of input filtering.
@param[in]	intensityChangeKernelSize The size of the kernel for blurring the intensity change.
@param[in]	intensityFilteringIterations Iterations of intensity filtering.
@param[in]	rowsStep Rows counting step.
@param[in]	colsStep Columns counting step.
@param[in]	upperBound Maximal value for a pixel channel.
@param[in]	rFactor Maximum spray radius as part of the image diagonal.
*/
void ProcessLightnessChannel(cv::Mat initialSource, cv::Mat &destination, double downP, double upP, int N, int n, int inputKernelSize, int inputFilteringIterations, int illuminantKernelSize, int illuminantFilteringIterations, int rowsStep, int colsStep, double upperBound, double rFactor );
/**
Performs image enhancement by processing the RGB channels using the Color Badger algorithm on a given image for specified parameters.

@param[in]	source The image to be processed.
@param[out]	destination The resulting image.
@param[in]	downP Root of the channel values to be calculated before the application of the slightly modified LRSR algorithm.
@param[in]	upP Power of the channel values to be calculated after the application of the slightly modified LRSR algorithm.
@param[in]	N Number of sprays to create.
@param[in]	n Size of individual spray in pixels.
@param[in]	inputKernelSize The size of the kernel for blurring the original image and the RSR resulting image.
@param[in]	inputFilteringIterations Iterations of input filtering.
@param[in]	intensityChangeKernelSize The size of the kernel for blurring the intensity change.
@param[in]	intensityFilteringIterations Iterations of intensity filtering.
@param[in]	rowsStep Rows counting step.
@param[in]	colsStep Columns counting step.
@param[in]	upperBound Maximal value for a pixel channel.
@param[in]	rFactor Maximum spray radius as part of the image diagonal.
*/
void ProcessRGBChannels(cv::Mat initialSource, cv::Mat &destination, double downP, double upP, int N, int n, int inputKernelSize, int inputFilteringIterations, int illuminantKernelSize, int illuminantFilteringIterations, int rowsStep, int colsStep, double upperBound, double rFactor);

cv::Mat badger_process(cv::Mat source); 



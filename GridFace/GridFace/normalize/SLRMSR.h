
/*
* Copyright (c) University of Zagreb, Faculty of Electrical Engineering and Computing
* Authors: Nikola Banic <nikola.banic@fer.hr> and Sven Loncaric <sven.loncaric@fer.hr>
*
* This is only a research code and is therefore only of prototype quality.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* LITERATURE:
*
* N. Banic and S. Loncaric
* "Smart Light Random Memory Sprays Retinex: A Fast Retinex
* Implementation for High-Quality Brightness Adjustment and Color
* Correction"
* Journal of the Optical Society of America A
*
*/

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
#define CV_BGR2GRAY COLOR_BGR2GRAY



template<typename T>
class CartesianTree{
public:
	CartesianTree(int maxSize = 10007) :maxSize(maxSize){
		elements = new T[maxSize];
		parent = new int[maxSize];
		left = new int[maxSize];
		right = new int[maxSize];

		for (int i = 0; i<maxSize; ++i){
			parent[i] = left[i] = right[i] = -2;
		}

		count = 0;
		leftIdx = rightIdx = rootIdx = -2;
	}

	virtual ~CartesianTree(){
		delete[] elements;
		delete[] parent;
		delete[] left;
		delete[] right;
	}

	void Push(T element){
		++count;

		if (rightIdx == -2){
			rightIdx = 0;
		}
		else{
			++rightIdx;
		}

		if (rightIdx >= maxSize){
			rightIdx = 0;
		}

		elements[rightIdx] = element;
		if (leftIdx == -2){
			leftIdx = rightIdx;
		}

		if (rootIdx == -2){
			rootIdx = rightIdx;
			return;
		}

		parent[rightIdx] = rightIdx - 1;
		if (parent[rightIdx] == -1){
			if (count>1){
				parent[rightIdx] = maxSize - 1;
			}
			else{
				parent[rightIdx] = -2;
			}
		}
		left[rightIdx] = right[rightIdx] = -2;
		elements[rightIdx] = element;

		while (parent[rightIdx] != -2){
			if (elements[rightIdx] <= elements[parent[rightIdx]]){
				break;
			}
			left[rightIdx] = parent[rightIdx];
			parent[rightIdx] = parent[parent[rightIdx]];
		}

		if (left[rightIdx] != -2){
			parent[left[rightIdx]] = rightIdx;
		}
		if (parent[rightIdx] != -2){
			right[parent[rightIdx]] = rightIdx;
		}
		else{
			rootIdx = rightIdx;
		}

	}

	T Max() const{
		if (rootIdx == -2){
			return 0;
		}
		else{
			return elements[rootIdx];
		}
	}

	void Pop(){
		if (count == 0){
			return;
		}

		if (parent[leftIdx] != -2){
			if (right[leftIdx] != -2){
				parent[right[leftIdx]] = parent[leftIdx];
				left[parent[leftIdx]] = right[leftIdx];
			}
		}
		else{
			rootIdx = right[leftIdx];
			if (right[leftIdx] != -2){
				parent[right[leftIdx]] = -2;
			}
		}

		--count;
		if (count>0){
			++leftIdx;
			if (leftIdx >= maxSize){
				leftIdx = 0;
			}
		}
		else{
			leftIdx = rightIdx = rootIdx = -2;
		}

	}

	int Size() const{
		return count;
	}
private:
	int maxSize;
	T *elements;
	int *parent;
	int *left;
	int *right;
	int count;
	int rootIdx;
	int leftIdx;
	int rightIdx;
};


void ApplyIllumination(cv::Mat source, cv::Mat illumination, cv::Mat &destination, double upperBound);

void CombineImages(cv::Mat source1, cv::Mat source2, cv::Mat &destination, cv::Mat map);

void GuidedImageFilterC1(cv::Mat img, cv::Mat guidance, cv::Mat &result, int r, double epsilon);

void GuidedImageFilterC3(cv::Mat img, cv::Mat guidance, cv::Mat &result, int r, double epsilon);

void Normalize(cv::Mat &image);

void RandomMemorySpraysRetinexPerformWhiteBalance(cv::Mat source, cv::Mat &destination, int N, int n, double upperBound, int rowsStep, int colsStep, double rFactor);

void GuidedLightRandomMemorySpraysRetinexEstimateLocalIlumination(cv::Mat source, cv::Mat &destination, int N, int n, int inputKernelSize, int illuminantKernelSize, bool normalizeIlluminant, int rowsStep, int colsStep, double upperBound, double rFactor);

void LightRandomMemorySpraysRetinexEstimateLocalIlumination(cv::Mat source, cv::Mat &destination, int N, int n, int inputKernelSize, int illuminantKernelSize, bool normalizeIlluminant, int rowsStep, int colsStep, double upperBound, double rFactor);

void Adjust(cv::Mat source, cv::Mat &destination, bool adjustBrightness, bool adjustColors, int n, bool filterOnlyIllumination, bool guidedFilter, double adjustmentExponent, int kernelSize, int finalFilterSize, double upperBound);

cv::Mat SLRMSR_process(cv::Mat source); 


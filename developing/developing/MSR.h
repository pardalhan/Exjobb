
/*
* MSRCR：带彩色恢复的多尺度Retinex图像增强
* (Multi-Scale Retinex with Color Restoration)
* 改写自：2003 Fabien Pelisson <Fabien.Pelisson@inrialpes.fr>的
* GRetinex GIMP plug-in
*
* Copyright (C) 2009 MAO Y.B
*               2009. 3. 3
*               Visual Information Processing (VIP) Group, NJUST
*
* 算法细节请参考下面论文：
* D. J. Jobson, Z. Rahman, and G. A. Woodell. A multi-scale
* Retinex for bridging the gap between color images and the
* human observation of scenes. IEEE Transactions on Image Processing,
* 1997, 6(7): 965-976
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*/

# include <stdlib.h>   
# include <stdio.h>   
# include <math.h>   
# include <string.h>   

#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <../opencv/cv_image.h>

# define MAX_RETINEX_SCALES    8     
# define MIN_GAUSSIAN_SCALE   16      
# define MAX_GAUSSIAN_SCALE  250      

typedef struct
{
	int     scale;         
	int     nscales;       
	int     scales_mode;   
	float   cvar;          
} RetinexParams;


# define RETINEX_UNIFORM 0   
# define RETINEX_LOW     1   
# define RETINEX_HIGH    2   


static float RetinexScales[MAX_RETINEX_SCALES];

typedef struct
{
	int    N;
	float  sigma;
	double B;
	double b[4];
} gauss3_coefs;

/*
* Private variables.
*/
static RetinexParams rvals =
{
	300,             /* Scale */
	3,               /* Scales */
	RETINEX_UNIFORM, /* Retinex processing mode */
	1.2f             /* A variant */
};

# define clip( val, minv, maxv )    (( val = (val < minv ? minv : val ) ) > maxv ? maxv : val ) 

/*
* calculate scale values for desired distribution.
*/
void retinex_scales_distribution(float* scales, int nscales, int mode, int s);

/*
* Calculate the average and variance in one go.
*/
void compute_mean_var(float *src, float *mean, float *var, int size, int bytes);

/*
* Calculate the coefficients for the recursive filter algorithm
* Fast Computation of gaussian blurring.
*/
void compute_coefs3(gauss3_coefs * c, float sigma);

void gausssmooth(float *in, float *out, int size, int rowstride, gauss3_coefs *c);

/*
* This function is the heart of the algo.
* (a)  Filterings at several scales and sumarize the results.
* (b)  Calculation of the final values.
*/
void MSRCR(unsigned char * src, int width, int height, int bytes);

/*******************************************************************************
OpenCV的图像处理接口函数
*******************************************************************************/

cv::Mat MSR_process(cv::Mat source);
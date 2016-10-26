#include "stdafx.h"

//加入椒盐噪声

cv::Mat imgnoise(cv::Mat& img)
{
	cv::Mat img_noise(img);
	unsigned int img_row, img_col, img_channel, pos;
	img_row = img.rows;
	img_col = img.cols;
	img_channel = img.channels();
	switch (img_channel)
	{
	case 1:	//灰度图的情况
	{
		for (unsigned int i = 0;i < img_row;++i)
		{
			for (unsigned int j = 0;j < img_col;++j)
			{
				pos = random();
				 if (pos == 1)	//1代表黑色“椒”噪声点
				{
					img_noise.at<unsigned>(i, j) = 0;
				}	
				else if (pos == 2)	//2代表白色“盐”噪声点
				{ 
					img_noise.at<unsigned>(i, j) = 255; 
				}
				else
				{
					continue;
				}
			}
		}
		break;
	}
	case 3:	//彩色图的情况 
	{
		for (unsigned int i = 0;i < img_row;++i)
		{
			for (unsigned int j = 0;j < img_col;++j)
			{
				pos = random();	
				if (pos == 1)
				{
					for (unsigned int c = 0;c < 3;++c)
					{
						img_noise.at<cv::Vec3b>(i, j)[c] = 0;
					}
				}
				else if (pos==2)
				{
					for (unsigned int c = 0;c < 3;++c)
					{
						img_noise.at<cv::Vec3b>(i, j)[c] = 255;
					}
				}
				else
				{
					continue;
				}
			}
		}
		break;
	}
	}
	return img_noise;
}

#include "stdafx.h"

//谐波均值滤波

cv::Mat myHarmonic(cv::Mat & src)
{
	unsigned short img_row, img_col, model_row, model_col, channel, num;
	double count;
	cv::Mat dst(src);
	for (img_row = 1;img_row < src.rows - 1;++img_row)
	{
		for (img_col = 1;img_col < src.cols - 1;++img_col)
		{
			for (channel = 0;channel < 3;++channel)
			{
				count = 0;
				num = 9;
				for (model_row = img_row - 1;model_row <= img_row + 1;++model_row)
				{
					for (model_col = img_col - 1;model_col <= img_col + 1;++model_col)
					{
						if (src.at<cv::Vec3b>(model_row, model_col)[channel] != 0)
						{
							count += 1.0 / src.at<cv::Vec3b>(model_row, model_col)[channel];
						}
						else
						{
							--num;	//这好像还是很重要的一步？但应该影响不大啊
							continue;
						}
					}
				}
				dst.at<cv::Vec3b>(img_row, img_col)[channel] = num / count;
			}
		}
	}
	return dst;
}

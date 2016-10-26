#include "stdafx.h"

//ËãÊõ¾ùÖµÂË²¨

cv::Mat myArithmetic(cv::Mat& src)
{
	unsigned long int count;
	unsigned short img_row, img_col, model_row, model_col, channel, num;
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
							count += src.at<cv::Vec3b>(model_row, model_col)[channel];
						}
						else
						{
							//--num;
							continue;
						}
					}
				}
				dst.at<cv::Vec3b>(img_row, img_col)[channel] = count / num;
			}
		}
	}
	return dst;
}

#include "stdafx.h"

//ÖÐÖµÂË²¨Ëã·¨

cv::Mat myMedian(cv::Mat & src)
{
	unsigned short img_row, img_col, model_row, model_col, channel;
	cv::Mat dst(src);
	for (img_row = 1;img_row < src.rows - 1;++img_row)
	{
		for (img_col = 1;img_col < src.cols - 1;++img_col)
		{
			for (channel = 0;channel < 3;++channel)
			{
				std::vector<unsigned int> med;
				for (model_row = img_row - 1;model_row <= img_row + 1;++model_row)
				{
					for (model_col = img_col - 1;model_col <= img_col + 1;++model_col)
					{
						med.push_back(src.at<cv::Vec3b>(model_row, model_col)[channel]);
					}
				}
				std::sort(med.begin(), med.end());
				dst.at<cv::Vec3b>(img_row, img_col)[channel] = med[4];
			}
		}
	}
	return dst;
}

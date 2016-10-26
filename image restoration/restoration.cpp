#include "stdafx.h"
//输入图像为扩维后的图像，输出的图像维度与输入图像相同
//所有滤波模板均为3*3

void restoration(cv::Mat& src, cv::Mat& dst, unsigned int pos)
{
	switch (pos)
	{
	case Arithmetic_Filter:
		dst = myArithmetic(src);
		break;
	case Geometric_Filter:
		dst = myGeometric(src);
		break;
	case Harmonic_Filter:
		dst = myHarmonic(src);
		break;
	case Median_Filter:
		dst = myMedian(src);
		break;
	}
}

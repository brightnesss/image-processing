#include "stdafx.h"
//����ͼ��Ϊ��ά���ͼ�������ͼ��ά��������ͼ����ͬ
//�����˲�ģ���Ϊ3*3

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

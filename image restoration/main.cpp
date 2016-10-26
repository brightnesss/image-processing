//����ͼ������Ĵ���ҵ��ͼ��ԭ
//author by ZhangHan 2016/10/25

#include "stdafx.h"

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("F:\\�γ��ļ�\\����ͼ����\\���Ĵ���ҵ\\lena.jpg");
	imshow("lena", img);
	Mat img_noise = imgnoise(img);
	imshow("lena_noise", img_noise);
	Mat padded,img_re;
	copyMakeBorder(img_noise, padded, 1, 1, 1, 1, BORDER_CONSTANT,Scalar::all(255));
	imshow("padded", padded);
	restoration(padded, img_re, Arithmetic_Filter);
	img_re(Rect(1, 1, img.cols, img.rows)).copyTo(img_re);
	imshow("after_filter", img_re);
	waitKey();
	return 0;
}
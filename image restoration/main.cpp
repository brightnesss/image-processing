//数字图像处理第四次作业：图像复原
//author by ZhangHan 2016/10/25

#include "stdafx.h"

using namespace cv;
using namespace std;

int main()
{
	Mat img = imread("F:\\课程文件\\数字图像处理\\第四次作业\\lena.jpg");
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
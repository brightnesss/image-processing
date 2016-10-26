// cv1.cpp : 定义控制台应用程序的入口点。
//	数字图像处理第二次作业
//	灰度图的频域化及频域滤波
//	Author: ZhangHan
//	Time: 2016/10/02

#include "stdafx.h"

using namespace std;
using namespace cv;

void centralized(Mat& img)
{
	img.convertTo(img, CV_32FC1); //将img的元素类型转成允许存在负数的32bit:float
	for (int row = 0;row != img.rows;++row)
	{
		for (int col = 0;col != img.cols;++col)
		{
			img.at<float>(row, col) = ((row + col) % 2 == 0) ? img.at<float>(row, col) : -1 * img.at<float>(row, col);
		}
	}
} //中心化

Mat BWLF(const int m,const int n,const double D0,const int n0)
{
	//m,n分别是Mat模板的长和宽
	//D0是滤波器阈值
	//n0是阶数,n0越大越接近理想滤波器

	Mat bwf = Mat::zeros(m, n, CV_32FC1);
	for (int row = 0;row != bwf.rows;++row)
	{
		for (int col = 0;col != bwf.cols;++col)
		{
			double D = (row - (double(m) - 1) / 2.0)*(row - (double(m) - 1) / 2.0) + (col - (double(n) - 1) / 2.0)*(col - (double(n) - 1) / 2.0);
			D = sqrtf(D);
			bwf.at<float>(row, col) = 1.0 / (1 + powf((D / D0), 2*n0));
		}
	}
	return bwf;
} //创建巴特沃斯低通滤波器

int main()
{
	string imgStr = "D:\\ppt\\数字图像处理\\homework\\第二次作业\\addnoise.jpg";

	Mat img = imread(imgStr,CV_LOAD_IMAGE_GRAYSCALE);

	imshow("img", img);
	
	int dftRow = getOptimalDFTSize(img.rows); //行扩展
	int dftCol = getOptimalDFTSize(img.cols); //列扩展

	Mat img_padded;	//保存扩展后的图像

	copyMakeBorder(img, img_padded, 0, dftRow - img.rows, 0, dftCol - img.cols, BORDER_CONSTANT, Scalar::all(0)); //对原始图像img进行维度扩展以方便快速傅里叶变换

	centralized(img_padded); //中心化

	normalize(img_padded, img_padded, 0, 1, CV_MINMAX); //归一化

	Mat plans[] = { Mat_<float>(img_padded),Mat::zeros(img_padded.size(),CV_32FC1) };

	Mat complexI;

	merge(plans, 2, complexI);

	dft(complexI, complexI); //快速傅里叶变换

	split(complexI, plans);

	Mat magI;
	magnitude(plans[0], plans[1], magI); //求幅值

	magI += Scalar::all(1);
	log(magI, magI); //取对数以增强视觉效果

	normalize(magI, magI, 0, 1, CV_MINMAX);

	imshow("magI", magI);

	Mat bwf = BWLF(magI.rows, magI.cols, 40.0, 2); //生成一个巴特沃斯低通滤波器

	imshow("BWLF", bwf); 

	Mat plansFilter[]= { Mat_<float>(bwf),Mat::zeros(bwf.size(),CV_32FC1) };

	Mat complexFilter;

	merge(plansFilter, 2, complexFilter);

	mulSpectrums(complexI, complexFilter, complexI, 0);

	//以下为输出滤波后频率图的代码
	split(complexI, plans);

	Mat magI_after;
	magnitude(plans[0], plans[1], magI_after); //求幅值

	magI_after += Scalar::all(1);
	log(magI_after, magI_after); //取对数以增强视觉效果

	normalize(magI_after, magI_after, 0, 1, CV_MINMAX);

	imshow("magI_after", magI_after);
	//以上为输出滤波后频率图的代码

	normalize(complexI, complexI, 0, 0.5, CV_MINMAX);
	
	Mat img_after; //经滤波处理后的图像

	idft(complexI, complexI, DFT_REAL_OUTPUT);

	split(complexI, plans);

	img_after = plans[0];

	centralized(img_after);

	imshow("img_after", img_after);

	waitKey();
	
	return 0;
}


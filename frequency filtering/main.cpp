// cv1.cpp : �������̨Ӧ�ó������ڵ㡣
//	����ͼ����ڶ�����ҵ
//	�Ҷ�ͼ��Ƶ�򻯼�Ƶ���˲�
//	Author: ZhangHan
//	Time: 2016/10/02

#include "stdafx.h"

using namespace std;
using namespace cv;

void centralized(Mat& img)
{
	img.convertTo(img, CV_32FC1); //��img��Ԫ������ת��������ڸ�����32bit:float
	for (int row = 0;row != img.rows;++row)
	{
		for (int col = 0;col != img.cols;++col)
		{
			img.at<float>(row, col) = ((row + col) % 2 == 0) ? img.at<float>(row, col) : -1 * img.at<float>(row, col);
		}
	}
} //���Ļ�

Mat BWLF(const int m,const int n,const double D0,const int n0)
{
	//m,n�ֱ���Matģ��ĳ��Ϳ�
	//D0���˲�����ֵ
	//n0�ǽ���,n0Խ��Խ�ӽ������˲���

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
} //����������˹��ͨ�˲���

int main()
{
	string imgStr = "D:\\ppt\\����ͼ����\\homework\\�ڶ�����ҵ\\addnoise.jpg";

	Mat img = imread(imgStr,CV_LOAD_IMAGE_GRAYSCALE);

	imshow("img", img);
	
	int dftRow = getOptimalDFTSize(img.rows); //����չ
	int dftCol = getOptimalDFTSize(img.cols); //����չ

	Mat img_padded;	//������չ���ͼ��

	copyMakeBorder(img, img_padded, 0, dftRow - img.rows, 0, dftCol - img.cols, BORDER_CONSTANT, Scalar::all(0)); //��ԭʼͼ��img����ά����չ�Է�����ٸ���Ҷ�任

	centralized(img_padded); //���Ļ�

	normalize(img_padded, img_padded, 0, 1, CV_MINMAX); //��һ��

	Mat plans[] = { Mat_<float>(img_padded),Mat::zeros(img_padded.size(),CV_32FC1) };

	Mat complexI;

	merge(plans, 2, complexI);

	dft(complexI, complexI); //���ٸ���Ҷ�任

	split(complexI, plans);

	Mat magI;
	magnitude(plans[0], plans[1], magI); //���ֵ

	magI += Scalar::all(1);
	log(magI, magI); //ȡ��������ǿ�Ӿ�Ч��

	normalize(magI, magI, 0, 1, CV_MINMAX);

	imshow("magI", magI);

	Mat bwf = BWLF(magI.rows, magI.cols, 40.0, 2); //����һ��������˹��ͨ�˲���

	imshow("BWLF", bwf); 

	Mat plansFilter[]= { Mat_<float>(bwf),Mat::zeros(bwf.size(),CV_32FC1) };

	Mat complexFilter;

	merge(plansFilter, 2, complexFilter);

	mulSpectrums(complexI, complexFilter, complexI, 0);

	//����Ϊ����˲���Ƶ��ͼ�Ĵ���
	split(complexI, plans);

	Mat magI_after;
	magnitude(plans[0], plans[1], magI_after); //���ֵ

	magI_after += Scalar::all(1);
	log(magI_after, magI_after); //ȡ��������ǿ�Ӿ�Ч��

	normalize(magI_after, magI_after, 0, 1, CV_MINMAX);

	imshow("magI_after", magI_after);
	//����Ϊ����˲���Ƶ��ͼ�Ĵ���

	normalize(complexI, complexI, 0, 0.5, CV_MINMAX);
	
	Mat img_after; //���˲�������ͼ��

	idft(complexI, complexI, DFT_REAL_OUTPUT);

	split(complexI, plans);

	img_after = plans[0];

	centralized(img_after);

	imshow("img_after", img_after);

	waitKey();
	
	return 0;
}


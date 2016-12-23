//8*8矩阵的zigzag行程压缩编码示意

#include <iostream>
#include <opencv2\opencv.hpp>
#include <math.h>
#include <vector>
#include <algorithm>

#define PLUS 1
#define MINUS 0

using namespace std;
using namespace cv;

void InitMat(Mat &img, float(*pf)[8])
{
	for (unsigned short i = 0;i < img.rows;++i)
		for (unsigned short j = 0;j < img.cols;++j)
			img.at<float>(i, j) = *(*(pf + i) + j);
}

void MoveImg(Mat &src, Mat &dst, bool flag)
{
	for (unsigned short i = 0;i < src.rows;++i)
		for (unsigned short j = 0;j < src.cols;++j)
			dst.at<float>(i, j) = flag ? src.at<float>(i, j) + 128 : src.at<float>(i, j) - 128;
}

void WriteImg(Mat &img)
{
	for (unsigned short i = 0;i < img.rows;++i)
	{
		for (unsigned short j = 0;j < img.cols;++j)
		{
			cout << img.at<float>(i, j) << "  ";
		}
		cout << endl;
	}
}

void round(Mat &src,Mat &dst)
{
	for (unsigned short i = 0;i < src.rows;++i)
		for (unsigned short j = 0;j < src.cols;++j)
			dst.at<float>(i, j) = round(src.at<float>(i, j));
}

void quantification(Mat &src, Mat &dst, Mat &model)
{
	if ((src.rows != model.rows) || (src.cols != model.cols)) { return; }
	for (unsigned short i = 0;i < src.rows;++i)
		for (unsigned short j = 0;j < src.cols;++j)
			dst.at<float>(i, j) = src.at<float>(i, j) / model.at<float>(i, j);
	round(dst, dst);
}

void dequantification(Mat &src, Mat &dst, Mat &model) {
	if ((src.rows != model.rows) || (src.cols != model.cols)) { return; }
	for (unsigned short i = 0;i < src.rows;++i)
		for (unsigned short j = 0;j < src.cols;++j)
			dst.at<float>(i, j) = src.at<float>(i, j) * model.at<float>(i, j);
}

vector<int> zigzag(Mat &img)
{
	//8*8矩阵的zigzag系数排列矩阵
	vector<int> ans;
	int count = 0;
	int i = 0; //行标号
	int j = 0; //列标号
	int count_j = 1;
	int count_i = 2;
	ans.push_back(img.at<float>(i, j));
	++count;
	++j;
	while (count != 64)
	{
		while ((j != -1) && (i != 8))
		{
			ans.push_back(img.at<float>(i++, j--));
			++count;
		}

		if (i == 8)
		{
			i = 7;
			j = count_j;
			count_j += 2;
		}
		else j = 0;
		while ((i != -1) && (j != 8))
		{
			ans.push_back(img.at<float>(i--, j++));
			++count;
		}

		if (j == 8)
		{
			j = 7;
			i = count_i;
			count_i += 2;
		}
		else i = 0;
	}
	while (*ans.rbegin() == 0) ans.pop_back();
	return ans;
}

float calculateError(Mat &img1, Mat &img2)
{
	float error = 0;
	for (unsigned short i = 0;i != img1.rows;++i)
		for (unsigned short j = 0;j != img1.cols;++j)
			error = error + pow((img1.at<float>(i, j) - img2.at<float>(i, j)), 2);
	error = error / (img1.rows*img1.cols);
	return sqrt(error);
}

int main()
{
	float f_img[][8] = {
		52,55,61,66,70,61,64,73,
		63,59,66,90,109,85,69,72,
		62,59,68,113,144,104,66,73,
		63,58,71,122,154,106,70,69,
		67,61,68,104,126,88,68,70,
		79,65,60,70,77,68,58,75,
		85,71,64,59,55,61,65,83,
		87,79,69,68,65,76,78,94
	};
	Mat img(8, 8, CV_32FC1);
	InitMat(img, f_img);

	Mat moveImg(8, 8, CV_32FC1);
	MoveImg(img, moveImg, MINUS);

	Mat dctImg(8, 8, CV_32FC1);
	dct(moveImg, dctImg);

	round(dctImg, dctImg);

	float f_model[][8] = {
		16,11,10,16,24,40,51,61,
		12,12,14,19,26,58,60,55,
		14,13,16,24,40,57,69,56,
		14,17,22,29,51,87,80,62,
		18,22,37,56,68,109,103,77,
		24,35,55,64,81,104,113,92,
		49,64,78,87,103,121,120,101,
		72,92,95,98,112,100,103,99
	};
	Mat model(8, 8, CV_32FC1);
	InitMat(model, f_model);

	Mat compressImg(8, 8, CV_32FC1);
	quantification(dctImg, compressImg, model);

	vector<int> zigzagVec;
	zigzagVec = zigzag(compressImg);

	for (vector<int>::const_iterator iter = zigzagVec.begin();iter != zigzagVec.end();++iter)
	{
		cout << *iter << " ";
	}
	cout << "EOB" << endl << endl;

	Mat decode(8, 8, CV_32FC1);
	dequantification(compressImg, decode, model);

	Mat idctImg(8, 8, CV_32FC1);
	idct(decode, idctImg);

	round(idctImg, idctImg);

	Mat removeImg(8, 8, CV_32FC1);
	MoveImg(idctImg, removeImg, PLUS);

	float error = calculateError(img, removeImg);
	cout << error << endl;
}
//图像拼接

#include <opencv2\opencv.hpp>
#include <opencv2\nonfree\nonfree.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\legacy\legacy.hpp>
#include <vector>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main()
{
	string str = "F:\\课程文件\\数字图像处理\\大作业\\图像拼接\\Horizontal";
	string temp;
	Mat img1, img2;
	img1 = imread(str + "\\" + to_string(2)+".JPG");
	img2 = imread(str + "\\" + to_string(3)+".JPG");

	//imshow("img1", img1);
	//imshow("img2", img2);
	//vector<Mat> imgs;
	//vector<Mat>::size_type maxNum(5);
	//for (unsigned int iter = 0;iter != 5;++iter)
	//{
	//	temp = str + "\\" + to_string(iter) + ".JPG";
	//	imgs.push_back(imread(temp));
	//	//imgs[iter] = imread(temp);
	//}
	//for (unsigned int iter = 0;iter != maxNum;++iter)
	//{
	//	imshow("img" + to_string(iter), imgs[iter]);
	//}

	SiftFeatureDetector siftdtc;
	vector<KeyPoint> kp1, kp2;

	siftdtc.detect(img1, kp1);
	Mat output1;
	drawKeypoints(img1, kp1, output1);
	imshow("img1 keypooints", output1);

	//KeyPoint kp;

	//vector<KeyPoint>::iterator itvc;
	//for (itvc = kp1.begin();itvc != kp1.end();++itvc)
	//{
	//	cout << "angle" << itvc->angle << "\t" << itvc->class_id << "\t" << itvc->octave << "\t";
	//}

	siftdtc.detect(img2, kp2);
	Mat output2;
	drawKeypoints(img2, kp2, output2);
	imshow("img2 keypoints", output2);

	SiftDescriptorExtractor extra;
	Mat descri1, descri2;
	
	BruteForceMatcher<L2<float>> matcher;
	vector<DMatch> matches;
	Mat img_matches;
	extra.compute(img1, kp1, descri1);
	extra.compute(img2, kp2, descri2);

	//imshow("desc1", descri1);
	//cout << endl << descri1 << endl;
	//imshow("desc2", descri2);
	//cout << descri2 << endl;
	matcher.match(descri1, descri2, matches);

	drawMatches(img1, kp1, img2, kp2, matches, img_matches);
	imshow("matches", img_matches);

	waitKey();
}
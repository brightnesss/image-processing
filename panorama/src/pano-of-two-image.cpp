//图像拼接

#include <opencv2\opencv.hpp>
#include <opencv2\nonfree\nonfree.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\legacy\legacy.hpp>
#include <opencv2\stitching\stitcher.hpp>
#include <math.h>
#include <vector>
#include <iostream>
#include <string>
#include "pano.h"

using namespace std;
using namespace cv;

int main()
{
	string str = "..\\Horizontal";
	Mat img1, img2;
	img1 = imread(str + "\\" + to_string(2) + ".JPG");
	img2 = imread(str + "\\" + to_string(3) + ".JPG");

	SurfFeatureDetector surfdtc;
	//SiftFeatureDetector siftdtc;
	vector<KeyPoint> kp1, kp2;

	surfdtc.detect(img1, kp1);		//surf特征提取
	//siftdtc.detect(img1, kp1);	//sift特征提取
	Mat output1;
	drawKeypoints(img1, kp1, output1);	 //画出特征点到output1中
	//imwrite(str + "\\kp1.JPG", output1);
	imshow("img1 keypooints", output1);

	surfdtc.detect(img2, kp2);
	//siftdtc.detect(img2, kp2);
	Mat output2;
	drawKeypoints(img2, kp2, output2);
	//imwrite(str + "\\kp2.JPG", output2);
	imshow("img2 keypoints", output2);


	SurfDescriptorExtractor extra;	//surf特征描述子提取
	//SiftDescriptorExtractor extra;	//sift特征描述子提取
	Mat descri1, descri2;
	//
	BruteForceMatcher<L2<float>> matcher;	//暴力匹配
	//FlannBasedMatcher matcher;	//k近邻匹配
	vector<DMatch> matches;
	Mat img_matches;
	extra.compute(img1, kp1, descri1);
	extra.compute(img2, kp2, descri2);

	matcher.match(descri1, descri2, matches);	//匹配特征


	//去除不好的匹配，留下好的匹配结果
	double max_dist = 0;
	double min_dist = 100;

	//找到匹配的最大最小阈值
	for (int i = 0;i != descri1.rows;++i)
	{
		double dist = matches[i].distance;
		if (dist < min_dist) min_dist = dist;
		else if (dist > max_dist) max_dist = dist;
	}

	//以3倍最小距离为界，定义好、坏匹配
	vector<DMatch> goodMatches;
	for (int i = 0;i != descri1.rows;++i)
	{
		if (matches[i].distance <= max(3 * min_dist, 0.02))		
			goodMatches.push_back(matches[i]);
	}

	drawMatches(img1, kp1, img2, kp2, goodMatches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);	//画出好的匹配
	//drawMatches(img1, kp1, img2, kp2, matches, img_matches);

	////imwrite(str + "\\good_img_matches.JPG", img_matches);
	imshow("matches", img_matches);

	vector<Point2f> obj, scene;

	//通过ransac算法计算转移矩阵
	for (int i = 0;i != (int)goodMatches.size();++i)
	{
		//obj.push_back(kp1[goodMatches[i].queryIdx].pt);
		//scene.push_back(kp2[goodMatches[i].trainIdx].pt);

		obj.push_back(kp2[goodMatches[i].trainIdx].pt);
		scene.push_back(kp1[goodMatches[i].queryIdx].pt);
	}
	
	Mat H = findHomography(obj, scene, CV_RANSAC);

	//vector<Point2f> obj_corners(4);
	//obj_corners[0] = Point(0, 0);
	//obj_corners[1] = Point(img1.cols, 0);
	//obj_corners[2] = Point(img1.cols, img1.rows);
	//obj_corners[3] = Point(0, img1.rows);

	//vector<Point2f> scene_corners(4);
	//perspectiveTransform(obj_corners, scene_corners, H);
	//Point2f offset((float)img1.cols, 0);
	//line(img_matches, scene_corners[0] + offset, scene_corners[1] + offset, Scalar(0, 255, 0), 4);
	//line(img_matches, scene_corners[1] + offset, scene_corners[2] + offset, Scalar(0, 255, 0), 4);
	//line(img_matches, scene_corners[2] + offset, scene_corners[3] + offset, Scalar(0, 255, 0), 4);
	//line(img_matches, scene_corners[3] + offset, scene_corners[0] + offset, Scalar(0, 255, 0), 4);

	//imshow("matches+detection", img_matches);

	////imwrite(str + "\\matches+detection.JPG", img_matches);

	Mat result, resultback;

	//普通的融合，没有对融合中界点做出处理
	//warpPerspective(img2, result, H, Size(2 * img2.cols, img2.rows));
	//Mat half(result, cv::Rect(0, 0, img2.cols, img2.rows));
	//img1.copyTo(half);
	//imshow("reslut", result);

	////imwrite(str + "\\result.JPG", result);

	//使用最大最小滤波方法对融合中界作出处理
	warpPerspective(img2, result, H, Size(2 * img2.cols, img2.rows));
	result.copyTo(resultback);

	Mat half(result, Rect(0, 0, img2.cols, img2.rows));
	img1.copyTo(half);
	imshow("ajust", result);

	Mat result_maxblend = result.clone();
	double dblend = 0.0;
	int ioffset = img2.cols - 100;
	for (int i = 0;i != img2.rows;++i)
	{
		for (int j = 0;j != 100;++j)
		{
			int iresult = result.at<cv::Vec3b>(i, ioffset + j)[0] + result.at<cv::Vec3b>(i, ioffset + j)[1] + result.at<cv::Vec3b>(i, ioffset + j)[2];
			int iresultback = resultback.at<cv::Vec3b>(i, ioffset + j)[0] + resultback.at<cv::Vec3b>(i, ioffset + j)[1] + resultback.at<cv::Vec3b>(i, ioffset + j)[2];
			if (iresultback > iresult)
			{
				result_maxblend.at<cv::Vec3b>(i, ioffset + j) = resultback.at<cv::Vec3b>(i, ioffset + j);
			}
		}
	}

	imshow("result_maxvalue", result_maxblend);
	//imwrite(str + "\\result_maxvalue.JPG", result_linerblend);
	

	waitKey();
}
#pragma once

#include <opencv2\opencv.hpp>
#include <opencv2\nonfree\nonfree.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\legacy\legacy.hpp>
#include <opencv2\stitching\stitcher.hpp>
#include <math.h>
#include <vector>
#include <string>
typedef unsigned int flag;
static const flag Plane(0);
static const flag Spherical(1);
static const flag Cylindrical(2);
static const flag Horiz(3);
static const flag Vert(4);

class Pano
{
private:
	std::vector<cv::Mat> imgs;
	std::string address;
	std::vector<cv::Mat>::size_type maxNum;
	float ratio;
	cv::Stitcher mystitcher = cv::Stitcher::createDefault();
	bool LoadImgs(); //读入图片。读入数量与图片地址由maxNum和address控制
public:
	explicit Pano(const std::string &str, const std::vector<cv::Mat>::size_type &num, const float &r = 0.8) :maxNum(num), address(str), ratio(r) { LoadImgs(); }
	bool setMaxNum(const std::vector<cv::Mat>::size_type &); //重新设置maxNum的数值，若与原先数值不等，则需要增加或减少imgs的图片
	bool setAddress(const std::string &); //重新设置str，需要改变imgs的图片
	bool setRatio(const float &);
	//bool LoadImgs(); //读入图片。读入数量与图片地址由maxNum和address控制
	void setExposure(bool exposure = true);
	void setWarper(const flag& warper = Cylindrical);
	void setDierct(const flag& direct = Horiz);
	cv::Stitcher::Status computePano(cv::Mat &);
};
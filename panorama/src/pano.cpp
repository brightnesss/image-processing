#include "pano.h"

bool Pano::LoadImgs()
{
	std::string temp;
	cv::Mat read;
	std::vector<cv::Mat>::size_type iter;
	for (iter = 0;iter != maxNum;++iter)
	{
		temp = address + "\\" + std::to_string(iter) + ".JPG";
		try
		{
			read = cv::imread(temp);
		}
		catch (const std::exception&)
		{
			std::cerr << temp << " is not read successful" << std::endl;
			imgs.clear();
			return false;
		}
		int read_row = read.rows;
		int read_col = read.cols;
		resize(read, read, cv::Size(ratio*read_col, ratio*read_row), 0, 0);
		imgs.push_back(read);
	}
	return true;
}

bool Pano::setMaxNum(const std::vector<cv::Mat>::size_type &num)
{
	if (num == maxNum) return true;
	else if (maxNum > num)
	{
		std::vector<cv::Mat>::size_type temp = maxNum - num;
		while (temp--)
		{
			imgs.pop_back();
		}
	}
	else
	{
		std::vector<cv::Mat>::size_type iter;
		std::string str;
		cv::Mat read;
		for (iter=maxNum;iter != num;++iter)
		{
			str = address + "\\" + std::to_string(iter) + ".JPG";
			try
			{
				read = cv::imread(str);
			}
			catch (const std::exception&)
			{
				std::cerr << str << " is not read successful" << std::endl;
				imgs.clear();
				return false;
			}
			int read_row = read.rows;
			int read_col = read.cols;
			resize(read, read, cv::Size(ratio*read_col, ratio*read_row), 0, 0);
			imgs.push_back(read);
		}
	}
	maxNum = num;
	return true;
}

bool Pano::setAddress(const std::string &str)
{
	address = str;
	imgs.clear();
	bool isComplete = LoadImgs();
	return isComplete;
}

bool Pano::setRatio(const float &r)
{
	ratio = r;
	imgs.clear();
	bool isComplete = LoadImgs();
	return isComplete;
}

void Pano::setExposure(bool exposure)
{
	if (exposure)
	{
		mystitcher.setExposureCompensator(new cv::detail::BlocksGainCompensator());
	}
	else if (!exposure)
	{
		mystitcher.setExposureCompensator(new cv::detail::NoExposureCompensator()); //²»ÒªÆØ¹â²¹³¥
	}
}

void Pano::setWarper(const flag &warper)
{
	switch (warper)
	{
	case 0:
		{
			cv::PlaneWarper* cw = new cv::PlaneWarper();
			mystitcher.setWarper(cw);
			break;
		}
	case 1:
		{
			cv::SphericalWarper* cw = new cv::SphericalWarper();
			mystitcher.setWarper(cw);
			break;
		}
	case 2:
		{
			cv::CylindricalWarper* cw = new cv::CylindricalWarper();
			mystitcher.setWarper(cw);
			break;
		}
	}
}

void Pano::setDierct(const flag & direct)
{
	switch (direct)
	{
	case 3:
		mystitcher.setWaveCorrectKind(cv::detail::WAVE_CORRECT_HORIZ);
		break;
	case 4:
		mystitcher.setWaveCorrectKind(cv::detail::WAVE_CORRECT_VERT);
		break;
	}
}

cv::Stitcher::Status Pano::computePano(cv::Mat &dst)
{
	cv::Stitcher::Status status = mystitcher.estimateTransform(imgs);
	status = mystitcher.composePanorama(dst);
	return status;
}

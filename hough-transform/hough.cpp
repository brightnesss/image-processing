//Hough Transform

#include <opencv2\opencv.hpp>
#include <string>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main()
{
	string filename = "Data\\airport.jpg";

	Mat src_color = imread(filename);
	imshow("source", src_color);
	if (src_color.empty())
	{
		cout << "can not open " << filename << endl;
		return -1;
	}

	Mat src;
	cvtColor(src_color, src, CV_RGB2GRAY);
	Mat dst, cdst;
	Canny(src, dst, 250, 350, 3);
	cvtColor(dst, cdst, COLOR_GRAY2BGR);

	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 100, 50, 56);
	for (size_t i = 0; i < lines.size(); ++i)
	{
		Vec4i l = lines[i];
		line(src_color, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 2, CV_AA);
	}

	imshow("canny", dst);
	imshow("detected lines", src_color);

	waitKey();

	return 0;
}

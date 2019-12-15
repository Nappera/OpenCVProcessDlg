#pragma once
#include "opencv.hpp"

typedef struct RUN_LENGTH_ENCODING
{
	int row;
	int start_column;
	int end_column;


}T_RUN_LENGTH_ENCODING;


/***************************************************************************
/*************************DRAW CLASS****************************************
/**************************************************************************/
/**--------------------------------------------------------------------------------------------------------
**函数描述：在pic控件上画矩形
**日期 : 2019 - 04 - 01
**pDC：DC指针
**cr: 要汇出的矩形;
**--------------------------------------------------------------------------------------------------------*/
void DV_DrawRectangleOnDC(CDC* pDC, CRect cr);

/**--------------------------------------------------------------------------------------------------------
**函数描述：在pic控件上显示Mat
**日期 : 2019 - 04 - 01
**hWnd：控件句柄
**img: 图像矩阵;
**--------------------------------------------------------------------------------------------------------*/
void DV_ShowMatWithPic(HWND hWnd, cv::Mat &img);

/***************************************************************************
/*************************MATH CLASS*******************************
/**************************************************************************/

/**--------------------------------------------------------------------------------------------------------
**函数描述：冒泡排序
**日期 : 2019 - 07 - 03
**a 数组首地址
**n 数组长度
**model  排序方式  -1 降序  1 升序
**--------------------------------------------------------------------------------------------------------*/
void DV_BubbleSort(int* a, int n, int model);

/**--------------------------------------------------------------------------------------------------------
**函数描述：点到直线距离
**日期 : 2019 - 07 - 03
**pt:点
**line_pt1:直线上一点
**line_pt1:直线上另点;
**--------------------------------------------------------------------------------------------------------*/
double DV_DistancePL(cv::Point2f pt, cv::Point2f line_pt1, cv::Point2f line_pt2);

/**--------------------------------------------------------------------------------------------------------
**函数描述：字符串分割函数
**日期 : 2019 - 10 - 21
**str:字符串
**pattern:分割字符样式
**--------------------------------------------------------------------------------------------------------*/
std::vector<std::string> DV_split(std::string str, std::string pattern);
/**--------------------------------------------------------------------------------------------------------
**函数描述：字符串分割函数
**日期 : 2019 - 11 - 05
**line1:直线1
**line2:直线2
**--------------------------------------------------------------------------------------------------------*/

cv::Point2f DV_Intersectionll(cv::Vec4i line1, cv::Vec4i line2);


/***************************************************************************
/*************************IMAGE PROCESS CLASS*******************************
/**************************************************************************/
/**--------------------------------------------------------------------------------------------------------
**函数描述：取反互补原理填充区域内部
**日期 : 2019 - 04 - 01
**--------------------------------------------------------------------------------------------------------*/
void DV_fillHole(const cv::Mat srcimage, cv::Mat &dstimage);
/**--------------------------------------------------------------------------------------------------------
**函数描述：填充整幅图像小区域函数
**日期 : 2019 - 04 - 01
**AreaLimit：小区域界定阈值
**CheckMode: 0代表去除黑区域，1代表去除白区域;
**NeihborMode：0代表4邻域，1代表8邻域;
**--------------------------------------------------------------------------------------------------------*/
void DV_RemoveSmallRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode);

/**--------------------------------------------------------------------------------------------------------
**函数描述：计算区域方向，要求提取对象为1
**日期 : 2019 - 07 - 02
**src：输入region图像
**angle:region方向(角度值，顺时针方向）;
**--------------------------------------------------------------------------------------------------------*/
void DV_RegionOrientation(cv::Mat &src, double& angle);

/**--------------------------------------------------------------------------------------------------------
**函数描述：输出面积最大的轮廓
**日期 : 2019 - 07 - 02
**_contours：输入轮廓
**_maxContour:输出最大轮廓;
**--------------------------------------------------------------------------------------------------------*/
void DV_ContoursStd(std::vector<std::vector<cv::Point>> _contours, std::vector<cv::Point>& _maxContour);
/**--------------------------------------------------------------------------------------------------------
**函数描述：输出面积最大的轮廓的外接矩形并表达
**日期 : 2019 - 07 - 02
**src:输入图像
**dst:输出图像（with rec BGR)
**_maxContour:输入轮廓;
**color：显示颜色
**rect 最小外接矩形
**--------------------------------------------------------------------------------------------------------*/
void DV_SmallestRectangle2(cv::Mat src, cv::Mat& dst, std::vector<cv::Point>& _maxContour, cv::Scalar color, cv::RotatedRect& rect);
/**--------------------------------------------------------------------------------------------------------
**函数描述：生成带旋转角度的矩形掩模
**日期 : 2019 - 07 - 03
**src:输入图像
**dst:输出图像（with rec BGR)
**_maxContour:输入轮廓;
**color：显示颜色
**rect 最小外接矩形
**--------------------------------------------------------------------------------------------------------*/
void DV_GenElementRect2(cv::Mat& element,cv::Size size, double angle);

/**--------------------------------------------------------------------------------------------------------
**函数描述：BGR转HSI
**日期 : 2019 - 10 - 07
**src:输入图像（BGR）
**dst:输出图像（HSI)
**--------------------------------------------------------------------------------------------------------*/
void DV_BGR2HSI(cv::Mat src, cv::Mat& dst);

/**--------------------------------------------------------------------------------------------------------
**函数描述：保存图像
**日期 : 2019 - 10 - 25
**--------------------------------------------------------------------------------------------------------*/
void SaveImg(cv::Mat src, CString _dir);

/**--------------------------------------------------------------------------------------------------------
**函数描述：遍历线段上所有点坐标
**日期 : 2019 - 11 - 26
**--------------------------------------------------------------------------------------------------------*/
void DV_CreateLineIterator(cv::Point pt1, cv::Point pt2, std::vector<cv::Point>& ptcloud);

/**--------------------------------------------------------------------------------------------------------
**函数描述：根据轮廓生成region(run模式）
**日期 : 2019 - 11 - 26
**--------------------------------------------------------------------------------------------------------*/
void CV_GenRegionPolygonFilled(const std::vector<cv::Point>* input, std::vector<T_RUN_LENGTH_ENCODING>* output);
void CV_GenRegionPolygonFilled_V1(const std::vector<cv::Point>* input, std::vector<T_RUN_LENGTH_ENCODING>* output);
bool ComparePointY(cv::Point a, cv::Point b);
bool ComparePointX(cv::Point a, cv::Point b);
/***************************************************************************
/*************************LineFinder CLASS*******************************
/**************************************************************************/
class LineFinder {

private:
	const double PI = 3.1415926;
	// original image
	cv::Mat img;

	// vector containing the end points 
	// of the detected lines
	std::vector<cv::Vec4i> lines;

	// accumulator resolution parameters
	double deltaRho;
	double deltaTheta;

	// minimum number of votes that a line 
	// must receive before being considered
	int minVote;

	// min length for a line
	double minLength;

	// max allowed gap along the line
	double maxGap;

public:

	// Default accumulator resolution is 1 pixel by 1 degree
	// no gap, no mimimum length
	LineFinder() : deltaRho(1), deltaTheta(PI / 180), minVote(10), minLength(0.), maxGap(0.) {}

	// Set the resolution of the accumulator
	void setAccResolution(double dRho, double dTheta) {

		deltaRho = dRho;
		deltaTheta = dTheta;
	}

	// Set the minimum number of votes
	void setMinVote(int minv) {

		minVote = minv;
	}

	// Set line length and gap
	void setLineLengthAndGap(double length, double gap) {

		minLength = length;
		maxGap = gap;
	}

	// Apply probabilistic Hough Transform
	std::vector<cv::Vec4i> findLines(cv::Mat& binary) {

		lines.clear();
		/*	image: 边缘检测的输出图像.它应该是个灰度图(但事实上是个二值化图) *

		lines : 储存着检测到的直线的参数对(x_{ start }, y_{ start }, x_{ end }, y_{ end }) 的容器，也就是线段两个端点的坐标

		rho : 参数极径 r 以像素值为单位的分辨率.我们使用 1 像素.

		theta : 参数极角 \theta 以弧度为单位的分辨率.我们使用 1度(即CV_PI / 180)

		threshold: 要”检测” 一条直线所需最少的的曲线交点

		minLinLength : 能组成一条直线的最少点的数量.点数量不足的直线将被抛弃.线段的最小长度

		maxLineGap : 线段上最近两点之间的阈值
*/
		cv::HoughLinesP(binary, lines, deltaRho, deltaTheta, minVote, minLength, maxGap);
		return lines;
	}

	// Draw the detected lines on an image
	void drawDetectedLines(cv::Mat &image, cv::Scalar color = cv::Scalar(255, 255, 255),int thickness=1) {

		// Draw the lines
		std::vector<cv::Vec4i>::const_iterator it2 = lines.begin();

		while (it2 != lines.end()) {

			cv::Point pt1((*it2)[0], (*it2)[1]);
			cv::Point pt2((*it2)[2], (*it2)[3]);

			cv::line(image, pt1, pt2, color, thickness);

			++it2;
		}
	}

	double DistancePl(cv::Point2f pt, cv::Vec4i line)
	{
		double distance = 0.0;

		if (line[0] == line[2])
		{
			distance = std::abs(pt.x - line[0]);
		}
		else if (line[1] == line[3])
		{
			distance = std::abs(pt.y - line[1]);
		}
		else
		{
			double k = double(line[3] - line[1]) / (line[2] - line[0]);
			double b = double(line[1] - k*line[0]);
			distance = std::abs((k*pt.x + b - pt.y) / std::sqrt(1 + k*k));
		}
		return distance;
	}


	std::vector<cv::Vec4i> UnionLines(std::vector<cv::Vec4i> lines, double max_distance) 
	{
		std::vector<std::vector<cv::Vec4i>> u_lines;
		//归类
		for (size_t i = 0; i < lines.size(); i++)
		{
			cv::Vec4i templine = lines[i];
			int flag = 1;

			for (size_t j = 0; j < u_lines.size(); j++)
			{
				for (size_t k = 0; k < u_lines[j].size(); k++)
				{
					cv::Vec4i tempuline= u_lines[j][k];
					double distance = 0.0;
					//如果两条直线斜率都不存在
					if (templine[0]==templine[2]&&tempuline[0]==tempuline[2])
					{
						distance = std::abs(templine[0] - tempuline[0]);
					}
					else
					{
						double d1 = DistancePl(cv::Point2f(templine[0], templine[1]), tempuline);
						double d2 = DistancePl(cv::Point2f(templine[2], templine[3]), tempuline);
						double d3 = DistancePl(cv::Point2f(tempuline[0], tempuline[1]), templine);
						double d4 = DistancePl(cv::Point2f(tempuline[2], tempuline[3]), templine);
						distance = std::max(distance, d1);
						distance = std::max(distance, d2);
						distance = std::max(distance, d3);
						distance = std::max(distance, d4);
					}
					if (distance<max_distance)
					{
						u_lines[j].push_back(templine);
						j = u_lines.size();
						flag = 0;
						break;
					}

				}
			}

			if (flag)
			{
				u_lines.push_back(std::vector<cv::Vec4i>{templine});
			}
		}
		//拟合
		lines.clear();
		for (size_t i = 0; i < u_lines.size(); i++)
		{
			std::vector<cv::Point> points;
			int x_min = INT_MAX, x_max = 0;
			int y_min = INT_MAX, y_max = 0;
			for (size_t j = 0; j < u_lines[i].size(); j++)
			{
				cv::Vec4i line = u_lines[i][j];
				points.push_back(cv::Point(line[0], line[1]));
				points.push_back(cv::Point(line[2], line[3]));

				x_min = std::min(x_min, line[0]);
				x_min = std::min(x_min, line[2]);
				y_min = std::min(y_min, line[1]);
				y_min = std::min(y_min, line[3]);
				x_max = std::max(x_max, line[0]);
				x_max = std::max(x_max, line[2]);
				y_max = std::max(y_max, line[1]);
				y_max = std::max(y_max, line[3]);
			}
			cv::Vec4f line;
			cv::fitLine(cv::Mat(points), line, CV_DIST_L2, 0, 0.001, 0.001);

			int x_start, y_start;
			int x_end, y_end;

			double cos_theta = line[0];
			double sin_theta = line[1];
			double x0 = line[2], y0 = line[3];

			double k = sin_theta / cos_theta;
			double b = y0 - k * x0;

			if ((x_max - x_min) > (y_max - y_min))
			{
				x_start = x_min;
				y_start = k*x_start+b;

				x_end = x_max;
				y_end = k*x_end + b;
			}
			else
			{
				y_start = y_min;
				x_start = (y_start - b) / k;
				y_end = y_max;
				x_end = (y_end - b) / k;
			}
			lines.push_back(cv::Vec4i(x_start, y_start, x_end, y_end));

		}
		return lines;
	}
};
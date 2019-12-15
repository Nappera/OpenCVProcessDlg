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
**������������pic�ؼ��ϻ�����
**���� : 2019 - 04 - 01
**pDC��DCָ��
**cr: Ҫ����ľ���;
**--------------------------------------------------------------------------------------------------------*/
void DV_DrawRectangleOnDC(CDC* pDC, CRect cr);

/**--------------------------------------------------------------------------------------------------------
**������������pic�ؼ�����ʾMat
**���� : 2019 - 04 - 01
**hWnd���ؼ����
**img: ͼ�����;
**--------------------------------------------------------------------------------------------------------*/
void DV_ShowMatWithPic(HWND hWnd, cv::Mat &img);

/***************************************************************************
/*************************MATH CLASS*******************************
/**************************************************************************/

/**--------------------------------------------------------------------------------------------------------
**����������ð������
**���� : 2019 - 07 - 03
**a �����׵�ַ
**n ���鳤��
**model  ����ʽ  -1 ����  1 ����
**--------------------------------------------------------------------------------------------------------*/
void DV_BubbleSort(int* a, int n, int model);

/**--------------------------------------------------------------------------------------------------------
**�����������㵽ֱ�߾���
**���� : 2019 - 07 - 03
**pt:��
**line_pt1:ֱ����һ��
**line_pt1:ֱ�������;
**--------------------------------------------------------------------------------------------------------*/
double DV_DistancePL(cv::Point2f pt, cv::Point2f line_pt1, cv::Point2f line_pt2);

/**--------------------------------------------------------------------------------------------------------
**�����������ַ����ָ��
**���� : 2019 - 10 - 21
**str:�ַ���
**pattern:�ָ��ַ���ʽ
**--------------------------------------------------------------------------------------------------------*/
std::vector<std::string> DV_split(std::string str, std::string pattern);
/**--------------------------------------------------------------------------------------------------------
**�����������ַ����ָ��
**���� : 2019 - 11 - 05
**line1:ֱ��1
**line2:ֱ��2
**--------------------------------------------------------------------------------------------------------*/

cv::Point2f DV_Intersectionll(cv::Vec4i line1, cv::Vec4i line2);


/***************************************************************************
/*************************IMAGE PROCESS CLASS*******************************
/**************************************************************************/
/**--------------------------------------------------------------------------------------------------------
**����������ȡ������ԭ����������ڲ�
**���� : 2019 - 04 - 01
**--------------------------------------------------------------------------------------------------------*/
void DV_fillHole(const cv::Mat srcimage, cv::Mat &dstimage);
/**--------------------------------------------------------------------------------------------------------
**�����������������ͼ��С������
**���� : 2019 - 04 - 01
**AreaLimit��С����綨��ֵ
**CheckMode: 0����ȥ��������1����ȥ��������;
**NeihborMode��0����4����1����8����;
**--------------------------------------------------------------------------------------------------------*/
void DV_RemoveSmallRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode);

/**--------------------------------------------------------------------------------------------------------
**��������������������Ҫ����ȡ����Ϊ1
**���� : 2019 - 07 - 02
**src������regionͼ��
**angle:region����(�Ƕ�ֵ��˳ʱ�뷽��;
**--------------------------------------------------------------------------------------------------------*/
void DV_RegionOrientation(cv::Mat &src, double& angle);

/**--------------------------------------------------------------------------------------------------------
**������������������������
**���� : 2019 - 07 - 02
**_contours����������
**_maxContour:����������;
**--------------------------------------------------------------------------------------------------------*/
void DV_ContoursStd(std::vector<std::vector<cv::Point>> _contours, std::vector<cv::Point>& _maxContour);
/**--------------------------------------------------------------------------------------------------------
**����������������������������Ӿ��β����
**���� : 2019 - 07 - 02
**src:����ͼ��
**dst:���ͼ��with rec BGR)
**_maxContour:��������;
**color����ʾ��ɫ
**rect ��С��Ӿ���
**--------------------------------------------------------------------------------------------------------*/
void DV_SmallestRectangle2(cv::Mat src, cv::Mat& dst, std::vector<cv::Point>& _maxContour, cv::Scalar color, cv::RotatedRect& rect);
/**--------------------------------------------------------------------------------------------------------
**�������������ɴ���ת�Ƕȵľ�����ģ
**���� : 2019 - 07 - 03
**src:����ͼ��
**dst:���ͼ��with rec BGR)
**_maxContour:��������;
**color����ʾ��ɫ
**rect ��С��Ӿ���
**--------------------------------------------------------------------------------------------------------*/
void DV_GenElementRect2(cv::Mat& element,cv::Size size, double angle);

/**--------------------------------------------------------------------------------------------------------
**����������BGRתHSI
**���� : 2019 - 10 - 07
**src:����ͼ��BGR��
**dst:���ͼ��HSI)
**--------------------------------------------------------------------------------------------------------*/
void DV_BGR2HSI(cv::Mat src, cv::Mat& dst);

/**--------------------------------------------------------------------------------------------------------
**��������������ͼ��
**���� : 2019 - 10 - 25
**--------------------------------------------------------------------------------------------------------*/
void SaveImg(cv::Mat src, CString _dir);

/**--------------------------------------------------------------------------------------------------------
**���������������߶������е�����
**���� : 2019 - 11 - 26
**--------------------------------------------------------------------------------------------------------*/
void DV_CreateLineIterator(cv::Point pt1, cv::Point pt2, std::vector<cv::Point>& ptcloud);

/**--------------------------------------------------------------------------------------------------------
**����������������������region(runģʽ��
**���� : 2019 - 11 - 26
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
		/*	image: ��Ե�������ͼ��.��Ӧ���Ǹ��Ҷ�ͼ(����ʵ���Ǹ���ֵ��ͼ) *

		lines : �����ż�⵽��ֱ�ߵĲ�����(x_{ start }, y_{ start }, x_{ end }, y_{ end }) ��������Ҳ�����߶������˵������

		rho : �������� r ������ֵΪ��λ�ķֱ���.����ʹ�� 1 ����.

		theta : �������� \theta �Ի���Ϊ��λ�ķֱ���.����ʹ�� 1��(��CV_PI / 180)

		threshold: Ҫ����⡱ һ��ֱ���������ٵĵ����߽���

		minLinLength : �����һ��ֱ�ߵ����ٵ������.�����������ֱ�߽�������.�߶ε���С����

		maxLineGap : �߶����������֮�����ֵ
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
		//����
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
					//�������ֱ��б�ʶ�������
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
		//���
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
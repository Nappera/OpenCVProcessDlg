#include "stdafx.h"
#include "Process.h"

/***************************************************************************
/*************************DRAW CLASS****************************************
/**************************************************************************/
/**--------------------------------------------------------------------------------------------------------
** ���� : �ڿؼ��ϻ����ο�Ĭ��͸����
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void DV_DrawRectangleOnDC(CDC* pDC, CRect cr)
{

pDC->SetROP2(R2_NOT);
//pDC->SelectStockObject(NULL_BRUSH);

pDC->MoveTo(CPoint(cr.left, cr.top));
pDC->LineTo(CPoint(cr.right+1 , cr.top));

pDC->MoveTo(CPoint(cr.left, cr.top));
pDC->LineTo(CPoint(cr.left, cr.bottom+1 ));

pDC->MoveTo(CPoint(cr.right, cr.bottom-1));
pDC->LineTo(CPoint(cr.left-1 , cr.bottom));

pDC->MoveTo(CPoint(cr.right-1, cr.bottom));
pDC->LineTo(CPoint(cr.right, cr.top-1 ));
}
/**--------------------------------------------------------------------------------------------------------
** ���� : �ڿؼ�����ʾͼ��
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void DV_ShowMatWithPic(HWND hWnd, cv::Mat &img)
{
	
	int width = img.cols;
	int height = img.rows;
	int channels = img.channels();
	int type;
	type = img.type();

	size_t size = width*height*channels;
	unsigned char* dst = new unsigned char[size];
	memcpy(dst, img.data, size);

	static BITMAPINFO *bitmapinfo = NULL;
	unsigned char buffer[sizeof(BITMAPINFOHEADER) + 1024]; //BYTE *bitBuffer = new BYTE[40 + 4 * 256]
	memset(buffer, 0, 40 + 4 * 256);
	bitmapinfo = (BITMAPINFO*)buffer;
	bitmapinfo->bmiHeader.biSize = 40;
	bitmapinfo->bmiHeader.biPlanes = 1;
	bitmapinfo->bmiHeader.biWidth = width;
	bitmapinfo->bmiHeader.biHeight = -height;
	bitmapinfo->bmiHeader.biBitCount = 8 * channels;

	for (int i = 0; i < 256; i++)

	{ //��ɫ��ȡֵ��Χ (0-255)
		bitmapinfo->bmiColors[i].rgbBlue = i;
		bitmapinfo->bmiColors[i].rgbGreen = i;
		bitmapinfo->bmiColors[i].rgbRed = i;
		bitmapinfo->bmiColors[i].rgbReserved = 0;//�ɲ�Ҫ

	}
	/*	CClientDC dc(CWnd::FromHandle(hWnd));
	HDC hdc = dc.GetSafeHdc();*/
	HDC hdc = ::GetDC(hWnd);
	SetStretchBltMode(hdc, COLORONCOLOR);  //�ر������λͼ�ػ�ʧ��

	CRect drect;
	CWnd::FromHandle(hWnd)->GetClientRect(&drect);

	StretchDIBits(hdc,
		drect.left,
		drect.top,
		drect.Width(),
		drect.Height(),
		0,
		0,
		width,
		height,
		dst,
		bitmapinfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);

	::ReleaseDC(hWnd, hdc);
	delete(dst);  //�����ڴ����
}
/**--------------------------------------------------------------------------------------------------------
** ���� :�����߶������е�����
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void DV_CreateLineIterator(cv::Point pt1, cv::Point pt2, std::vector<cv::Point>& ptcloud)
{
	int _grayvalue = 0;

	int dx = abs(pt2.x - pt1.x);
	int dy = abs(pt2.y - pt1.y);

	int negY = (pt1.y > pt2.y)?-1:1;
	int negX = (pt1.x > pt2.x)?-1:1;

	cv::Point _dsd(-12, -12);

	if (dx==0)   //����
	{
		int count = dy+1;
		int n = 0;
		while (count--)
		{
			cv::Point _pt(pt1.x, pt1.y + negY * n);
			ptcloud.push_back( _pt);
			n++;
         }
	}
	else if (dy == 0) //����
	{
		int count = dx + 1;
		int n = 0;
		while (count--)
		{
			cv::Point _pt(pt1.x + negX * n, pt1.y );
			ptcloud.push_back(_pt);
			n++;
		}
	}
	else //б��
	{
		int steep_slope = (dy >= dx);
		if (steep_slope)
		{
			double slope = (dx*1.0)/ dy;
			int count = dy + 1;
			int n = 0;
			while (count--) 
			{
				cv::Point _pt(pt1.x + negX * slope*n+0.5*negX, pt1.y+negY*n);
				ptcloud.push_back(_pt);
				n++;
			}
		}
		else
		{
			double slope = dy*1.0 / dx;
			int count = dx + 1;
			int n = 0;
			while (count--)
			{
				cv::Point _pt(pt1.x + negX*n, pt1.y + negY*n * slope+0.5*negY);
				ptcloud.push_back(_pt);
				n++;
			}
		}

	}

	int a = 0;
}

/***************************************************************************
/*************************MATH CLASS*******************************
/**************************************************************************/
/**--------------------------------------------------------------------------------------------------------
** ���� : ð���㷨
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void DV_BubbleSort(int* a, int n, int model)
{
	int num = sizeof(a);
	int i, j, tmp;
	int flag;                 // ���
	if (model==0)
	{
		return;
	}
	else if(model > 0)  	 // ����
	{
		for (i = n - 1; i > 0; i--)
		{
			flag = 0;            // ��ʼ�����Ϊ0
			for (j = 0; j < i; j++)
			{
				if (a[j] > a[j + 1])
				{
					// ����a[j]��a[j+1]
					//tmp = a[j];
					//a[j] = a[j + 1];
					//a[j + 1] = tmp;
					std::swap(a[j], a[j + 1]);
					flag = 1;    // ������������������Ϊ1
				}
			}

			if (flag == 0)
				break;            // ��û������������˵������������
		}
	}
	else
	{                       // ����
		for (i = n-1; i >0; i--)
		{
			flag = 0;
			for(j = n-1; j > n-(i+1); j--)
			{
				int temp1 = a[j];
				int temp2 = a[j - 1];
				if (a[j]>a[j-1])
				{
					std::swap(a[j], a[j - 1]);
					flag = 1;
				}
			}
			if (flag == 0)
				break;          
		}
	}
}
/**--------------------------------------------------------------------------------------------------------
** ���� : ����㵽ֱ�߾���
** ���� :
**--------------------------------------------------------------------------------------------------------*/
double DV_DistancePL(cv::Point2f pt, cv::Point2f line_pt1, cv::Point2f line_pt2) {

	// ��(x0,y0)��ֱ��Ax+By+C=0�ľ���Ϊd = (A*x0+B*y0+C)/sqrt(A^2+B^2)
	double A, B, C, dis;
	dis = DBL_MAX;
	// ��������ʽΪһ��ʽ
	// ����ʽ��ʽΪ(y - y1)/(x - x1) = (y2 - y1)/ (x2 - x1)
	// ����Ϊһ��ʽΪ(y2 - y1)x + (x1 - x2)y + (x2y1 - x1y2) = 0
	// A = y2 - y1
	// B = x1 - x2
	// C = x2y1 - x1y2
	A = line_pt2.y - line_pt1.y;
	B = line_pt1.x - line_pt2.x;
	C = line_pt2.x * line_pt1.y - line_pt1.x * line_pt2.y;
	if (A!=0|B!=0)
	{
		// ���빫ʽΪd = |A*x0 + B*y0 + C|/��(A^2 + B^2)
		dis = abs(A * pt.x + B * pt.y + C) / sqrt(A * A + B * B);
	}
	return dis;
}


/**--------------------------------------------------------------------------------------------------------
** ���� : �ָ��ַ�
** ���� :
**--------------------------------------------------------------------------------------------------------*/
std::vector<std::string> DV_split(std::string str, std::string pattern) {
	
	std::vector<std::string> ret;
	if (pattern.empty()) return ret;
	
	int index = str.find_first_of(pattern, 0);
	int start = 0;
	while (index != -1)
	{
		ret.push_back(str.substr(start, index - start));
		start = index + 1;
		index = str.find_first_of(pattern, start);
	}

	if (!str.substr(start).empty())         //��ֹ���һ���ַ���������pattern����������ַ�
	{
		ret.push_back(str.substr(start));
	}


	return ret;




}
/**--------------------------------------------------------------------------------------------------------
** ���� : ��ֱ�߽���
** ���� :
**--------------------------------------------------------------------------------------------------------*/
cv::Point2f DV_Intersectionll(cv::Vec4i line1, cv::Vec4i line2) {

	cv::Point2f pt(FLT_MAX,FLT_MAX);
	float k1, k2, b1, b2;

	if (line1[0]==line1[2])//�����һ��ֱ��б�ʲ�����
	{
		if (line2[0] == line2[2])//����ڶ���ֱ��б��Ҳ������
		{
			return pt;
		}

		pt.x = line1[0];
		pt.y = line2[1] == line2[3] ? line2[1] : float(line2[1] - line2[3])*(pt.x - line2[0]) / (line2[0] - line2[2]) + line2[1];

	}
	else if (line2[0] == line2[2]) //����ڶ���ֱ��б�ʲ����ڣ���һ������
	{
		pt.x = line2[0];
		pt.y = line1[1] == line1[3] ? line1[1] :
			float(line1[1] - line1[3])*(pt.x - line1[0]) / (line1[0] - line1[2]) + line1[1];
	}
	else 
	{
		k1 = float(line1[3] - line1[1]) / (line1[2] - line1[0]);      b1 = float(line1[1] - k1*line1[0]);
		k2 = float(line2[3] - line2[1]) / (line2[2] - line2[0]);      b2 = float(line2[1] - k2*line2[0]);

		if (k1 == k2)//�����ֱ����б�ʵ�ƽ��
		{
			return pt;
		}

		pt.x = (b2 - b1) / (k1 - k2);  //���x
		pt.y = k1* pt.x + b1; //���y
	}

	return pt;
}

/***************************************************************************
/*************************IMAGE PROCESS CLASS*******************************
/**************************************************************************/
/**--------------------------------------------------------------------------------------------------------
** ���� : ���ͼ��with floodfill)
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void DV_fillHole(const cv::Mat srcimage, cv::Mat &dstimage)
{

	cv::Size m_Size = srcimage.size();
	cv::Mat temimage = cv::Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcimage.type());//��չͼ��  
	srcimage.copyTo(temimage(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)));
	floodFill(temimage, cv::Point(0, 0), cv::Scalar(255));
	cv::imshow("temimage", temimage);
	cv::Mat cutImg;//�ü���չ��ͼ��  
	temimage(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)).copyTo(cutImg);
	cv::imshow("~cutImg", ~cutImg);
	dstimage = srcimage | (~cutImg);
	cv::imshow("dstimage", dstimage);
}
/**--------------------------------------------------------------------------------------------------------
** ���� : ȥ��ͼ���ϵ�С����
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void DV_RemoveSmallRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode)
{
	int RemoveCount = 0;
	//�½�һ����ǩͼ���ʼ��Ϊ0���ص㣬Ϊ�˼�¼ÿ�����ص����״̬�ı�ǩ��
	//0����δ��飬1�������ڼ��,2�����鲻�ϸ���Ҫ��ת��ɫ����3������ϸ������ 
	//��ʼ����ͼ��ȫ��Ϊ0��δ���
	cv::Mat PointLabel = cv::Mat::zeros(Src.size(), CV_8UC1);
	cv::Mat FinalMat = cv::Mat::zeros(Src.size(), CV_8UC1);
	if (CheckMode == 1)//ȥ��С��ͨ����İ�ɫ��
	{
		for (int i = 0; i < Src.rows; i++)
		{
			for (int j = 0; j < Src.cols; j++)
			{
				if (Src.at<uchar>(i, j) < 10)
				{
					PointLabel.at<uchar>(i, j) = 3;//��������ɫ����Ϊ�ϸ�����Ϊ3
				}
			}
		}
	}
	else//ȥ���׶�����ɫ������
	{

		for (int i = 0; i < Src.rows; i++)
		{
			for (int j = 0; j < Src.cols; j++)
			{
				if (Src.at<uchar>(i, j) > 10)
				{
					PointLabel.at<uchar>(i, j) = 3;//���ԭͼ�ǰ�ɫ���򣬱��Ϊ�ϸ�����Ϊ3
				}
			}
		}
	}


	std::vector<cv::Point2i>NeihborPos;//������ѹ������
	NeihborPos.push_back(cv::Point2i(-1, 0));
	NeihborPos.push_back(cv::Point2i(1, 0));
	NeihborPos.push_back(cv::Point2i(0, -1));
	NeihborPos.push_back(cv::Point2i(0, 1));
	if (NeihborMode == 1)
	{

		NeihborPos.push_back(cv::Point2i(-1, -1));
		NeihborPos.push_back(cv::Point2i(-1, 1));
		NeihborPos.push_back(cv::Point2i(1, -1));
		NeihborPos.push_back(cv::Point2i(1, 1));
	}

	int NeihborCount = 4 + 4 * NeihborMode;
	int CurrX = 0, CurrY = 0;
	//��ʼ���
	for (int i = 0; i < Src.rows; i++)
	{
		for (int j = 0; j < Src.cols; j++)
		{
			if (PointLabel.at<uchar>(i, j) == 0)//��ǩͼ�����ص�Ϊ0����ʾ��δ���Ĳ��ϸ��
			{   //��ʼ���
				std::vector<cv::Point2i>GrowBuffer;//��¼������ص�ĸ���
				GrowBuffer.push_back(cv::Point2i(j, i));
				PointLabel.at<uchar>(i, j) = 1;//���Ϊ���ڼ��
				int CheckResult = 0;


				for (int z = 0; z < GrowBuffer.size(); z++)
				{
					for (int q = 0; q < NeihborCount; q++)
					{
						CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
						CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
						if (CurrX >= 0 && CurrX<Src.cols&&CurrY >= 0 && CurrY<Src.rows)  //��ֹԽ��  
						{
							if (PointLabel.at<uchar>(CurrY, CurrX) == 0)
							{
								GrowBuffer.push_back(cv::Point2i(CurrX, CurrY));  //��������buffer  
								PointLabel.at<uchar>(CurrY, CurrX) = 1;           //���������ļ���ǩ�������ظ����  
							}
						}
					}
				}
				if (GrowBuffer.size()>AreaLimit) //�жϽ�����Ƿ񳬳��޶��Ĵ�С����1Ϊδ������2Ϊ����  
					CheckResult = 2;
				else
				{
					CheckResult = 1;
					RemoveCount++;//��¼�ж�������ȥ��
				}


				for (int z = 0; z < GrowBuffer.size(); z++)
				{
					CurrX = GrowBuffer.at(z).x;
					CurrY = GrowBuffer.at(z).y;
					PointLabel.at<uchar>(CurrY, CurrX) += CheckResult;//��ǲ��ϸ�����ص㣬����ֵΪ2
				}
				//********�����õ㴦�ļ��**********  

			}
		}

	}

	CheckMode = 255 * (1 - CheckMode);
	//��ʼ��ת�����С������  
	for (int i = 0; i < Src.rows; ++i)
	{
		for (int j = 0; j < Src.cols; ++j)
		{
			if (PointLabel.at<uchar>(i, j) == 2)
			{
				FinalMat.at<uchar>(i, j) = CheckMode;
			}
			else if (PointLabel.at<uchar>(i, j) == 3)
			{
				FinalMat.at<uchar>(i, j) = Src.at<uchar>(i, j);

			}
		}
	}

	FinalMat.copyTo(Dst);
}
/**--------------------------------------------------------------------------------------------------------
** ���� : ���������
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void DV_RegionOrientation(cv::Mat &src, double& angle) {

	cv::Moments m = cv::moments(src, true);//moments()������������׼�һ�µľ� 
	cv::Point2d center(m.m10 / m.m00, m.m01 / m.m00);//��Ϊ���� 
   //���㷽�� 
	double a = m.m20 / m.m00 - center.x*center.x;
	double b = m.m11 / m.m00 - center.x*center.y; 
	double c = m.m02 / m.m00 - center.y*center.y; 
	double theta = cv::fastAtan2(2*b,(a - c))/2;//��Ϊ��״�ķ���
	angle = theta;
}
/**--------------------------------------------------------------------------------------------------------
** ���� : �����������
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void DV_ContoursStd(std::vector<std::vector<cv::Point>> _contours, std::vector<cv::Point>& _maxContour) {
	
	std::vector<cv::Point> tempcon = {cv::Point(0,0)};
	if (_contours.size()==0)
	{
		_maxContour = tempcon;
		return;
	}
	double maxarea = 0;
	int selindex = -1;
	for (int i = 0; i < _contours.size(); i++)
	{
		double tmparea = fabs(cv::contourArea(_contours[i]));
		if (maxarea<tmparea)
		{
			maxarea = tmparea;
			selindex = i;
		}
	}

	_maxContour.assign(_contours[selindex].begin(), _contours[selindex].end());

}
/**--------------------------------------------------------------------------------------------------------
** ���� : ������������������Ӿ��β����
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void DV_SmallestRectangle2(cv::Mat src, cv::Mat& dst, std::vector<cv::Point>& _maxContour, cv::Scalar color, cv::RotatedRect& rect) {

    rect = cv::minAreaRect(_maxContour);
	cv::Point2f pt[4];
	rect.points(pt);


	//�����������Ϊ��ɫ���Լ����ҪΪ��ɫ
	cv::Mat _show(src.rows, src.cols, CV_8UC3, cv::Scalar(0, 0, 0));
	for (size_t i = 0; i < _show.rows; i++)
	{
		uchar* data = _show.ptr<uchar>(i);
		uchar* data1 = src.ptr<uchar>(i);
		for (size_t j = 0; j < _show.cols; j++)
		{
			if (src.channels()==3)
			{
				data[j * 3 + 0] = data1[j * 3 + 0];
				data[j * 3 + 1] = data1[j * 3 + 1];
				data[j * 3 + 2] = data1[j * 3 + 2];
			}
			else if(src.channels() == 1){
				data[j * 3 + 0] = data1[j];
				data[j * 3 + 1] = data1[j];
				data[j * 3 + 2] = data1[j];
			}

		}
	}

	for (int i = 0; i < 4; i++)
		cv::line(_show, pt[i], pt[(i + 1) % 4], cv::Scalar(0, 255, 0), 3);

	_show.copyTo(dst);
}
/**--------------------------------------------------------------------------------------------------------
** ���� : ��ȡ���Ƕȵľ��νṹԪ
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void DV_GenElementRect2(cv::Mat& element, cv::Size size, double angle)
{

	cv::RotatedRect rectrotated(cv::Point2f(0, 0), size, angle);
	cv::Rect rect = rectrotated.boundingRect();

	element = cv::Mat(rect.size().height + 2, rect.size().width + 2, CV_8UC1, cv::Scalar(0));

	std::vector<cv::Point> contour;
	cv::RotatedRect re_rectrotated(cv::Point2f(element.cols / 2, element.rows / 2), size, angle);
	cv::Point2f pt[4];   re_rectrotated.points(pt);
	cv::Point2i point[4];
	int a = sizeof(pt) / sizeof(pt[0]);
	for (size_t i = 0; i <a; i++)
	{
		point[i] = pt[i];
		contour.push_back(point[i]);
	}

	std::vector<std::vector<cv::Point>> contours;
	contours.push_back(contour);
	cv::drawContours(element, contours, 0, 255, 1, 8);


	cv::Point2f point_tt;
	for (size_t i = 0; i < element.rows; i++)
	{
		uchar* data = element.ptr<uchar>(i);
		for (size_t j = 0; j < element.cols; j++)
		{
			point_tt.x = j;
			point_tt.y = i;
			/*��measureDist����Ϊtrueʱ������ʵ�ʾ���ֵ��
			������ֵΪ������ʾ���ڶ�����ڲ�������ֵΪ������ʾ�ڶ�����ⲿ������ֵΪ0����ʾ�ڶ�����ϡ�
			��measureDist����Ϊfalseʱ������ - 1��0��1�����̶�ֵ��
			������ֵΪ + 1����ʾ���ڶ�����ڲ�������ֵΪ - 1����ʾ�ڶ�����ⲿ������ֵΪ0����ʾ�ڶ�����ϡ�
			*/
			if (cv::pointPolygonTest(contours[0], point_tt, false) >= 0)
			{
				data[j] = 255;
			}
		}

	}
}

/**--------------------------------------------------------------------------------------------------------
** ���� : BGRתHSI
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void DV_BGR2HSI(cv::Mat src, cv::Mat& dst)
{
	int width = src.cols;
	int height = src.rows;
	int channels = src.channels();


	if (channels != 3)
		return;
	dst = cv::Mat(cv::Size(width, height), CV_8UC3);
	int nl = height; // number of lines
	int nc = width; // total number of elements per line

	if (src.isContinuous()) {
		nc = nc*nl;
		nl = 1;  // it is now a 1D array
	}

	//	unsigned char* dst = new unsigned char[nl*nc];

	for (int j = 0; j<nl; j++) {
		unsigned char* data = src.ptr<unsigned char>(j);
		unsigned char* dst_ptr = dst.ptr<unsigned char>(j);
		for (int i = 0; i<nc; i++) {

			//��ȡRGB����
			double B = *data++ / 255.0;
			double G = *data++ / 255.0;
			double R = *data++ / 255.0;

			//RGB2HSI���㷨ת��
			double maxRGB = std::max(std::max(R, G), B);
			double minRGB = std::min(std::min(R, G), B);
			double H;
			double S;
			double I;
			double Temp1, Temp2, Radians, Angle;
			double PI = 3.1415926;

			I = (R + G + B) / 3;

			if (I<0.078431)
				S = 0;
			else if (I>0.920000)
				S = 0;
			else
				S = 1.0 - (3.0*minRGB) / (R + G + B);

			if (maxRGB == minRGB)
			{
				H = 0;
				S = 0;
			}

			Temp1 = ((R - G) + (R - B)) / 2;
			Temp2 = (R - G)*(R - G) + (R - B)*(G - B);
			double Q = Temp1 / sqrt(Temp2);
			if (Q>0.9999999999)
				Radians = 0;
			else if (Q<-0.9999999999)
				Radians = PI;
			else
				Radians = acos(Q);
			Angle = Radians*180.0 / PI;
			if (B>G)
				H = (360.0 - Angle);
			else
				H = Angle;

			I = 255 * I;
			S = 255 * S;

			//�����������γ�HSIͼ��


			dst_ptr[3 * i] = (unsigned char)H;
			dst_ptr[3 * i + 1] = (unsigned char)S;
			dst_ptr[3 * i + 2] = (unsigned char)I;
		} // end of line                   
	}



}

/**--------------------------------------------------------------------------------------------------------
** ���� : ����ͼ��
** ���� :
**--------------------------------------------------------------------------------------------------------*/
void SaveImg(cv::Mat src, CString m_strFilePath)
{
	// TODO: Add your control notification handler code here
	/*
	λͼȫ�������أ��ǰ����������ϣ��������ҵ�˳�����е� RGB����Ҳ�ǵ�����ģ�ԭʼ�����ǰ�B��G��R��˳�����е�
    
	��Ϊ32λ��Windows����ϵͳ����4���ֽ�(32λ)���ٶȱȽϿ죬����BMP��ÿһ����ɫռ�õ��ֽ����涨Ϊ4��������
	*/

	int m_nImageWidth = src.cols; //������4��������
	int m_nImageHeight = src.rows;
	int channel = src.channels();
	uchar* m_pImg = src.data;

	DWORD		         dwImageSize = (DWORD)(m_nImageWidth * m_nImageHeight * channel);
	BITMAPFILEHEADER     stBfh = { 0 };
	DWORD		         dwBytesRead = 0;
	SYSTEMTIME           sysTime;
	CString              strFileName = TEXT ("");

	//��������ͼ����ļ���
	BOOL bRet = CreateDirectory(m_strFilePath, NULL);
	if (bRet)
	{
		SetFileAttributes(m_strFilePath, FILE_ATTRIBUTE_NORMAL);
	}
	//---------------------------��ʼ��bitmapͷ---------------------------

	BITMAPINFO          *m_pBmpInfo;	        ///< BITMAPINFO �ṹָ�룬��ʾͼ��ʱʹ��
	char                 m_chBmpBuf[40+1024];	    ///< BIMTAPINFO �洢��������m_pBmpInfo��ָ��˻�����
	//��ʵ���ڲ�ɫͼ��  m_chBmpBuf��40���͹��� û��ɫ��

	m_pBmpInfo = (BITMAPINFO *)m_chBmpBuf;
	m_pBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pBmpInfo->bmiHeader.biWidth = (LONG)m_nImageWidth; 
	m_pBmpInfo->bmiHeader.biHeight = -(LONG)m_nImageHeight;

	m_pBmpInfo->bmiHeader.biPlanes = 1;
	m_pBmpInfo->bmiHeader.biBitCount = 8* channel;
	m_pBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pBmpInfo->bmiHeader.biSizeImage = 0;
	m_pBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pBmpInfo->bmiHeader.biClrUsed = 0;
	m_pBmpInfo->bmiHeader.biClrImportant = 0;

	//�ڰ�ͼ����Ҫ�˲���
	for (int i = 0; i < 256; i++)

	{ //��ɫ��ȡֵ��Χ (0-255)
		m_pBmpInfo->bmiColors[i].rgbBlue = i;
		m_pBmpInfo->bmiColors[i].rgbGreen = i;
		m_pBmpInfo->bmiColors[i].rgbRed = i;
		m_pBmpInfo->bmiColors[i].rgbReserved = 0;//�ɲ�Ҫ

	}

	// ��ȡ��ǰʱ��Ϊͼ�񱣴��Ĭ������
	GetLocalTime(&sysTime);
	strFileName.Format(_T("%s\\%d_%d_%d_%d_%d_%d_%d.bmp"), m_strFilePath,
		sysTime.wYear,
		sysTime.wMonth,
		sysTime.wDay,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond,
		sysTime.wMilliseconds);


	////////////////////////////////////////////�ڰ�ͼ�񱣴����
	int nTable = 0;

	if (m_pBmpInfo->bmiHeader.biBitCount > 8) {
		nTable = 0;
	}
	else 
	{
		nTable = 256;
	}


	int size_RGBQUAD = sizeof(RGBQUAD);

	stBfh.bfType = (WORD)'M' << 8 | 'B';			 //�����ļ�����
	/*
	���ͼ���ǵ�ɫ��16ɫ��256ɫ��������ŵ�ɫ�����λͼ���ݣ�λͼ������ָ���ɫ���������š�
    ���λͼ��16λ��24λ��32λɫ����ͼ���ļ��в�������ɫ�壬�������ڵ�ɫ�壬ͼ�����ɫֱ����λͼ�����и�����
	*/
	stBfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+ size_RGBQUAD * nTable;	//�����ļ�ͷ��С��ɫ
	stBfh.bfSize = stBfh.bfOffBits + dwImageSize; //�ļ���С

												  //�����ļ�
	HANDLE hFile = ::CreateFile(strFileName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		/*
		BOOL WINAPI WriteFile(
  __in          HANDLE hFile,                   // �ļ����
  __in          LPCVOID lpBuffer,               // Ҫд�������
  __in          DWORD nNumberOfBytesToWrite,    // Ҫд����ֽ���
  __out         LPDWORD lpNumberOfBytesWritten, // ʵ��д����ֽ���
  __in          LPOVERLAPPED lpOverlapped       // OVERLAPPED �ṹ��һ���趨Ϊ NULL
);*/
		::WriteFile(hFile, &stBfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
		::WriteFile(hFile, m_pBmpInfo, sizeof(BITMAPINFOHEADER)+ size_RGBQUAD * nTable, &dwBytesRead, NULL);  //��ɫ
		::WriteFile(hFile, m_pImg, dwImageSize, &dwBytesRead, NULL);
		CloseHandle(hFile);
	}

}



void CV_GenRegionPolygonFilled(const std::vector<cv::Point>* input, std::vector<T_RUN_LENGTH_ENCODING>* output)
{
	const std::vector<cv::Point>* p = input;
	size_t size = p->size();
	std::vector<cv::Point> vP;
	cv::Point pt = (*p)[0];
	vP.push_back(pt);
	int min_row = pt.x;
	int max_row = pt.x;
	for (int i = 1; i < size; i++)
	{
		int length = (*p)[i].x - pt.x;
		if ((*p)[i].x - pt.x >= -1 && (*p)[i].x - pt.x <= 1)  //�жϷ�ʽ������ ���磨0��0������1��100�� �����㣿
		{
			pt = (*p)[i];
			vP.push_back(pt);
		}
		else if (length > 1)
		{
			int index = abs(length);
			double a = 1.0*((*p)[i].y - pt.y) / ((*p)[i].x - pt.x);
			double b = pt.y - a * pt.x + 0.5;
			while (index--)
			{
				pt.x = pt.x + 1;
				//double c = a * pt.x + b;
				pt.y = a * pt.x + b;
				vP.push_back(pt);
			}
			pt = (*p)[i];
			//vP.push_back(pt);
		}
		else
		{
			int index = abs(length) - 1;
			double a = 1.0*((*p)[i].y - pt.y) / ((*p)[i].x - pt.x);
			double b = pt.y - a * pt.x + 0.5;
			while (index--)
			{
				pt.x = pt.x - 1;
				pt.y = a * pt.x + b;
				vP.push_back(pt);
			}
			pt = (*p)[i];
			//vP.push_back(pt);
		}

		min_row = std::min(min_row, pt.x);
		max_row = std::max(max_row, pt.x);
	}

	size = vP.size();
	if (vP[0] == vP[size - 1])
	{
		vP.erase(vP.begin() + size - 1);
	}
	size = vP.size();
	std::vector <std::vector<cv::Point>> vPs(max_row - min_row + 1);
	for (int i = 0; i < size; i++)
	{
		vPs[vP[i].x - min_row].push_back(vP[i]);

	}

	size = vPs.size();
	for (int i = 0; i < size; i++)
	{
		std::vector<cv::Point>* q = &vPs[i];
		std::sort(q->begin(), q->end(), ComparePointY);
		int length = q->size() / 2;
		for (int j = 0; j< length; j++)
		{
			int index = j << 1;
			output->push_back(T_RUN_LENGTH_ENCODING{ (*q)[index].x,(*q)[index].y,(*q)[index + 1].y });

		}

	}

	int m = 0;
}
/**--------------------------------------------------------------------------------------------------------
**���� :������������region(runģʽ��
**���� :
**--------------------------------------------------------------------------------------------------------*/
void CV_GenRegionPolygonFilled_V1(const std::vector<cv::Point>* input, std::vector<T_RUN_LENGTH_ENCODING>* output)
{
	std::vector<cv::Point> _temp(*input);
	std::sort(_temp.begin(), _temp.end(), ComparePointY);
	int mat_height = _temp[_temp.size()-1].y;
	std::sort(_temp.begin(), _temp.end(), ComparePointX);
	int mat_width = _temp[_temp.size() - 1].x;
	cv::Mat _mat(mat_height+1, mat_width+1, CV_8UC1);

	std::vector<cv::Point> _inputcopy(*input);
	_inputcopy.push_back(_inputcopy[0]);

	const std::vector<cv::Point>* p = &_inputcopy;
	size_t size = p->size();

	std::vector<cv::Point> vP;
	cv::Point pt = (*p)[0];
	//vP.push_back(pt);
	int min_row = pt.y;
	int max_row = pt.y;
	for (int i = 0; i < size-1; i++)
	{
		cv::LineIterator linefit(_mat, (*p)[i], (*p)[i+1], 8);

		for (size_t i = 0; i < linefit.count-1; i++) //��Ҫ���һλ  -1
		{
			vP.push_back(linefit.pos());
			linefit++;
		}

		min_row = (min_row - (*p)[i].y>0) ? (*p)[i].y : min_row;
		max_row = (max_row - (*p)[i].y<0) ? (*p)[i].y : max_row;

	}
	
	size = vP.size();
	std::vector <std::vector<cv::Point>> vPs(max_row - min_row + 1);
	for (int i = 0; i < size; i++)
	{
		vPs[vP[i].y - min_row].push_back(vP[i]);

	}

	size = vPs.size();

	for (int i = 0; i < size; i++)
	{
		std::vector<cv::Point>* q = &vPs[i];
		std::sort(q->begin(), q->end(), ComparePointX);	
	    output->push_back(T_RUN_LENGTH_ENCODING{ (*q)[0].y,(*q)[0].x,(*q)[q->size() - 1].x });
	}

	int a = 0;

}

bool ComparePointY(cv::Point a, cv::Point b)
{
	return a.y < b.y; //��������
}
bool ComparePointX(cv::Point a, cv::Point b)
{
	return a.x < b.x; //��������
}

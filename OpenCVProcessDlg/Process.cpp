#include "stdafx.h"
#include "Process.h"

/***************************************************************************
/*************************DRAW CLASS****************************************
/**************************************************************************/
/**--------------------------------------------------------------------------------------------------------
** 描述 : 在控件上画矩形框（默认透明）
** 参数 :
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
** 描述 : 在控件上显示图像
** 参数 :
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

	{ //颜色的取值范围 (0-255)
		bitmapinfo->bmiColors[i].rgbBlue = i;
		bitmapinfo->bmiColors[i].rgbGreen = i;
		bitmapinfo->bmiColors[i].rgbRed = i;
		bitmapinfo->bmiColors[i].rgbReserved = 0;//可不要

	}
	/*	CClientDC dc(CWnd::FromHandle(hWnd));
	HDC hdc = dc.GetSafeHdc();*/
	HDC hdc = ::GetDC(hWnd);
	SetStretchBltMode(hdc, COLORONCOLOR);  //必备，解决位图重绘失真

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
	delete(dst);  //否则内存溢出
}
/**--------------------------------------------------------------------------------------------------------
** 描述 :遍历线段上所有点坐标
** 参数 :
**--------------------------------------------------------------------------------------------------------*/
void DV_CreateLineIterator(cv::Point pt1, cv::Point pt2, std::vector<cv::Point>& ptcloud)
{
	int _grayvalue = 0;

	int dx = abs(pt2.x - pt1.x);
	int dy = abs(pt2.y - pt1.y);

	int negY = (pt1.y > pt2.y)?-1:1;
	int negX = (pt1.x > pt2.x)?-1:1;

	cv::Point _dsd(-12, -12);

	if (dx==0)   //竖线
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
	else if (dy == 0) //横线
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
	else //斜线
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
** 描述 : 冒泡算法
** 参数 :
**--------------------------------------------------------------------------------------------------------*/
void DV_BubbleSort(int* a, int n, int model)
{
	int num = sizeof(a);
	int i, j, tmp;
	int flag;                 // 标记
	if (model==0)
	{
		return;
	}
	else if(model > 0)  	 // 升序
	{
		for (i = n - 1; i > 0; i--)
		{
			flag = 0;            // 初始化标记为0
			for (j = 0; j < i; j++)
			{
				if (a[j] > a[j + 1])
				{
					// 交换a[j]和a[j+1]
					//tmp = a[j];
					//a[j] = a[j + 1];
					//a[j + 1] = tmp;
					std::swap(a[j], a[j + 1]);
					flag = 1;    // 若发生交换，则设标记为1
				}
			}

			if (flag == 0)
				break;            // 若没发生交换，则说明数列已有序。
		}
	}
	else
	{                       // 降序
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
** 描述 : 计算点到直线距离
** 参数 :
**--------------------------------------------------------------------------------------------------------*/
double DV_DistancePL(cv::Point2f pt, cv::Point2f line_pt1, cv::Point2f line_pt2) {

	// 点(x0,y0)到直线Ax+By+C=0的距离为d = (A*x0+B*y0+C)/sqrt(A^2+B^2)
	double A, B, C, dis;
	dis = DBL_MAX;
	// 化简两点式为一般式
	// 两点式公式为(y - y1)/(x - x1) = (y2 - y1)/ (x2 - x1)
	// 化简为一般式为(y2 - y1)x + (x1 - x2)y + (x2y1 - x1y2) = 0
	// A = y2 - y1
	// B = x1 - x2
	// C = x2y1 - x1y2
	A = line_pt2.y - line_pt1.y;
	B = line_pt1.x - line_pt2.x;
	C = line_pt2.x * line_pt1.y - line_pt1.x * line_pt2.y;
	if (A!=0|B!=0)
	{
		// 距离公式为d = |A*x0 + B*y0 + C|/√(A^2 + B^2)
		dis = abs(A * pt.x + B * pt.y + C) / sqrt(A * A + B * B);
	}
	return dis;
}


/**--------------------------------------------------------------------------------------------------------
** 描述 : 分割字符
** 参数 :
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

	if (!str.substr(start).empty())         //防止最后一个字符串正好是pattern，输出个空字符
	{
		ret.push_back(str.substr(start));
	}


	return ret;




}
/**--------------------------------------------------------------------------------------------------------
** 描述 : 求直线交点
** 参数 :
**--------------------------------------------------------------------------------------------------------*/
cv::Point2f DV_Intersectionll(cv::Vec4i line1, cv::Vec4i line2) {

	cv::Point2f pt(FLT_MAX,FLT_MAX);
	float k1, k2, b1, b2;

	if (line1[0]==line1[2])//如果第一条直线斜率不存在
	{
		if (line2[0] == line2[2])//如果第二条直线斜率也不存在
		{
			return pt;
		}

		pt.x = line1[0];
		pt.y = line2[1] == line2[3] ? line2[1] : float(line2[1] - line2[3])*(pt.x - line2[0]) / (line2[0] - line2[2]) + line2[1];

	}
	else if (line2[0] == line2[2]) //如果第二条直线斜率不存在，第一条存在
	{
		pt.x = line2[0];
		pt.y = line1[1] == line1[3] ? line1[1] :
			float(line1[1] - line1[3])*(pt.x - line1[0]) / (line1[0] - line1[2]) + line1[1];
	}
	else 
	{
		k1 = float(line1[3] - line1[1]) / (line1[2] - line1[0]);      b1 = float(line1[1] - k1*line1[0]);
		k2 = float(line2[3] - line2[1]) / (line2[2] - line2[0]);      b2 = float(line2[1] - k2*line2[0]);

		if (k1 == k2)//如果两直线有斜率但平行
		{
			return pt;
		}

		pt.x = (b2 - b1) / (k1 - k2);  //算出x
		pt.y = k1* pt.x + b1; //算出y
	}

	return pt;
}

/***************************************************************************
/*************************IMAGE PROCESS CLASS*******************************
/**************************************************************************/
/**--------------------------------------------------------------------------------------------------------
** 描述 : 填充图像（with floodfill)
** 参数 :
**--------------------------------------------------------------------------------------------------------*/
void DV_fillHole(const cv::Mat srcimage, cv::Mat &dstimage)
{

	cv::Size m_Size = srcimage.size();
	cv::Mat temimage = cv::Mat::zeros(m_Size.height + 2, m_Size.width + 2, srcimage.type());//延展图像  
	srcimage.copyTo(temimage(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)));
	floodFill(temimage, cv::Point(0, 0), cv::Scalar(255));
	cv::imshow("temimage", temimage);
	cv::Mat cutImg;//裁剪延展的图像  
	temimage(cv::Range(1, m_Size.height + 1), cv::Range(1, m_Size.width + 1)).copyTo(cutImg);
	cv::imshow("~cutImg", ~cutImg);
	dstimage = srcimage | (~cutImg);
	cv::imshow("dstimage", dstimage);
}
/**--------------------------------------------------------------------------------------------------------
** 描述 : 去除图像上的小区域
** 参数 :
**--------------------------------------------------------------------------------------------------------*/
void DV_RemoveSmallRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode)
{
	int RemoveCount = 0;
	//新建一幅标签图像初始化为0像素点，为了记录每个像素点检验状态的标签，
	//0代表未检查，1代表正在检查,2代表检查不合格（需要反转颜色），3代表检查合格或不需检查 
	//初始化的图像全部为0，未检查
	cv::Mat PointLabel = cv::Mat::zeros(Src.size(), CV_8UC1);
	cv::Mat FinalMat = cv::Mat::zeros(Src.size(), CV_8UC1);
	if (CheckMode == 1)//去除小连通区域的白色点
	{
		for (int i = 0; i < Src.rows; i++)
		{
			for (int j = 0; j < Src.cols; j++)
			{
				if (Src.at<uchar>(i, j) < 10)
				{
					PointLabel.at<uchar>(i, j) = 3;//将背景黑色点标记为合格，像素为3
				}
			}
		}
	}
	else//去除孔洞，黑色点像素
	{

		for (int i = 0; i < Src.rows; i++)
		{
			for (int j = 0; j < Src.cols; j++)
			{
				if (Src.at<uchar>(i, j) > 10)
				{
					PointLabel.at<uchar>(i, j) = 3;//如果原图是白色区域，标记为合格，像素为3
				}
			}
		}
	}


	std::vector<cv::Point2i>NeihborPos;//将邻域压进容器
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
	//开始检测
	for (int i = 0; i < Src.rows; i++)
	{
		for (int j = 0; j < Src.cols; j++)
		{
			if (PointLabel.at<uchar>(i, j) == 0)//标签图像像素点为0，表示还未检查的不合格点
			{   //开始检查
				std::vector<cv::Point2i>GrowBuffer;//记录检查像素点的个数
				GrowBuffer.push_back(cv::Point2i(j, i));
				PointLabel.at<uchar>(i, j) = 1;//标记为正在检查
				int CheckResult = 0;


				for (int z = 0; z < GrowBuffer.size(); z++)
				{
					for (int q = 0; q < NeihborCount; q++)
					{
						CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
						CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
						if (CurrX >= 0 && CurrX<Src.cols&&CurrY >= 0 && CurrY<Src.rows)  //防止越界  
						{
							if (PointLabel.at<uchar>(CurrY, CurrX) == 0)
							{
								GrowBuffer.push_back(cv::Point2i(CurrX, CurrY));  //邻域点加入buffer  
								PointLabel.at<uchar>(CurrY, CurrX) = 1;           //更新邻域点的检查标签，避免重复检查  
							}
						}
					}
				}
				if (GrowBuffer.size()>AreaLimit) //判断结果（是否超出限定的大小），1为未超出，2为超出  
					CheckResult = 2;
				else
				{
					CheckResult = 1;
					RemoveCount++;//记录有多少区域被去除
				}


				for (int z = 0; z < GrowBuffer.size(); z++)
				{
					CurrX = GrowBuffer.at(z).x;
					CurrY = GrowBuffer.at(z).y;
					PointLabel.at<uchar>(CurrY, CurrX) += CheckResult;//标记不合格的像素点，像素值为2
				}
				//********结束该点处的检查**********  

			}
		}

	}

	CheckMode = 255 * (1 - CheckMode);
	//开始反转面积过小的区域  
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
** 描述 : 区域方向计算
** 参数 :
**--------------------------------------------------------------------------------------------------------*/
void DV_RegionOrientation(cv::Mat &src, double& angle) {

	cv::Moments m = cv::moments(src, true);//moments()函数计算出三阶及一下的矩 
	cv::Point2d center(m.m10 / m.m00, m.m01 / m.m00);//此为重心 
   //计算方向 
	double a = m.m20 / m.m00 - center.x*center.x;
	double b = m.m11 / m.m00 - center.x*center.y; 
	double c = m.m02 / m.m00 - center.y*center.y; 
	double theta = cv::fastAtan2(2*b,(a - c))/2;//此为形状的方向
	angle = theta;
}
/**--------------------------------------------------------------------------------------------------------
** 描述 : 计算最大轮廓
** 参数 :
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
** 描述 : 输出面积最大的轮廓的外接矩形并表达
** 参数 :
**--------------------------------------------------------------------------------------------------------*/
void DV_SmallestRectangle2(cv::Mat src, cv::Mat& dst, std::vector<cv::Point>& _maxContour, cv::Scalar color, cv::RotatedRect& rect) {

    rect = cv::minAreaRect(_maxContour);
	cv::Point2f pt[4];
	rect.points(pt);


	//考虑输入可能为彩色，以及输出要为彩色
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
** 描述 : 获取带角度的矩形结构元
** 参数 :
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
			/*当measureDist设置为true时，返回实际距离值。
			若返回值为正，表示点在多边形内部，返回值为负，表示在多边形外部，返回值为0，表示在多边形上。
			当measureDist设置为false时，返回 - 1、0、1三个固定值。
			若返回值为 + 1，表示点在多边形内部，返回值为 - 1，表示在多边形外部，返回值为0，表示在多边形上。
			*/
			if (cv::pointPolygonTest(contours[0], point_tt, false) >= 0)
			{
				data[j] = 255;
			}
		}

	}
}

/**--------------------------------------------------------------------------------------------------------
** 描述 : BGR转HSI
** 参数 :
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

			//抽取RGB分量
			double B = *data++ / 255.0;
			double G = *data++ / 255.0;
			double R = *data++ / 255.0;

			//RGB2HSI的算法转换
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

			//将分量联合形成HSI图像


			dst_ptr[3 * i] = (unsigned char)H;
			dst_ptr[3 * i + 1] = (unsigned char)S;
			dst_ptr[3 * i + 2] = (unsigned char)I;
		} // end of line                   
	}



}

/**--------------------------------------------------------------------------------------------------------
** 描述 : 保存图像
** 参数 :
**--------------------------------------------------------------------------------------------------------*/
void SaveImg(cv::Mat src, CString m_strFilePath)
{
	// TODO: Add your control notification handler code here
	/*
	位图全部的像素，是按照自下向上，自左向右的顺序排列的 RGB数据也是倒着念的，原始数据是按B、G、R的顺序排列的
    
	因为32位的Windows操作系统处理4个字节(32位)的速度比较快，所以BMP的每一行颜色占用的字节数规定为4的整数倍
	*/

	int m_nImageWidth = src.cols; //必须是4的整倍数
	int m_nImageHeight = src.rows;
	int channel = src.channels();
	uchar* m_pImg = src.data;

	DWORD		         dwImageSize = (DWORD)(m_nImageWidth * m_nImageHeight * channel);
	BITMAPFILEHEADER     stBfh = { 0 };
	DWORD		         dwBytesRead = 0;
	SYSTEMTIME           sysTime;
	CString              strFileName = TEXT ("");

	//创建保存图像的文件夹
	BOOL bRet = CreateDirectory(m_strFilePath, NULL);
	if (bRet)
	{
		SetFileAttributes(m_strFilePath, FILE_ATTRIBUTE_NORMAL);
	}
	//---------------------------初始化bitmap头---------------------------

	BITMAPINFO          *m_pBmpInfo;	        ///< BITMAPINFO 结构指针，显示图像时使用
	char                 m_chBmpBuf[40+1024];	    ///< BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区
	//其实对于彩色图像  m_chBmpBuf【40】就够了 没有色盘

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

	//黑白图像需要此部分
	for (int i = 0; i < 256; i++)

	{ //颜色的取值范围 (0-255)
		m_pBmpInfo->bmiColors[i].rgbBlue = i;
		m_pBmpInfo->bmiColors[i].rgbGreen = i;
		m_pBmpInfo->bmiColors[i].rgbRed = i;
		m_pBmpInfo->bmiColors[i].rgbReserved = 0;//可不要

	}

	// 获取当前时间为图像保存的默认名称
	GetLocalTime(&sysTime);
	strFileName.Format(_T("%s\\%d_%d_%d_%d_%d_%d_%d.bmp"), m_strFilePath,
		sysTime.wYear,
		sysTime.wMonth,
		sysTime.wDay,
		sysTime.wHour,
		sysTime.wMinute,
		sysTime.wSecond,
		sysTime.wMilliseconds);


	////////////////////////////////////////////黑白图像保存加入
	int nTable = 0;

	if (m_pBmpInfo->bmiHeader.biBitCount > 8) {
		nTable = 0;
	}
	else 
	{
		nTable = 256;
	}


	int size_RGBQUAD = sizeof(RGBQUAD);

	stBfh.bfType = (WORD)'M' << 8 | 'B';			 //定义文件类型
	/*
	如果图像是单色、16色和256色，则紧跟着调色板的是位图数据，位图数据是指向调色板的索引序号。
    如果位图是16位、24位和32位色，则图像文件中不保留调色板，即不存在调色板，图像的颜色直接在位图数据中给出。
	*/
	stBfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+ size_RGBQUAD * nTable;	//定义文件头大小彩色
	stBfh.bfSize = stBfh.bfOffBits + dwImageSize; //文件大小

												  //创建文件
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
  __in          HANDLE hFile,                   // 文件句柄
  __in          LPCVOID lpBuffer,               // 要写入的数据
  __in          DWORD nNumberOfBytesToWrite,    // 要写入的字节数
  __out         LPDWORD lpNumberOfBytesWritten, // 实际写入的字节数
  __in          LPOVERLAPPED lpOverlapped       // OVERLAPPED 结构，一般设定为 NULL
);*/
		::WriteFile(hFile, &stBfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL);
		::WriteFile(hFile, m_pBmpInfo, sizeof(BITMAPINFOHEADER)+ size_RGBQUAD * nTable, &dwBytesRead, NULL);  //彩色
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
		if ((*p)[i].x - pt.x >= -1 && (*p)[i].x - pt.x <= 1)  //判断方式有问题 例如（0，0）到（1，100） 几个点？
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
**描述 :根据轮廓生成region(run模式）
**参数 :
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

		for (size_t i = 0; i < linefit.count-1; i++) //不要最后一位  -1
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
	return a.y < b.y; //升序排列
}
bool ComparePointX(cv::Point a, cv::Point b)
{
	return a.x < b.x; //升序排列
}

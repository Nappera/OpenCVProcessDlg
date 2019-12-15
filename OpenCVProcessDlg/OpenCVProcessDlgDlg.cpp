
// OpenCVProcessDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenCVProcessDlg.h"
#include "OpenCVProcessDlgDlg.h"
#include "afxdialogex.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	
END_MESSAGE_MAP()


// COpenCVProcessDlgDlg �Ի���



COpenCVProcessDlgDlg::COpenCVProcessDlgDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_OPENCVPROCESSDLG_DIALOG, pParent)
{
	m_pCTemplateRotateDlg = NULL;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
COpenCVProcessDlgDlg::~COpenCVProcessDlgDlg() {
	if (m_pCTemplateRotateDlg != NULL)
	{
		delete(m_pCTemplateRotateDlg);
	}
}

void COpenCVProcessDlgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(COpenCVProcessDlgDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BTN_ADAPTIVE, &COpenCVProcessDlgDlg::OnBnClickedBtnAdaptive)
	ON_BN_CLICKED(IDC_BTN_BGR2GRAY, &COpenCVProcessDlgDlg::OnBnClickedBtnBgr2gray)
	ON_BN_CLICKED(IDC_BTN_CONTOURS, &COpenCVProcessDlgDlg::OnBnClickedBtnContours)
	ON_BN_CLICKED(IDC_BTN_FILLUPa, &COpenCVProcessDlgDlg::OnBnClickedBtnFillupa)
	ON_BN_CLICKED(IDC_BTN_OPENING, &COpenCVProcessDlgDlg::OnBnClickedBtnOpening)
	ON_COMMAND(ID_TEMPLATE, &COpenCVProcessDlgDlg::OnTemplate)
	ON_MESSAGE(WM_MYMSG_BASIC_TEMPLATE_MATCHING, &COpenCVProcessDlgDlg::OnBasicTemplateMatching)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BTN_ORIENTTATION, &COpenCVProcessDlgDlg::OnBnClickedBtnOrienttation)
	ON_BN_CLICKED(IDC_BTN_OPENINGREC, &COpenCVProcessDlgDlg::OnBnClickedBtnOpeningrec)
	ON_BN_CLICKED(IDC_BTN_HOUGHLINE, &COpenCVProcessDlgDlg::OnBnClickedBtnHoughline)
	ON_BN_CLICKED(IDC_BTN_Canny, &COpenCVProcessDlgDlg::OnBnClickedBtnCanny)
	ON_BN_CLICKED(IDC_BTN_TEST, &COpenCVProcessDlgDlg::OnBnClickedBtnTest)
	ON_BN_CLICKED(IDC_BTN_DISTANCETRANS, &COpenCVProcessDlgDlg::OnBnClickedBtnDistancetrans)
	ON_BN_CLICKED(IDC_BTN_TEST2, &COpenCVProcessDlgDlg::OnBnClickedBtnTest2)
	ON_COMMAND(ID_SAMPLE, &COpenCVProcessDlgDlg::OnSample)
	ON_BN_CLICKED(IDC_BTN_THRESHOLD, &COpenCVProcessDlgDlg::OnBnClickedBtnThreshold)
END_MESSAGE_MAP()


// COpenCVProcessDlgDlg ��Ϣ�������

BOOL COpenCVProcessDlgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�
	::CMenu m_Menu;
	m_Menu.LoadMenuW(IDR_MENU1);
	SetMenu(&m_Menu);

	m_statusBarCtrl.Create(WS_VISIBLE | WS_CHILD | SBT_OWNERDRAW, CRect(0, 0, 0, 0), this, 0);
	CRect cr;
	m_statusBarCtrl.GetWindowRect(&cr);
	int nWidth = cr.Width();
	int strPartDim[4] = { 2.5 * nWidth / 7,4 * nWidth / 7,6 * nWidth / 7, -1 }; //�ָ�����
	m_statusBarCtrl.SetParts(5, strPartDim);
	m_statusBarCtrl.SetText(TEXT("Camera Status, Now is Empty"), 0, 0);
	m_statusBarCtrl.SetText(TEXT("Remain "), 1, 0);
	m_statusBarCtrl.SetText(TEXT("About Calibration"), 2, 0);
	m_statusBarCtrl.SetText(TEXT("X,Y,Val"), 3, 0);


	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetDlgItemInt(IDC_EDIT_ADPT_BLOCKSIZE, 11);
	SetDlgItemInt(IDC_EDIT_ADPT_C, 5);
	SetDlgItemInt(IDC_EDIT_OPENINGELLIPSE_MASKW, 4);
	SetDlgItemInt(IDC_EDIT_OPENINGELLIPSE_MASKH, 4);
	SetDlgItemInt(IDC_EDIT_OPENINGREC_MASKW, 100);
	SetDlgItemInt(IDC_EDIT_OPENINGREC_MASKH, 3);
	SetDlgItemInt(IDC_EDIT_HOUGHLINESPTHRESHOLD, 100);
	SetDlgItemInt(IDC_EDIT_HOUGHLINESPMAX, 300);
	SetDlgItemInt(IDC_EDIT_CANNYTH1, 40);
	SetDlgItemInt(IDC_EDIT_CANNYTH2, 200);
	
	m_pCTemplateRotateDlg = new CTemplateRotateDlg(this);
	m_pCTemplateRotateDlg->Create(IDD_TEMPLATE_DIALOG);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void COpenCVProcessDlgDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void COpenCVProcessDlgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR COpenCVProcessDlgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void COpenCVProcessDlgDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	TCHAR szPath[MAX_PATH] = { 0 };
	UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
	for (UINT idx = 0; idx < 1; ++idx) {
		DragQueryFile(hDropInfo, idx, szPath, MAX_PATH);
		CString str(szPath);
		CString str_tail = str.Right(3);

		if (str.CompareNoCase(_T("png")) | str.CompareNoCase(_T("bmp")) | str.CompareNoCase(_T("jpg")) | str.CompareNoCase(_T("tif")))
		{
			USES_CONVERSION;
			m_matOriIMG = cv::imread(W2A(str), cv::IMREAD_UNCHANGED);
			//��Ȳ�Ϊ4��ͼƬ��ʾ�����������
			cv::resize(m_matOriIMG, m_matOriIMG, cv::Size(m_matOriIMG.cols / 4 * 4, m_matOriIMG.rows));
			DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, m_matOriIMG);


		}

	}
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}

void COpenCVProcessDlgDlg::OnBnClickedBtnAdaptive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	cv::blur(m_matCurrentIMG, m_matCurrentIMG, cv::Size(5, 5));
	cv::adaptiveThreshold(m_matCurrentIMG, m_matCurrentIMG, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, GetDlgItemInt(IDC_EDIT_ADPT_BLOCKSIZE), GetDlgItemInt(IDC_EDIT_ADPT_C));
	DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, m_matCurrentIMG);
}
void COpenCVProcessDlgDlg::OnBnClickedBtnThreshold()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cv::threshold(m_matCurrentIMG, m_matCurrentIMG, 128, 255, CV_THRESH_BINARY);
	DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, m_matCurrentIMG);
}


void COpenCVProcessDlgDlg::OnBnClickedBtnBgr2gray()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cv::Mat dst;
	if (m_matOriIMG.empty())
	{
		return;
	}
	if (m_matOriIMG.channels() == 3)
	{
		cv::cvtColor(m_matOriIMG, m_matCurrentIMG, CV_BGR2GRAY);
	}
	else {
		m_matOriIMG.copyTo(m_matCurrentIMG);
	}

	DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, m_matCurrentIMG);
}


void COpenCVProcessDlgDlg::OnBnClickedBtnContours()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(m_matCurrentIMG, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);
	cv::Mat _img(m_matCurrentIMG.rows, m_matCurrentIMG.cols, CV_8UC1, cv::Scalar(0, 0, 0));

	cv::Scalar color(255, 255, 255);
	cv::drawContours(_img, contours, -1, color, CV_FILLED, 8, hierarchy);
	_img.copyTo(m_matCurrentIMG);

	DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, m_matCurrentIMG);
	//��֤ ������������region��Run��ʽ
	std::vector<T_RUN_LENGTH_ENCODING> output;
	CV_GenRegionPolygonFilled_V1(&contours[0], &output);
	
}


void COpenCVProcessDlgDlg::OnBnClickedBtnFillupa()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	DV_fillHole(m_matCurrentIMG, m_matCurrentIMG);

	DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, m_matCurrentIMG);

}


void COpenCVProcessDlgDlg::OnBnClickedBtnOpening()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cv::Mat element_matrix = cv::getStructuringElement(CV_SHAPE_ELLIPSE, cv::Size(GetDlgItemInt(IDC_EDIT_OPENINGELLIPSE_MASKW), GetDlgItemInt(IDC_EDIT_OPENINGELLIPSE_MASKH)));
	cv::morphologyEx(m_matCurrentIMG, m_matCurrentIMG, cv::MORPH_OPEN, element_matrix);

	DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, m_matCurrentIMG);
}


void COpenCVProcessDlgDlg::OnTemplate()
{
	// TODO: �ڴ���������������
	m_pCTemplateRotateDlg->ShowWindow(SW_NORMAL);
}

LRESULT COpenCVProcessDlgDlg::OnBasicTemplateMatching(WPARAM w, LPARAM l) {
	if ((int)w==0)
	{

			CRect rc;
			//GetClientRect(&rc);
			//InvalidateRect(&rc, 1);
			::GetWindowRect(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, &rc);
			ScreenToClient(&rc);
			::InvalidateRect(m_hWnd, &rc, 1);
			OnPaint();
			DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, m_matOriIMG);
			m_rect.SetRect(-1, -1, -1, -1);
		
	}
	return 1;
}


void COpenCVProcessDlgDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}


void COpenCVProcessDlgDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_pCTemplateRotateDlg->GetShowWindowStatus())
	{
		if (m_pCTemplateRotateDlg->GetdrawTemplateAreaStatus())
		{
			if (m_rect.left != -1 & m_rect.top != -1)
			{
				CClientDC dc(this);
				DV_DrawRectangleOnDC(&dc, m_rect);
			}
			m_rect.SetRect(-1, -1, -1, -1);
			m_bIsDraw = true;
			m_rect.left = point.x;
			m_rect.top = point.y;
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void COpenCVProcessDlgDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_pCTemplateRotateDlg->GetShowWindowStatus())
	{
		if (m_pCTemplateRotateDlg->GetdrawTemplateAreaStatus())
		{
			m_bIsDraw = false;
			m_pCTemplateRotateDlg->FreezedrawTemplateAreaPermission();

			CString temp;
			temp.Format(TEXT("ģ��ROI(%d,%d,%d,%d)"), m_rect.left,m_rect.top,m_rect.right,m_rect.bottom);
			m_statusBarCtrl.SetText(temp, 1, 0);
		}
	}
	CDialogEx::OnLButtonUp(nFlags, point);
}


void COpenCVProcessDlgDlg::OnMouseMove(UINT nFlags, CPoint point)
{

	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	
	CString temp;
	temp.Format(TEXT("Client(%d,%d)"), point.x, point.y);

	ClientToScreen(&point);

	CString temp1;
	temp1.Format(TEXT("Screen(%d,%d)"), point.x, point.y);
	m_statusBarCtrl.SetText(temp + " " + temp1, 0, 0);

	if (m_bIsDraw)
	{
		CRect _rc;
		GetDlgItem(IDC_STATIC_VIEW)->GetWindowRect(&_rc);

		if (_rc.PtInRect(point))
		{
			CClientDC dc(this);//������GetDlgItem(IDC_STATIC_VIEW),�����ͼ��ƫ�Ƹÿռ��leftֵ&topֵ��ƫ����
			if (m_rect.right >= 0)
				DV_DrawRectangleOnDC(&dc, m_rect);
			ScreenToClient(&point);
			m_rect.right = point.x;
			m_rect.bottom = point.y;
			DV_DrawRectangleOnDC(&dc, m_rect);
			CDialogEx::OnMouseMove(nFlags, point);
		}
	}

}


void COpenCVProcessDlgDlg::OnBnClickedBtnOrienttation()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	double _angle = 0.0;
	std::vector<cv::Point> _maxcontour;
	std::vector<std::vector<cv::Point>> contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(m_matCurrentIMG, contours, hierarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE);

	DV_ContoursStd(contours, _maxcontour);
	cv::RotatedRect rect;
	cv::Scalar color = cv::Scalar(0, 255, 0);
	cv::Mat showMat;
	DV_SmallestRectangle2(m_matCurrentIMG, showMat, _maxcontour, color, rect);
	
	DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, showMat);
	_angle = rect.angle;

	CString _str;
	_str.Format(_T("%lf"), _angle);
	SetDlgItemText(IDC_EDIT_ORIENTTATION, _str);
}


void COpenCVProcessDlgDlg::OnBnClickedBtnOpeningrec()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_matCurrentIMG.empty())
	{
		return;
	}
	CString tempstr;
	GetDlgItemText(IDC_EDIT_ORIENTTATION, tempstr);
	double angle = _wtof(tempstr);
	cv::Size2f rectrotated_size = cv::Size2f(GetDlgItemInt(IDC_EDIT_OPENINGREC_MASKW), GetDlgItemInt(IDC_EDIT_OPENINGREC_MASKH));

	cv::Mat element;
	DV_GenElementRect2(element, rectrotated_size, angle);

	std::string str = "./";
	cv::imwrite(str + "openingrec.jpg", element);
    //��ʾ�ṹԪ
	//cv::Mat element_show;
	//cv::resize(element, element_show, cv::Size(element.cols / 4 * 4, element.rows / 4 * 4));
	//DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, element_show);
    cv::morphologyEx(m_matCurrentIMG, m_matCurrentIMG, cv::MORPH_OPEN, element);
	DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, m_matCurrentIMG);
}


void COpenCVProcessDlgDlg::OnBnClickedBtnHoughline()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cv::Mat _show = m_matOriIMG.clone();
	cv::Mat _show1 = m_matOriIMG.clone();
	cv::Mat _showtemp;
	if (_show.channels() == 3)
	{
		cv::cvtColor(_show, _showtemp, cv::COLOR_BGR2GRAY);
	}
	else
	{
		_show.copyTo(_showtemp);
	}

	// ��̬��ֵ�ָ����̸���ģΪ�߿��2+1
	cv::Mat img_threshold;
	cv::adaptiveThreshold(_showtemp, img_threshold, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 17, 60);


	// ��ȡ��Ե
	cv::Mat img_contours;
	cv::Canny(img_threshold, img_contours, 65, 250);
	// Hughֱ�߼��
	LineFinder lineFinder;
	// Set probabilistic Hough parameters
	lineFinder.setLineLengthAndGap(150, 30);
	lineFinder.setMinVote(10);

	// Detect lines
	std::vector<cv::Vec4i> lines = lineFinder.findLines(img_contours);
	//Draw lines
	lineFinder.drawDetectedLines(_show1, cv::Scalar(0, 0, 255), 2);
	//Union lines
	std::vector<cv::Vec4i> fitlines=lineFinder.UnionLines(lines, 30);

	for (size_t i = 0; i < fitlines.size(); i++)
	{
		cv::line(_show, cv::Point(fitlines[i][0], fitlines[i][1]), cv::Point(fitlines[i][2], fitlines[i][3]), cv::Scalar(0, 255, 255), 2);
	}

	
	cv::namedWindow("ԭʼHoughLinesP", CV_WINDOW_NORMAL);
	cv::imshow("ԭʼHoughLinesP", _show);
	cv::imshow("ԭʼHoughLinesPPPPP", _show1);
	cv::waitKey(0);

	//DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, _show);

}


void COpenCVProcessDlgDlg::OnBnClickedBtnCanny()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cv::Canny(m_matCurrentIMG, m_matCurrentIMG, GetDlgItemInt(IDC_EDIT_CANNYTH1), GetDlgItemInt(IDC_EDIT_CANNYTH2), 3);
	DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, m_matCurrentIMG);

}


void COpenCVProcessDlgDlg::OnBnClickedBtnTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int array[] = { 10,20,50,20,76,45 };
	int ilen = (sizeof(array)) / (sizeof(array[0]));
	DV_BubbleSort(array, ilen, -1);
	DV_BubbleSort(array, ilen, 1);



	cv::Mat hsi, tempimg;
	DV_BGR2HSI(m_matOriIMG, hsi);
	// HSIͨ������
	cv::Mat imgChannels[3];
	cv::split(hsi, imgChannels);
	DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, imgChannels[1]);
	cv::resize(imgChannels[1], tempimg, imgChannels[1].size() / 5);


	std::list<CString> ll;
	ll.push_back(_T("A"));
	ll.push_back(_T("B"));
	ll.push_back(_T("C"));
	ll.push_back(_T("D"));
	ll.push_back(_T("E"));

	CString str = ll.front();
	ll.pop_front();
	str = ll.front();
	

}


void COpenCVProcessDlgDlg::OnBnClickedBtnDistancetrans()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cv::Mat imageThin(m_matCurrentIMG.size(), CV_32FC1); //���屣�����任�����Mat����  
	distanceTransform(m_matCurrentIMG, imageThin, CV_DIST_L2, 3);  //����任  
	cv::Mat distShow;
	distShow = cv::Mat::zeros(m_matCurrentIMG.size(), CV_8UC1); //����ϸ������ַ�����  

	float maxValue = 0;  //�������任�����е����ֵ 
	for (int i = 0; i<imageThin.rows; i++)
	{
		for (int j = 0; j<imageThin.cols; j++)
		{
			if (imageThin.at<float>(i, j)>maxValue)
			{
				maxValue = imageThin.at<float>(i, j);  //��ȡ����任�ļ���ֵ  
			}
		}
	}
	for (int i = 0; i<imageThin.rows; i++)
	{
		for (int j = 0; j<imageThin.cols; j++)
		{
			if (imageThin.at<float>(i, j)>maxValue / 1.8)
			{
				distShow.at<uchar>(i, j) = 255;   //���Ͼ���������ֵһ�����������ĵ���Ϊ255  
			}
		}
	}
	cv::namedWindow("distancetransform", CV_WINDOW_NORMAL);
	cv::imshow("distancetransform", distShow);
	cv::waitKey(0);
	DV_ShowMatWithPic(GetDlgItem(IDC_STATIC_VIEW)->m_hWnd, distShow);

}


void COpenCVProcessDlgDlg::OnBnClickedBtnTest2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	/**************************split����***************************/
	std::string str = "asad\\sdada\\g\\gfh\\;\\";
	std::vector<std::string> vec = DV_split(str, "\\");
	/**************************��ͼ����***************************/
	CString m_strFilePath = TEXT("");
	CString strDate = TEXT("");
	char    szName[MAX_PATH] = { 0 };
	CString strAppFullName = TEXT("");
	GetModuleFileNameA(NULL, szName, MAX_PATH);
	strAppFullName = szName;
	int nPos = strAppFullName.ReverseFind('\\');
	m_strFilePath += strAppFullName.Left(nPos);
	m_strFilePath += "\\GxSingleCamColorImages";
	SaveImg(m_matOriIMG, m_strFilePath);
	/**************************����CV::fitline�õ�����ֱ������α��***************************/
	std::vector<cv::Point> points;
	points.push_back(cv::Point(20, 50));
	points.push_back(cv::Point(20,90));

	cv::Vec4f line;
	cv::fitLine(cv::Mat(points), line, CV_DIST_L2, 0, 0.001, 0.001);

	double cos_theta = line[0];
	double sin_theta = line[1];
	double x0 = line[2], y0 = line[3];

	double k = sin_theta / cos_theta;
	double b = y0 - k * x0;

	int y1 = k * 20 + b;

	int x2 = (30 - b) / k;


	/**************************����DV_Intersectionll***************************/
	cv::Vec4i line1{ 20,50,20,70 };
	cv::Vec4i line2{ 40,50,40,70 };
	cv::Point2f pt=DV_Intersectionll(line1, line2);

	/**************************����cv::LineIterator***************************/
	cv::Mat _mat(4, 4, CV_8UC1);
	cv::LineIterator sd(_mat, cv::Point(10, 10), cv::Point(12, 12),8);
	for (size_t i = 0; i < sd.count; i++)
	{		
		cv::Point _Pt = sd.pos();
		sd++;
	}
	
	/**************************��������ָ��***************************/
	int list[5] = { 0,1,2,3,4 };
	int* p = list;
	int m = *(p+1); //=1
	m = p[2];//=2
   /**************************����DV_CreateLineIterator***************************/
	std::vector<cv::Point> pointcloud;
	DV_CreateLineIterator(cv::Point(0, 0), cv::Point(20, 5), pointcloud);

}


void COpenCVProcessDlgDlg::OnSample()
{
	// TODO: �ڴ���������������
	TCHAR sgCurPath[MAX_PATH];
	ZeroMemory(sgCurPath, sizeof(sgCurPath));
	GetModuleFileName(NULL, sgCurPath, sizeof(sgCurPath) / sizeof(TCHAR));
	CString sgModulePath = sgCurPath;
	CString str = sgModulePath.Left(sgModulePath.ReverseFind('\\'))+_T("\\..\\..\\�ز�");

	ShellExecute(NULL, NULL, _T("explorer"), str, NULL, SW_SHOW);
}



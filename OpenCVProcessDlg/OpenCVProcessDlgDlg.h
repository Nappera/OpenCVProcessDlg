
// OpenCVProcessDlgDlg.h : 头文件
//
#include "opencv.hpp"
#include "Process.h"
#include "TemplateRotateDlg.h"
#include "DobotMagician.h"

#pragma once


// COpenCVProcessDlgDlg 对话框
class COpenCVProcessDlgDlg : public CDialogEx
{
// 构造
public:
	COpenCVProcessDlgDlg(CWnd* pParent = NULL);	// 标准构造函数
	~COpenCVProcessDlgDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCVPROCESSDLG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	CRect m_rect;
	bool m_bIsDraw;
public:
	cv::Mat m_matCurrentIMG;
	cv::Mat m_matOriIMG;
	CTemplateRotateDlg* m_pCTemplateRotateDlg;
	CStatusBarCtrl m_statusBarCtrl;

	CDobotMagician* m_pDobot;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);

	afx_msg void OnBnClickedBtnAdaptive();
	afx_msg void OnBnClickedBtnBgr2gray();
	afx_msg void OnBnClickedBtnContours();
	afx_msg void OnBnClickedBtnFillupa();
	afx_msg void OnBnClickedBtnOpening();
	afx_msg void OnTemplate();

	afx_msg LRESULT OnBasicTemplateMatching(WPARAM w, LPARAM l);			//基础实验 - 模板匹配
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnOrienttation();
	afx_msg void OnBnClickedBtnOpeningrec();
	afx_msg void OnBnClickedBtnHoughline();
	afx_msg void OnBnClickedBtnCanny();
	afx_msg void OnBnClickedBtnTest();
	afx_msg void OnBnClickedBtnDistancetrans();
	afx_msg void OnBnClickedBtnTest2();
	afx_msg void OnSample();
	afx_msg void OnBnClickedBtnThreshold();
};

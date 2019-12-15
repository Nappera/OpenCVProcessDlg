// TemplateRotateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OpenCVProcessDlg.h"
#include "TemplateRotateDlg.h"
#include "afxdialogex.h"
#include "OpenCVProcessDlgDlg.h"


// CTemplateRotateDlg 对话框

IMPLEMENT_DYNAMIC(CTemplateRotateDlg, CDialogEx)

CTemplateRotateDlg::CTemplateRotateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEMPLATE_DIALOG, pParent)
{
	m_hParentWnd = pParent->m_hWnd;
	m_bIsInitialization = false;
	m_bIsShowWindow = false;
	m_bIsSelectTemplateEnabled = false;
}

CTemplateRotateDlg::~CTemplateRotateDlg()
{
}

void CTemplateRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTemplateRotateDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK, &CTemplateRotateDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_BUTTON1, &CTemplateRotateDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


void CTemplateRotateDlg::UpdateParams() {
	::SendMessageW(m_hParentWnd, WM_MYMSG_BASIC_TEMPLATE_MATCHING, 1, 0);
}
// CTemplateRotateDlg 消息处理程序

void CTemplateRotateDlg::OnBnClickedCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateParams();
}


void CTemplateRotateDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bIsSelectTemplateEnabled = true;
}



void CTemplateRotateDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	m_bIsShowWindow = false;
	::SendMessageW(m_hParentWnd, WM_MYMSG_BASIC_TEMPLATE_MATCHING, 0, 0);
	CDialogEx::OnClose();
}


BOOL CTemplateRotateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bIsInitialization = true;

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CTemplateRotateDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (m_bIsInitialization)
	{
		m_bIsShowWindow = true;

	}
	// TODO: 在此处添加消息处理程序代码
}

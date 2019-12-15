// TemplateRotateDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenCVProcessDlg.h"
#include "TemplateRotateDlg.h"
#include "afxdialogex.h"
#include "OpenCVProcessDlgDlg.h"


// CTemplateRotateDlg �Ի���

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
// CTemplateRotateDlg ��Ϣ�������

void CTemplateRotateDlg::OnBnClickedCheck()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateParams();
}


void CTemplateRotateDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bIsSelectTemplateEnabled = true;
}



void CTemplateRotateDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	m_bIsShowWindow = false;
	::SendMessageW(m_hParentWnd, WM_MYMSG_BASIC_TEMPLATE_MATCHING, 0, 0);
	CDialogEx::OnClose();
}


BOOL CTemplateRotateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_bIsInitialization = true;

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CTemplateRotateDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	if (m_bIsInitialization)
	{
		m_bIsShowWindow = true;

	}
	// TODO: �ڴ˴������Ϣ����������
}

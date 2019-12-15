#pragma once


// CTemplateRotateDlg �Ի���

class CTemplateRotateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTemplateRotateDlg)

public:
	CTemplateRotateDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTemplateRotateDlg();
protected:
	HWND m_hParentWnd;
	bool m_bIsShowWindow;
	bool m_bIsInitialization;
	bool m_bIsSelectTemplateEnabled;
public:
	void UpdateParams();
	bool GetShowWindowStatus() { return m_bIsShowWindow; }//��ȡ����״̬
	bool GetdrawTemplateAreaStatus() { return m_bIsSelectTemplateEnabled; }
	void FreezedrawTemplateAreaPermission() {  m_bIsSelectTemplateEnabled=false; }
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEMPLATE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};

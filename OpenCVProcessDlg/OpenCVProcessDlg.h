
// OpenCVProcessDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "global.h"


// COpenCVProcessDlgApp: 
// �йش����ʵ�֣������ OpenCVProcessDlg.cpp
//

class COpenCVProcessDlgApp : public CWinApp
{
public:
	COpenCVProcessDlgApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern COpenCVProcessDlgApp theApp;
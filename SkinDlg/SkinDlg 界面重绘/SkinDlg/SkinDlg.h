
// SkinDlg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once
#define CORNER_SIZE 2

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSkinDlgApp: 
// �йش����ʵ�֣������ SkinDlg.cpp
//

class CSkinDlgApp : public CWinApp
{
public:
	CSkinDlgApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSkinDlgApp theApp;
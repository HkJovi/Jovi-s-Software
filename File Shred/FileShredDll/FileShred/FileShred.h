
// FileShred.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFileShredApp: 
// �йش����ʵ�֣������ FileShred.cpp
//

class CFileShredApp : public CWinApp
{
public:
	CFileShredApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFileShredApp theApp;
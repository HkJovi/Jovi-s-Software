
// HttpClient.h : HttpClient Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CHttpClientApp:
// �йش����ʵ�֣������ HttpClient.cpp
//

class CHttpClientApp : public CWinApp
{
public:
	CHttpClientApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	DECLARE_MESSAGE_MAP()
};

extern CHttpClientApp theApp;

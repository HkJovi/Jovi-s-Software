
// SicauPhotoWatcher.h : SicauPhotoWatcher Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CSicauPhotoWatcherApp:
// �йش����ʵ�֣������ SicauPhotoWatcher.cpp
//

class CSicauPhotoWatcherApp : public CWinApp
{
public:
	CSicauPhotoWatcherApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��

public:
	DECLARE_MESSAGE_MAP()
};

extern CSicauPhotoWatcherApp theApp;

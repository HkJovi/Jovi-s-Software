// FileShredDll.h : FileShredDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFileShredDllApp
// �йش���ʵ�ֵ���Ϣ������� FileShredDll.cpp
//

class CFileShredDllApp : public CWinApp
{
public:
	CFileShredDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

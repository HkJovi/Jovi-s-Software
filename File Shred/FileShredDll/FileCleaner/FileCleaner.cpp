// FileCleaner.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>

int _tmain(int argc, _TCHAR* argv[])
{
	HINSTANCE hDll = NULL;
	typedef void(*lpFunc)();
	hDll = LoadLibrary(L".\\FileCleanerDll.dll");
	if (hDll == NULL)
	{
		MessageBox(NULL,L"����FileCleanerDll.dllʧ��", L"����", MB_OK);
		return 0;
	}
	lpFunc ShowCleanerDlg = (lpFunc)GetProcAddress(hDll, "CreateModalCleanerDlg");
	if (ShowCleanerDlg == NULL)
	{
		MessageBox(NULL,L"����CreateModalCleanerDlgʧ��", L"����", MB_OK);
		return 0;
	}
	ShowCleanerDlg();
	return 0;
}


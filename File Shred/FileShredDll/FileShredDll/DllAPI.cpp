#include "stdafx.h"
#include "DllShredDlg.h"
#include "DllFileTreeDlg.h"
#include "UACHelper.h"
#include "VersionHelper.h"

extern CShredDlg *g_pDllDlg;

extern "C" __declspec(dllexport) void CreateCleanerDlg()
{
	if (!CVersionHelper::IsWindowsXPOrGreater())
	{
		MessageBox(NULL, L"�ļ�����C��֧��WINXP����ϵͳ��", L"�e�`", MB_OK | MB_ICONWARNING);
		return;
	}
	if (CVersionHelper::IsWindowsVistaOrGreater()) //UACȨ��
	{
		if (!CUACHelper::IsProcessRunAsAdmin())  //�ǹ���Ա���н���
		{
			MessageBox(NULL,L"�ǹ���T���ޟo�����d�ļ�����C��", L"�e�`", MB_OK | MB_ICONWARNING);
			return;
		}
	}
	if (g_pDllDlg == NULL)
	{
		g_pDllDlg = new CShredDlg();
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		//������Ϊ����
		g_pDllDlg->Create(IDD_DLL_SHRED_DIALOG, CWnd::FromHandle(GetDesktopWindow()));
		g_pDllDlg->ShowWindow(SW_SHOW);
		g_pDllDlg->CenterWindow();
	}
	else
	{
		if (::IsWindow(g_pDllDlg->GetSafeHwnd()))
		{
			//���ǰ����
			g_pDllDlg->ShowWindow(SW_SHOWNORMAL);
			g_pDllDlg->SetForegroundWindow();
		}
		else
		{
			AFX_MANAGE_STATE(AfxGetStaticModuleState());
			g_pDllDlg->Create(IDD_DLL_SHRED_DIALOG, CWnd::FromHandle(GetDesktopWindow()));
			g_pDllDlg->ShowWindow(SW_SHOW);
			g_pDllDlg->CenterWindow();
		}
	}
}

extern "C" __declspec(dllexport) void CreateModalCleanerDlg()
{
	if (!CVersionHelper::IsWindowsXPOrGreater())
	{
		MessageBox(NULL, L"�ļ�����C��֧��WINXP����ϵͳ��", L"�e�`", MB_OK | MB_ICONWARNING);
		return;
	}
	if (CVersionHelper::IsWindowsVistaOrGreater()) //UACȨ��
	{
		if (!CUACHelper::IsProcessRunAsAdmin())  //�ǹ���Ա���н���
		{
			MessageBox(NULL, L"�ǹ���T���ޟo�����d�ļ�����C��", L"�e�`", MB_OK | MB_ICONWARNING);
			return;
		}
	}
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CShredDlg dlg;
	dlg.DoModal();
}
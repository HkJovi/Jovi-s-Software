
// SicauPhotoWatcher.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "SicauPhotoWatcher.h"
#include "PhotoWatcherDlg.h"
#include "UpdateSever.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSicauPhotoWatcherApp

BEGIN_MESSAGE_MAP(CSicauPhotoWatcherApp, CWinApp)
END_MESSAGE_MAP()


// CSicauPhotoWatcherApp ����

CSicauPhotoWatcherApp::CSicauPhotoWatcherApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO:  ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("SicauPhotoWatcher.AppID.NoVersion"));

	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CSicauPhotoWatcherApp ����

CSicauPhotoWatcherApp theApp;


// CSicauPhotoWatcherApp ��ʼ��

BOOL CSicauPhotoWatcherApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO:  Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));


	//�汾��
	VerInfo oldverinfo;
	oldverinfo.Version = "2.2.0.0";
	oldverinfo.Date = "20140921";

	// ��Ҫ���������ڣ��˴��뽫�����µĿ�ܴ���
	// ����Ȼ��������ΪӦ�ó���������ڶ���

	//MessageBox(NULL, "ʹ�ù��ߣ��뱣����������ͨ��", "��ʾ", MB_OK | MB_ICONINFORMATION);

	//�汾���¼��
	VerInfo verinfo;
	UpdateSever updatesever;
	updatesever.GetVersionInfo(&verinfo);
	if (updatesever.CheckVeision(&oldverinfo, &verinfo)) //���ָ���
	{
		updatesever.DoUpdate(&verinfo);
	}

	//���������
	PhotoWatcherDlg Dlg;
	m_pMainWnd = &Dlg;
	SkinH_Init(m_hInstance);//��ʼ��Ƥ���������Ǽ���Ƥ��ע��Ŷ������Ǽ���Ƥ����ǰ����ѡ�
	Dlg.DoModal();
	return TRUE;
}

int CSicauPhotoWatcherApp::ExitInstance()
{
	//TODO:  �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CSicauPhotoWatcherApp ��Ϣ�������


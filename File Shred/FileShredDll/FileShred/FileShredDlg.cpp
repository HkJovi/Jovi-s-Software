
// FileShredDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileShred.h"
#include "FileShredDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

HINSTANCE g_hDll = NULL;
// CFileShredDlg �Ի���



CFileShredDlg::CFileShredDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileShredDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileShredDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileShredDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFileShredDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_GETICON, &CFileShredDlg::OnBnClickedButtonGeticon)
END_MESSAGE_MAP()


// CFileShredDlg ��Ϣ�������

BOOL CFileShredDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFileShredDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFileShredDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileShredDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	typedef void(*lpFunc)();
	g_hDll = LoadLibrary(L".\\FileCleanerDll.dll");
	if (g_hDll == NULL)
	{
		MessageBox(L"����FileCleanerDll.dllʧ��", L"����", MB_OK);
		return;
	}
	lpFunc ShowCleanerDlg = (lpFunc)GetProcAddress(g_hDll, "CreateCleanerDlg");
	if (ShowCleanerDlg == NULL)
	{
		MessageBox(L"����CreateCleanerDlgʧ��", L"����", MB_OK);
		return;
	}
	ShowCleanerDlg();	
}


void CFileShredDlg::OnBnClickedButtonGeticon()
{
	// TODO: Add your control notification handler code here
	
}


void CFileShredDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	CDialogEx::OnCancel();
	if (g_hDll != NULL)
	{
		FreeLibrary(g_hDll);
		g_hDll = NULL;
	}
}

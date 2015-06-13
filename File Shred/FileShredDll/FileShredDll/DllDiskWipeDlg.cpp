// DllDiskWipeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileCleanerDll.h"
#include "DllDiskWipeDlg.h"
#include "afxdialogex.h"

// CDiskWipeDlg dialog
IMPLEMENT_DYNAMIC(CDiskWipeDlg, CDialogEx)

CDiskWipeDlg::CDiskWipeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiskWipeDlg::IDD, pParent)
{

}

CDiskWipeDlg::~CDiskWipeDlg()
{
}

void CDiskWipeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DISKLIST, m_combox_disk);
	DDX_Control(pDX, IDC_PROGRESS_DISKWIPE, m_progress_wipe);
	DDX_Control(pDX, IDC_BUTTON_PAUSE, m_btn_pause);
	DDX_Control(pDX, IDC_BUTTON_WIPE, m_btn_start);
	DDX_Control(pDX, IDC_STATIC_PROGRESS_INFO, m_lable_info);
}


BEGIN_MESSAGE_MAP(CDiskWipeDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_WIPE, &CDiskWipeDlg::OnBnClickedButtonWipe)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CDiskWipeDlg::OnBnClickedButtonPause)
	ON_MESSAGE(WM_UPDATE_WIPE_PROCESS, &CDiskWipeDlg::OnUpdateWipeProcess)
//	ON_WM_CLOSE()
ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDiskWipeDlg message handlers


BOOL CDiskWipeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	//�M�ȗl
	m_progress_wipe.SetRange(0, 100);
	//�[�ؕ�ͣ���o
	m_btn_pause.ShowWindow(SW_HIDE);
	//�@��߉݋�űP
	DWORD dwDrives = GetLogicalDrives();
	CString DriveName;
	char a = 'A';
	while (dwDrives > 0)
	{
		if (dwDrives % 2 == 1)
		{
			DriveName.Format(L"%c:\\", a);
			//��ȥ���
			if (GetDriveType(DriveName.GetBuffer(0)) != DRIVE_CDROM) 
			{
				m_combox_disk.AddString(DriveName.GetBuffer(0));
			}
		}
		a++;
		dwDrives /= 2;
	}
	m_combox_disk.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CDiskWipeDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_ESCAPE)     return   TRUE;
	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == VK_RETURN)   return   TRUE;
	else
	   return CDialogEx::PreTranslateMessage(pMsg);
}


void CDiskWipeDlg::OnBnClickedButtonWipe()
{
	// TODO: Add your control notification handler code here
	CString temp;
	m_btn_start.GetWindowText(temp); //�@ȡ�F�ڵİ��o����
	if (temp == L"�_ʼ����")
	{
		UINT nRst = MessageBox(L"�������팢���űP�M�д����x���K�ҟo���֏ͣ��Ƿ��^�m��", L"��ʾ", MB_YESNO|MB_ICONINFORMATION);
		if (nRst == IDNO)
		{
			return;
		}
		m_Wiper.Cancel(TRUE);
		CString DriveName;
		m_combox_disk.GetWindowText(DriveName);
		m_Wiper.CleanDiskFreeSpace(DriveName.GetBuffer(0), GetSafeHwnd());
		m_btn_start.SetWindowText(L"ȡ��");
		m_btn_pause.ShowWindow(SW_SHOW);
		m_combox_disk.EnableWindow(FALSE);
	}
	else
	{
		UINT nRst = MessageBox(L"�Ƿ�ȡ���űP����", L"��ʾ", MB_YESNO | MB_ICONINFORMATION);
		if (nRst == IDNO)
		{
			return;
		}
		m_Wiper.Cancel(FALSE);
		m_progress_wipe.SetPos(0);
		m_lable_info.SetWindowText(L"");
		m_btn_start.SetWindowText(L"�_ʼ����");
		m_btn_pause.ShowWindow(SW_HIDE);
		m_combox_disk.EnableWindow(TRUE);
	}	
}


void CDiskWipeDlg::OnBnClickedButtonPause()
{
	// TODO: Add your control notification handler code here
	CString temp;
	m_btn_pause.GetWindowText(temp); //�@ȡ�F�ڵİ��o����
	if (temp == L"��ͣ")
	{
		m_Wiper.Pause(TRUE);
		m_btn_pause.SetWindowText(L"�^�m");
	}
	else
	{
		m_Wiper.Pause(FALSE);
		m_btn_pause.SetWindowText(L"��ͣ");
	}
}


LRESULT CDiskWipeDlg::OnUpdateWipeProcess(WPARAM wParam, LPARAM lParam)
{
	PPROGRESS_WIPE_INFO p = (PPROGRESS_WIPE_INFO)wParam;
	WCHAR buf[64];
	wcscpy_s(buf, p->Tips);
	m_lable_info.SetWindowText(buf);
	m_progress_wipe.SetPos(p->nTotalWipeProgress);
	if (p->bFinished)
	{
		m_btn_start.SetWindowText(L"�_ʼ����");
		m_btn_pause.ShowWindow(SW_HIDE);
		m_combox_disk.EnableWindow(TRUE);
	}
	return TRUE;
}


void CDiskWipeDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	CString temp;
	m_btn_start.GetWindowText(temp);
	if (temp == L"ȡ��")
	{
		UINT nRst = MessageBox(L"�Ƿ�ȡ���űP����", L"��ʾ", MB_YESNO | MB_ICONINFORMATION);
		if (nRst == IDNO)
		{
			return;
		}
		m_Wiper.Cancel(FALSE);
	}
	CDialogEx::OnClose();
}

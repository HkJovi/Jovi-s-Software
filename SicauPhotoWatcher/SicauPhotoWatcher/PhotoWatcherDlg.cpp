// PhotoWatcherDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SicauPhotoWatcher.h"
#include "PhotoWatcherDlg.h"
#include "afxdialogex.h"
#include "afxinet.h"
#include "http.h"


// PhotoWatcherDlg �Ի���

IMPLEMENT_DYNAMIC(PhotoWatcherDlg, CDialog)

PhotoWatcherDlg::PhotoWatcherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PhotoWatcherDlg::IDD, pParent)
	, m_sXh(_T(""))
{
}

PhotoWatcherDlg::~PhotoWatcherDlg()
{
}

void PhotoWatcherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_XH, m_sXh);
	DDV_MaxChars(pDX, m_sXh, 8);
	DDX_Control(pDX, IDC_COMBONj, m_comboxNj);
	DDX_Control(pDX, IDC_infoxh, m_stacXh);
	DDX_Control(pDX, IDC_infoxm, m_stacXm);
	DDX_Control(pDX, IDC_infoxy, m_stacXy);
	//  DDX_Control(pDX, IDC_EDIT1, m_edtResult);
}


BEGIN_MESSAGE_MAP(PhotoWatcherDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BtnCx, &PhotoWatcherDlg::OnBnClickedBtncx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_bLast, &PhotoWatcherDlg::OnBnClickedblast)
	ON_BN_CLICKED(IDC_bNext, &PhotoWatcherDlg::OnBnClickedbnext)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BtnRandom, &PhotoWatcherDlg::OnBnClickedBtnrandom)
	ON_BN_CLICKED(IDC_cWaterwave, &PhotoWatcherDlg::OnBnClickedcwaterwave)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_bSave, &PhotoWatcherDlg::OnBnClickedbsave)
END_MESSAGE_MAP()


//UrlImg ��Ƭ��ȡ
HRESULT PhotoWatcherDlg::ShowPic(CString xh)
{
	//�ж��ļ��Ƿ����
	CFileFind fFind;
	BOOL bExist = fFind.FindFile("temp.jpg");
	if (bExist)
	{
		DeleteFile("temp.jpg");
	}
	fFind.Close();

	CString cstrImgUrl = "http://jiaowu.sicau.edu.cn/photo/" + xh + ".jpg";
	//����ͼƬ����
	CInternetSession session;
	CHttpFile *httpFile = (CHttpFile *)session.OpenURL(cstrImgUrl);
	CStdioFile imgFile;
	char buff[1024];    // ����  
	DWORD byte = (DWORD)httpFile->GetLength();
	imgFile.Open("temp.jpg", CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);

	DWORD dwStatusCode;
	httpFile->QueryInfoStatusCode(dwStatusCode);

	if (dwStatusCode == HTTP_STATUS_OK) {
		int size = 0;
		do {
			size = httpFile->Read(buff, 1024);    // ��ȡͼƬ  
			imgFile.Write(buff, size);
		} while (size > 0);
	}

	httpFile->Close();
	session.Close();
	imgFile.Close();

	//���������ļ�
	CFileStatus fs;
	CFile::GetStatus("temp.jpg", fs);
	fs.m_attribute = CFile::hidden;
	CFile::SetStatus("temp.jpg", fs);


	//���ļ�
	CImage image;
	HICON hIcon;
	HRESULT result = image.Load("temp.jpg");
	CDC* pDC = ((CWnd *)GetDlgItem(IDC_img))->GetDC();
	CBrush brush(RGB(240, 240, 240));
	CRect rect; //��ȡ��ʾ��Χ��С
	CWnd *pWnd = GetDlgItem(IDC_img);
	pWnd->GetWindowRect(&rect);
	pDC->FillRect(CRect(2, 2, rect.Width() - 2, rect.Height() - 2), &brush); //��ʼ���հײ���

	if (result)  //����Ƿ��ȡ��ͼƬ
	{
		hIcon = AfxGetApp()->LoadIconA(IDI_ICON2);
		pDC->DrawIcon(rect.Width()/2-10, rect.Height()/2-20 , hIcon);
		ReleaseDC(pDC);
		return S_FALSE;
	}
	//����ͼ����Сʧ��
	::SetStretchBltMode(pDC->m_hDC, HALFTONE);
	::SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	image.Draw(pDC->m_hDC, 3, 3, rect.Width() - 7, rect.Height() - 7);
	
	ReleaseDC(pDC);

	return S_OK;
}

void PhotoWatcherDlg::GetUserId(int flag)   //True����һ�� False��һ��
{
	if (m_sXh.GetLength() != 8 && (flag==1 ||flag ==2))
		return;
	int uid = _ttoi(m_sXh);
	switch (flag)
	{
	case 1:
		uid--;
		break;
	case 2:
		uid++;
		break;
	default:
		//�������ѧ��
		uid = 0;
		uid = rand() % 9990 ;
		CString nj;
		m_comboxNj.GetLBText(m_comboxNj.GetCurSel(), nj);
		uid += _ttoi(nj) * 10000;
		break;
	}
	CString temp;
	temp.Format("%d", uid);
	m_sXh = "123";
	m_sXh = temp;
	CWnd *pWnd = GetDlgItem(IDC_XH);
	pWnd->SetWindowTextA(m_sXh);
}

void PhotoWatcherDlg::DrowWaterText(CString text, CRect rect)
{
	CDC *pDC = ((CWnd *)GetDlgItem(IDC_img))->GetDC();
	//pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(text, &rect,  DT_SINGLELINE );
	ReleaseDC(pDC);
}

CString PhotoWatcherDlg::GetInfo()
{
	CString xh;
	CString xm;
	m_stacXh.GetWindowTextA(xh);
	m_stacXm.GetWindowTextA(xm);
	return xh + "  "+ xm;
}

void PhotoWatcherDlg::GetNameAndUnit()
{
	UpdateData();

	CString response ;
	CHttp http;

	//��ȡcookies
	response = http.get("http://202.115.182.3/gdweb/default.aspx");	
	response = http.post(("http://202.115.182.3/gdweb/ReaderLogin.aspx"), " ", ("ScriptManager1=UpdatePanel1|ImageButton1&__EVENTTARGET=&__EVENTARGUMENT=&__VIEWSTATE=%2FwEPDwUJNzQ4NzgzNjUwD2QWAgIDD2QWAgIFD2QWAmYPZBYGAgEPZBYCAgEPDxYCHgRUZXh0BesGPHRkIHN0eWxlPSJoZWlnaHQ6IDIxcHgiPjxBIGhyZWY9J2RlZmF1bHQuYXNweCc%2BPHNwYW4%2B6aaW6aG1PC9zcGFuPjwvQT48L3RkPjx0ZCBzdHlsZT0iaGVpZ2h0OiAyMXB4Ij48QSBocmVmPSdkZWZhdWx0LmFzcHgnPjxzcGFuPuS5puebruafpeivojwvc3Bhbj48L0E%2BPC90ZD48dGQgc3R5bGU9ImhlaWdodDogMjFweCI%2BPEEgaHJlZj0nTWFnYXppbmVDYW50b1NjYXJjaC5hc3B4Jz48c3Bhbj7mnJ%2FliIrnr4flkI08L3NwYW4%2BPC9BPjwvdGQ%2BPHRkIHN0eWxlPSJoZWlnaHQ6IDIxcHgiPjxBIGhyZWY9J1Jlc2VydmVkTGlzdC5hc3B4Jz48c3Bhbj7pooTnuqbliLDppoY8L3NwYW4%2BPC9BPjwvdGQ%2BPHRkIHN0eWxlPSJoZWlnaHQ6IDIxcHgiPjxBIGhyZWY9J0V4cGlyZWRMaXN0LmFzcHgnPjxzcGFuPui2heacn%2BWFrOWRijwvc3Bhbj48L0E%2BPC90ZD48dGQgc3R5bGU9ImhlaWdodDogMjFweCI%2BPEEgaHJlZj0nTmV3Qm9vS1NjYXJjaC5hc3B4Jz48c3Bhbj7mlrDkuabpgJrmiqU8L3NwYW4%2BPC9BPjwvdGQ%2BPHRkIHN0eWxlPSJoZWlnaHQ6IDIxcHgiPjxBIGhyZWY9J0FkdmljZXNTY2FyY2guYXNweCc%2BPHNwYW4%2B5oOF5oql5qOA57SiPC9zcGFuPjwvQT48L3RkPjx0ZCBzdHlsZT0iaGVpZ2h0OiAyMXB4Ij48QSBocmVmPSdDb21tZW5kTmV3Qm9va1NjYXJjaC5hc3B4Jz48c3Bhbj7mlrDkuablvoHorqI8L3NwYW4%2BPC9BPjwvdGQ%2BPHRkIHN0eWxlPSJoZWlnaHQ6IDIxcHgiPjxBIGhyZWY9J1JlYWRlckxvZ2luLmFzcHgnPjxzcGFuPuivu%2BiAheeZu%2BW9lTwvc3Bhbj48L0E%2BPC90ZD48dGQgc3R5bGU9ImhlaWdodDogMjFweCI%2BPEEgaHJlZj0nT25saW5lU3R1ZHkuYXNweCc%2BPHNwYW4%2B5Zyo57q%2F5ZKo6K%2BiL%2BWfueiurTwvc3Bhbj48L0E%2BPC90ZD5kZAIDD2QWBAICDw8WAh8ABTI8c3Bhbj7mrKLov47mgqg6R3Vlc3Qg6K%2B36YCJ5oup5L2g55qE5pON5L2cPC9zcGFuPmRkAgMPDxYCHgdWaXNpYmxlaGRkAgUPZBYCAgEPZBYCAgMPZBYCZg8QZBAVAwzlgJ%2Fkuabor4Hlj7cM6K%2B76ICF5p2h56CBBuWnk%2BWQjRUDDOWAn%2BS5puivgeWPtwzor7vogIXmnaHnoIEG5aeT5ZCNFCsDA2dnZ2RkGAEFHl9fQ29udHJvbHNSZXF1aXJlUG9zdEJhY2tLZXlfXxYBBQxJbWFnZUJ1dHRvbjFoCd0dA%2FM8nmanm%2BMWUnLxltmrEw%3D%3D&DropDownList1=%E5%80%9F%E4%B9%A6%E8%AF%81%E5%8F%B7&TextBox1="+ m_sXh +"&TextBox2=&__EVENTVALIDATION=%2FwEWCAKVlsqGDALgnZ70BALrr%2BCHBALntNySDgLwuLirBQLs0bLrBgLs0fbZDALSwpnTCINnD0bp0SEKc5MGS39%2FvwLUH8qY&ImageButton1.x=15&ImageButton1.y=9"));
	response = http.get("http://202.115.182.3/gdweb/ReaderTable.aspx");

	if (response.Find("�������")>=0 || response.Find("Guest") >=0 ||response.Find("���ߵ�¼")==-1||response.Find(m_sXh)==-1 )
	{
		m_stacXm.SetWindowTextA("��ȡʧ��");
		m_stacXy.SetWindowTextA("��ȡʧ��");
		return;
	}

	//����ץȡ
	int start = response.Find("\"LblreaderName\">");
	start = start + 16;
	int end = response.Find("</span>", start);
	CString name = response.Mid(start, end - start);
	m_stacXm.SetWindowTextA(name);

	//ѧԺץȡ
	start = response.Find("\"LblReaderUnit\">");
	start = start + 16;
	end = response.Find("</span>", start);
	CString unit = response.Mid(start, end - start);
	m_stacXy.SetWindowTextA(unit);
	//response = http.post("http://202.115.182.3/gdweb/ReaderTable.aspx", " ", "ScriptManager1=UpdatePanel1|CheckLogin1$LinkButton1&__EVENTTARGET=CheckLogin1%24LinkButton1&__EVENTARGUMENT=&__VIEWSTATE=%2FwEPDwUKMjEwMDkyODE5NA9kFgICAw9kFgICAw9kFgJmD2QWOAIBD2QWAgIBDw8WAh4EVGV4dAXABzx0ZCBzdHlsZT0iaGVpZ2h0OiAyMXB4Ij48QSBocmVmPSdkZWZhdWx0LmFzcHgnPjxzcGFuPummlumhtTwvc3Bhbj48L0E%2BPC90ZD48dGQgc3R5bGU9ImhlaWdodDogMjFweCI%2BPEEgaHJlZj0nZGVmYXVsdC5hc3B4Jz48c3Bhbj7kuabnm67mn6Xor6I8L3NwYW4%2BPC9BPjwvdGQ%2BPHRkIHN0eWxlPSJoZWlnaHQ6IDIxcHgiPjxBIGhyZWY9J01hZ2F6aW5lQ2FudG9TY2FyY2guYXNweCc%2BPHNwYW4%2B5pyf5YiK56%2BH5ZCNPC9zcGFuPjwvQT48L3RkPjx0ZCBzdHlsZT0iaGVpZ2h0OiAyMXB4Ij48QSBocmVmPSdSZXNlcnZlZExpc3QuYXNweCc%2BPHNwYW4%2B6aKE57qm5Yiw6aaGPC9zcGFuPjwvQT48L3RkPjx0ZCBzdHlsZT0iaGVpZ2h0OiAyMXB4Ij48QSBocmVmPSdFeHBpcmVkTGlzdC5hc3B4Jz48c3Bhbj7otoXmnJ%2FlhazlkYo8L3NwYW4%2BPC9BPjwvdGQ%2BPHRkIHN0eWxlPSJoZWlnaHQ6IDIxcHgiPjxBIGhyZWY9J05ld0Jvb0tTY2FyY2guYXNweCc%2BPHNwYW4%2B5paw5Lmm6YCa5oqlPC9zcGFuPjwvQT48L3RkPjx0ZCBzdHlsZT0iaGVpZ2h0OiAyMXB4Ij48QSBocmVmPSdBZHZpY2VzU2NhcmNoLmFzcHgnPjxzcGFuPuaDheaKpeajgOe0ojwvc3Bhbj48L0E%2BPC90ZD48dGQgc3R5bGU9ImhlaWdodDogMjFweCI%2BPEEgaHJlZj0nQ29tbWVuZE5ld0Jvb2tTY2FyY2guYXNweCc%2BPHNwYW4%2B5paw5Lmm5b6B6K6iPC9zcGFuPjwvQT48L3RkPjx0ZCBzdHlsZT0iaGVpZ2h0OiAyMXB4Ij48QSBocmVmPSdSZWFkZXJMb2dpbi5hc3B4Jz48c3Bhbj7or7vogIXnmbvlvZU8L3NwYW4%2BPC9BPjwvdGQ%2BPHRkIHN0eWxlPSJoZWlnaHQ6IDIxcHgiPjxBIGhyZWY9J09ubGluZVN0dWR5LmFzcHgnPjxzcGFuPuWcqOe6v%2BWSqOivoi%2Fln7norq08L3NwYW4%2BPC9BPjwvdGQ%2BPHRkIHN0eWxlPSJoZWlnaHQ6IDIxcHgiPjxBIGhyZWY9J1JlYWRlclRhYmxlLmFzcHgnPjxzcGFuPuivu%2BiAheeuoeeQhjwvc3BhbjwvYT48L3RkPmRkAgUPZBYCAgIPDxYCHwAFNzxzcGFuPiDmrKLov47mgqg6546L5a6J6ZuoIOivt%2BmAieaLqeS9oOeahOaTjeS9nDwvc3Bhbj5kZAIJDw8WAh8ABQoyMDE0MDAwNSAgZGQCCw8PFgIfAAUKMjAxNDAwMDUgIGRkAg0PDxYCHwAFCDIwMTQwMDA1ZGQCDw8PFgIeCEltYWdlVXJsBRRJbWFnZXMvcmVhZGVycGhvLmdpZmRkAhEPDxYCHwAFCeeOi%2BWuiembqGRkAhMPDxYCHwAFA%2BWls2RkAhUPDxYCHwAFEuS%2FoeaBr%2BW3peeoi%2BWtpumZomRkAhcPDxYCHwAFCOWtpiAg55SfZGQCGQ8PFgIfAAUGJm5ic3A7ZGQCGw8PFgIfAAUGJm5ic3A7ZGQCHQ8PFgIfAAUGJm5ic3A7ZGQCHw8PFgIfAGVkZAIhDw8WAh8AZWRkAiMPDxYCHwBlZGQCJQ8PFgIfAGVkZAInDw8WAh8AZWRkAikPDxYCHwBlZGQCKw8PFgIfAAUGMC4wMDAwZGQCLQ8PFgIfAAUGMC4wMDAwZGQCLw8PFgIfAAUBMGRkAjMPD2QWAh4Hb25jbGljawUrcmV0dXJuIGNvbmZpcm0oJ%2BehruiupOS9oOeahOS%2FruaUueWQl%2B%2B8nycpO2QCNw8PFgIfAAUY5pqC5peg5YCf5Lmm6K6w5b2VLi4uLi4uZGQCOQ88KwALAQE8KwAHAQQ8KwAEAQAWAh4HVmlzaWJsZWdkAkEPDxYCHwAFGOaaguaXoOmihOe6puWbvuS5pi4uLi4uLmRkAkMPPCsACwBkAkUPZBYCZg9kFgRmD2QWAgIBD2QWAgIBD2QWAmYPZBYCAgUPD2QWAh8CBTRyZXR1cm4gY29uZmlybSgn56Gu6K6k6KaB5L%2Bd5a2Y5L2g55qE5L%2Bu5pS55ZCX77yfJyk7ZAIBD2QWAgIBD2QWAgIBD2QWAmYPZBYCAgMPD2QWAh8CBS5yZXR1cm4gY29uZmlybSgn56Gu6K6k6KaB5oyC5aSx6K%2BB5Lu256CB77yfJyk7ZGSgX9Gek6cBJC4ZS%2BacybF78ly6yQ%3D%3D&TextBox2=&TextBox3=&TextBox4=&__EVENTVALIDATION=%2FwEWDgLxq8%2FvCgKrhfywDgL7hryJDwLgnZ70BAKW6avNDQLdmpmqCgKQ9M%2FrBQLXqL7fDwLwmLemDQLs0fbZDALs0Yq1BQL2%2F4n8CALs0e58Aovpt48JfEoYcwz%2Fj5kOIfVuzY%2BgUYd0rs0%3D&");
	//response = http.get("http://202.115.182.3/gdweb/default.aspx");
}
// PhotoWatcherDlg ��Ϣ�������

int PhotoWatcherDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	//�������
	srand((unsigned)time(NULL)); 

	//����ͼ��
	m_hIcon = AfxGetApp()->LoadIconA(IDI_ICON1);
	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	return 0;
}


HBRUSH PhotoWatcherDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	CRect rect;
	// TODO:  �ڴ˸��� DC ���κ�����  �ػ���ɫ
	
	if (pWnd->GetDlgCtrlID()==IDC_STATIC1) 
	{
		pDC->SetTextColor(RGB(0, 0, 255));
	}
	else if (pWnd->GetDlgCtrlID() == IDC_infoxh || pWnd->GetDlgCtrlID() == IDC_infoxm || pWnd->GetDlgCtrlID() == IDC_infoxy) //������Ϣ��ɫ
	{
		pDC->SetTextColor(RGB(255, 0, 255));
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void PhotoWatcherDlg::OnBnClickedBtncx()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	if (m_sXh.GetLength()!=8)
	{
		MessageBox("��������ȷ��8λѧ��!", "��ʾ", MB_OK);
		return;
	}
	//ȡ����ϢˮӡCheckbox
	CButton *btn = (CButton *)GetDlgItem(IDC_cWaterwave);
	btn->SetCheck(0);

	CWnd *pWnd = GetDlgItem(IDC_infoxh); //���ѧ��
	pWnd->SetWindowTextA(m_sXh);

	m_stacXm.SetWindowTextA("��ȡ��...");
	m_stacXy.SetWindowTextA("��ȡ��...");
	ShowPic(m_sXh);
	GetNameAndUnit();
}


void PhotoWatcherDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CFileFind fFind;
	if (fFind.FindFile("temp.jpg"))
	{
		DeleteFile("temp.jpg");
	}
	CDialog::OnClose();
}


void PhotoWatcherDlg::OnBnClickedblast()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetUserId(1);
	OnBnClickedBtncx();
}


void PhotoWatcherDlg::OnBnClickedbnext()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetUserId(2);
	OnBnClickedBtncx();
}

//��д������Ϣ
BOOL PhotoWatcherDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam==VK_ESCAPE) //����esc��,ENTER����OK����д
	{
		return TRUE;
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

//��׽����
void PhotoWatcherDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch (nChar)
	{
	case VK_LEFT:
		OnBnClickedblast();
		break;
	case VK_RIGHT:
		OnBnClickedbnext();
		break;
	//�س�������д
	//case VK_RETURN:
	//	OnBnClickedBtncx();
	//	break;
	default:
		break;
	}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

//��д�س�����Ϣ
void PhotoWatcherDlg::OnOK()
{
	OnBnClickedBtncx();
}


void PhotoWatcherDlg::OnBnClickedBtnrandom()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetUserId(0);
	OnBnClickedBtncx();
}


BOOL PhotoWatcherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SkinH_AttachResEx(MAKEINTRESOURCE(IDR_SHE1), _T("SHE"), _T(""), 0, 0, 0);
	//Combox��
	m_comboxNj.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void PhotoWatcherDlg::OnBnClickedcwaterwave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CButton *btn = (CButton *)GetDlgItem(IDC_cWaterwave);
	int state = btn->GetCheck();
	if (state == 1)
	{
		//��ѡ��
		CRect rect;
		CWnd *pWnd = (CWnd *)GetDlgItem(IDC_img);
		pWnd->GetWindowRect(&rect);
		DrowWaterText(GetInfo(), CRect(rect.Width()-140 , rect.Height()-22, rect.Width() - 3, rect.Height() - 3));
	}
	else
	{
		//δ��ѡ��
		if (m_sXh.GetLength()==8)
		OnBnClickedBtncx();
	}
}


void PhotoWatcherDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	CImage image;
	CDC* pDC = ((CWnd *)GetDlgItem(IDC_img))->GetDC();
	HRESULT result = image.Load("temp.jpg");
	if (result)   //�����޷�����ͼƬʱ��Ĵ���
		return;
	CBrush brush(RGB(240, 240, 240));
	CRect rect; //��ȡ��ʾ��Χ��С
	CWnd *pWnd = GetDlgItem(IDC_img);
	pWnd->GetWindowRect(&rect);
	pDC->FillRect(CRect(2, 2, rect.Width() - 2, rect.Height() - 2), &brush); //��ʼ���հײ���

	//����ͼ����Сʧ��
	::SetStretchBltMode(pDC->m_hDC, HALFTONE);
	::SetBrushOrgEx(pDC->m_hDC, 0, 0, NULL);
	image.Draw(pDC->m_hDC, 3, 3, rect.Width() - 7, rect.Height() - 7);

	ReleaseDC(pDC);
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}


void PhotoWatcherDlg::OnBnClickedbsave()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString posation;
	CString name;
	CFileDialog filedlg(false);
	filedlg.m_ofn.lpstrFilter = "JPEG(*.jpg)\0*.jpg\0\0)";
	//δ�ҵ�ͼƬ����
	CFileFind fFind;
	BOOL bExist = fFind.FindFile("temp.jpg");
	if (!bExist)	
		return;
	fFind.Close();

	if (IDOK == filedlg.DoModal())
	{
		posation = filedlg.GetPathName();
		name = filedlg.GetFileName();
		posation = posation + ".jpg";
		CopyFile("temp.jpg", posation, false);
		//�ָ�������
		CFileStatus fs;
		CFile::GetStatus(posation, fs);
		fs.m_attribute = CFile::normal;
		CFile::SetStatus(posation, fs);
		::MessageBox(NULL, "����ɹ�", "��ʾ", MB_OK | MB_ICONINFORMATION);
	}
	return;
}

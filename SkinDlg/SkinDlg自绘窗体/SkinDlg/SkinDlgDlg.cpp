
// SkinDlgDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SkinDlg.h"
#include "SkinDlgDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSkinDlg �Ի���



CSkinDlg::CSkinDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSkinDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSkinDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CSkinDlg ��Ϣ�������

BOOL CSkinDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//������Ӱ
	SetClassLong(this->m_hWnd, GCL_STYLE, GetClassLong(this->m_hWnd, GCL_STYLE) | CS_DROPSHADOW);

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

void CSkinDlg::OnPaint()
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
		//������
		CPaintDC dc2(this);
		CRect rect;
		GetClientRect(rect);

		//������ͼ
		dc2.SetBkMode(TRANSPARENT);
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc2);
		CBitmap Bitmap;
		BITMAP bmpinfo;
		Bitmap.LoadBitmap(IDB_BK1);
		Bitmap.GetObject(sizeof(bmpinfo),& bmpinfo);  //��ȡͼƬ��Ϣ
		CBitmap *pOldBitmap = MemDC.SelectObject(&Bitmap);
		
		dc2.StretchBlt(-1, -1, rect.Width()+2, rect.Height()+2, &MemDC, 0, 0, bmpinfo.bmWidth, bmpinfo.bmHeight, SRCCOPY);  //����λͼ
//		dc2.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, rect.left, rect.top, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
		MemDC.DeleteDC();

		//��߿�
		CPen *oldpen = NULL;
		CPen newpen(PS_SOLID, 1, RGB(27, 147, 186));
		oldpen = dc2.SelectObject(&newpen);

		dc2.MoveTo(rect.left, CORNER_SIZE);
		dc2.LineTo(CORNER_SIZE, rect.top);
		dc2.LineTo(rect.right - CORNER_SIZE - 1, rect.top);
		dc2.LineTo(rect.right - 1, CORNER_SIZE);
		dc2.LineTo(rect.right - 1, rect.bottom - CORNER_SIZE - 1);
		dc2.LineTo(rect.right - CORNER_SIZE - 1, rect.bottom - 1);
		dc2.LineTo(CORNER_SIZE, rect.bottom - 1);
		dc2.LineTo(rect.left, rect.bottom - CORNER_SIZE - 1);
		dc2.LineTo(rect.left, CORNER_SIZE);

		//����ȱ��
		dc2.MoveTo(rect.left + 1, CORNER_SIZE);
		dc2.LineTo(CORNER_SIZE + 1, rect.top);

		dc2.MoveTo(rect.right - CORNER_SIZE - 1, rect.top + 1);
		dc2.LineTo(rect.right - 1, CORNER_SIZE + 1);

		dc2.MoveTo(rect.right - 2, rect.bottom - CORNER_SIZE - 1);
		dc2.LineTo(rect.right - CORNER_SIZE - 1, rect.bottom - 1);

		dc2.MoveTo(CORNER_SIZE, rect.bottom - 2);
		dc2.LineTo(rect.left, rect.bottom - CORNER_SIZE - 2);

		dc2.SelectObject(oldpen);

		//�ڱ߿�
		CPen newpen2(PS_SOLID, 1, RGB(196, 234, 247));
		oldpen = dc2.SelectObject(&newpen2);

		dc2.MoveTo(rect.left + 1, CORNER_SIZE + 1);
		dc2.LineTo(CORNER_SIZE + 1, rect.top + 1);
		dc2.LineTo(rect.right - CORNER_SIZE - 2, rect.top + 1);
		dc2.LineTo(rect.right - 2, CORNER_SIZE + 1);
		dc2.LineTo(rect.right - 2, rect.bottom - CORNER_SIZE - 2);
		dc2.LineTo(rect.right - CORNER_SIZE - 2, rect.bottom - 2);
		dc2.LineTo(CORNER_SIZE + 1, rect.bottom - 2);
		dc2.LineTo(rect.left + 1, rect.bottom - CORNER_SIZE - 2);
		dc2.LineTo(rect.left + 1, CORNER_SIZE + 1);
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSkinDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSkinDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (nType != SIZE_MAXIMIZED)
	{
		CRect rc;
		GetClientRect(&rc);

		CRgn rgn;
		CPoint points[8] = { CPoint(rc.left, CORNER_SIZE), CPoint(CORNER_SIZE, rc.top),
			CPoint(rc.right - CORNER_SIZE, rc.top), CPoint(rc.right, CORNER_SIZE),
			CPoint(rc.right, rc.bottom - CORNER_SIZE - 1), CPoint(rc.right - CORNER_SIZE - 1, rc.bottom),
			CPoint(CORNER_SIZE + 1, rc.bottom), CPoint(rc.left, rc.bottom - CORNER_SIZE - 1) };
		int nPolyCounts[1] = { 8 };
		int dd = rgn.CreatePolyPolygonRgn(points, nPolyCounts, 1, WINDING);
		SetWindowRgn(rgn, TRUE);
	}
	else
	{
		SetWindowRgn(NULL, FALSE);
	}
	// TODO:  �ڴ˴������Ϣ����������
}



void CSkinDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	CDialogEx::OnLButtonDown(nFlags, point);
}

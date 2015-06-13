// SkinStatic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SkinDlg.h"
#include "SkinStatic.h"


// CSkinStatic

IMPLEMENT_DYNAMIC(CSkinStatic, CStatic)

CSkinStatic::CSkinStatic()
{
	m_crText = RGB(250,250,250);          // ������ɫ  
	m_hBrush = (HBRUSH)GetStockObject(NULL_BRUSH);          // ��ˢ  
    m_bTran = TRUE;            // �Ƿ�͸��  
	// ͨ���õ�������õ������������Ϣ
	HFONT hfont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	m_font.FromHandle(hfont);
	::GetObject(hfont, sizeof(LOGFONT), &m_lf);
	::DeleteObject(hfont);
}

CSkinStatic::~CSkinStatic()
{
}


BEGIN_MESSAGE_MAP(CSkinStatic, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()


HBRUSH CSkinStatic::CtlColor(CDC* pDC, UINT nCtlColor)
{
	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  �����Ӧ���ø����Ĵ�������򷵻ط� null ����
	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SelectObject(&m_font);
		pDC->SetTextColor(m_crText);
		pDC->SetBkColor(m_crBackColor);
		pDC->SelectObject(&m_font);
		if (m_bTran == TRUE)
			pDC->SetBkMode(TRANSPARENT);
	}
	return m_hBrush;
}

void CSkinStatic::SetTextColor(COLORREF crText)
{
	m_crText = crText;
	RedrawWindow();
}

void CSkinStatic::SetFontSize(int nSize)
{
	nSize *= -1;
	m_lf.lfHeight = nSize;
	m_font.DeleteObject();
	m_font.CreateFontIndirect(&m_lf);
	RedrawWindow();
}

void CSkinStatic::SetBackColor(COLORREF crBackColor)
{
	m_crBackColor = crBackColor;
	RedrawWindow();
}

void CSkinStatic::SetTransparent(BOOL bTran)
{
	m_bTran = bTran;
	RedrawWindow();
}
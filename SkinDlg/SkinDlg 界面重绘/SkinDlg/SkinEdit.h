#pragma once
#define  SKINEDIT_CORNER_SIZE 1

// CSkinEdit

class CSkinEdit : public CEdit
{
	DECLARE_DYNAMIC(CSkinEdit)

public:
	CSkinEdit();
	virtual ~CSkinEdit();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void PreSubclassWindow();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);

private:
	CFont* m_pFont; // ����ָ��
	int m_nFontSize; // �����С
	CString m_csFontName; // ��������
	COLORREF m_clrText; // �ı���ɫ
	COLORREF m_clrBk; // ������ɫ
	BOOL m_bItalic; // б��
	BOOL m_bBold; // ����
	BOOL m_bUnderLine; // �»���
	CBrush m_bkBrush; // ������ˢ
	BOOL m_bInitEdit;  //��һ�μ���

protected:
	void SetFontHelper(); // ���´�������������
	void DrawBkAndFrame(); // �������ͱ߿�

public:
	void SetTextColor(COLORREF clrText); // �����ı���ɫ
	void SetBkColor(COLORREF clrBk); // ���ñ�����ɫ
	void SetNewFont(int nFontSize, CString csFontName = _T("΢���ź�")); // ����������
	void SetFontStyle(BOOL bBold = TRUE, BOOL bItalic = FALSE, BOOL bUnderLine = FALSE); // ����������ʽ
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};



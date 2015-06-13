#pragma once


// CSkinStatic

class CSkinStatic : public CStatic
{
	DECLARE_DYNAMIC(CSkinStatic)

public:
	CSkinStatic();
	virtual ~CSkinStatic();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	void SetTextColor(COLORREF crText);
	void SetFontSize(int nSize);
	void SetBackColor(COLORREF crBackColor);
	void SetTransparent(BOOL bTran);
private:
	COLORREF m_crText;          // ������ɫ  
	COLORREF m_crBackColor;     // ������ɫ  
	HBRUSH   m_hBrush;          // ��ˢ  
	LOGFONT  m_lf;              // �����С 
	CFont*   m_pOldFont;        //����ϵͳ����
	CFont    m_font;            // ����  
	BOOL     m_bTran;            // �Ƿ�͸��  
};



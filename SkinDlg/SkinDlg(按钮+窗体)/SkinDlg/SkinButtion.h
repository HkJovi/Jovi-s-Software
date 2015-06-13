#pragma once
#include "atlimage.h"

// CSkinButtion
typedef enum SKIN_BTNSTATE  //���״̬
{
	SKIN_BTN_NOR,
	SKIN_BTN_FOCUS,
	SKIN_BTN_PRESS,
	SKIN_BTN_DIS
};

typedef struct _SKINPNGINFO_  //��ťͼƬ�ṹ
{
	int nWidth;
	int nHeight;
	CImage* pImg;
}SKINPNGINFO;



class CSkinButton : public CButton
{
	DECLARE_DYNAMIC(CSkinButton)

public:
	CSkinButton();
	virtual ~CSkinButton();
	//���뱳��ͼ
	void Load(UINT IDBkGroup, int width = 0, int height = 0, const CString & resourceType = _T("PNG"));
	//����Ӧ����ͼ
	void SetAutoSize(bool bAutoSize);
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual void PreSubclassWindow();

private:
	//���ְ�ť״̬
	SKINPNGINFO m_btninfoNor;
	SKINPNGINFO m_btninfoFocus;
	SKINPNGINFO m_btninfoPress;
	SKINPNGINFO m_btninfoDis;
	BOOL m_bFocus;
	BOOL m_bPress;
	BOOL m_bAutoSize;

private:
	void DrawBK(HDC dc, CImage *image, SKIN_BTNSTATE btnstate);
	void DrawBtnText(HDC dc ,const CString & strText, int nMove, SKIN_BTNSTATE btnstate);
};



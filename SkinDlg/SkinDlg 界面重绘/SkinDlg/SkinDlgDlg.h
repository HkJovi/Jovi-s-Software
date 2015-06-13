
// SkinDlgDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "SkinButtion.h"
#include "SkinEdit.h"
#include "SkinStatic.h"


// CSkinDlg �Ի���
class CSkinDlg : public CDialogEx
{
// ����
public:
	CSkinDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SKINDLG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CSkinButton m_btnOK;
	CSkinButton m_btnCancel;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	CSkinButton m_btnClose;
	afx_msg void OnBnClickedbtnclose();
	CSkinButton m_btnMini;
	afx_msg void OnBnClickedbtnmini();
	CSkinEdit m_edt1;
	CSkinStatic m_static1;
};

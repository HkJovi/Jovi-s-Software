#pragma once


// HttpClientDlg �Ի���

class HttpClientDlg : public CDialog
{
	DECLARE_DYNAMIC(HttpClientDlg)

public:
	HttpClientDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~HttpClientDlg();

// �Ի�������
	enum { IDD = IDD_HttpClient };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_Url;
	CString m_response;
	afx_msg void OnBnClickedButton1();
};

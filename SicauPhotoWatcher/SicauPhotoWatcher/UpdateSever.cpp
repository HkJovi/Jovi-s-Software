#include "stdafx.h"
#include "UpdateSever.h"


UpdateSever::UpdateSever()
{

}

UpdateSever::~UpdateSever()
{

}

//��ȡ�汾��Ϣ
void UpdateSever::GetVersionInfo(VerInfo *pVerinfo)
{
	CHttp http;
	CString response;
	response = http.get("http://blog.sina.com.cn/s/blog_a62456420101w0j5.html");
	if (response == "" || response.Find("�汾��:[") == -1 || response.Find("���ص�ַ:[") == -1 || response.Find("��������:[") == -1 || response.Find("ǿ�ư汾:[") == -1 || response.Find("����˵��:[") == -1)
	{
		return;
	}
	int start = response.Find("�汾��:[");
	int end = response.Find("]", start);
	start += 8;
	pVerinfo->Version = response.Mid(start , end - start);

	start = response.Find("���ص�ַ:[", end);
	end = response.Find("]", start);
	start += 10;
	pVerinfo->DownloadUrl = response.Mid(start, end - start);

	start = response.Find("��������:[", end);
	end = response.Find("]", start);
	start += 10;
	pVerinfo->Date = response.Mid(start, end - start);

	start = response.Find("ǿ�ư汾:[", end);
	end = response.Find("]", start);
	start += 10;
	pVerinfo->CanNotUseOld = response.Mid(start, end - start);

	start = response.Find("����˵��:[", end);
	end = response.Find("]", start);
	start += 10;
	pVerinfo->Introduce = response.Mid(start, end - start);
	return;
}

//���и��¼��
bool UpdateSever::CheckVeision(VerInfo *pOldVerinfo,VerInfo *pVerinfo)
{
	if ((_ttoi(pOldVerinfo->Date)-_ttoi(pVerinfo->Date))<0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//���и�����ʾ
void UpdateSever::DoUpdate(VerInfo *pVerinfo)
{
	CString tips;
	if (pVerinfo->CanNotUseOld=="��")
	{
		tips = "�ɰ汾�Կɼ���ʹ�á�";
	}
	else
	{
		tips = "�ɰ汾���ܼ���ʹ�ã�";
	}

	if (IDOK == MessageBox(NULL, "�汾��:     "+pVerinfo->Version+"\n��������:  "+pVerinfo->Date+"\n����˵��:  "+pVerinfo->Introduce+"\n\n"+tips+"\n���ڸ�����", "�����°汾", MB_OKCANCEL |MB_ICONINFORMATION))
	{
		ShellExecute(NULL, "open", pVerinfo->DownloadUrl, NULL, NULL, SW_SHOWNORMAL);
		exit(0);  //��������
	}
	else if (pVerinfo->CanNotUseOld == "��")
	{
		MessageBox(NULL, "����º�ʹ�ã�", "����", MB_OK|MB_ICONERROR);
		exit(0); //ǿ�ư汾ʱ�����¾ɰ治����
	}
}
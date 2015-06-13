#pragma once


#include "Http.h"

class VerInfo
{
public:
	CString Version;
	CString Date;
	CString DownloadUrl;
	CString CanNotUseOld = "��"; //Ĭ�Ͽ����ϰ汾
	CString Introduce;
};

class UpdateSever
{
public:
	UpdateSever();
	~UpdateSever();
	void GetVersionInfo(VerInfo *pVerinfo);
	bool CheckVeision(VerInfo *pOldVerinfo, VerInfo *pVersion);
	void DoUpdate(VerInfo *pVerinfo);
};


#include "stdafx.h"
#include "EraseCore.h"
#include "UnlockCore.h"
#include <psapi.h> 
#pragma comment(lib, "psapi.lib") 

CFileErase::CFileErase()
	:m_hThread(INVALID_HANDLE_VALUE),
	m_nTotalShrededSize(0),
	m_nTotalSize(0),
	m_bContinue(TRUE),
	m_strRoot(L""),
	m_strCurrentFile(L""),
	m_nEraseCount(3)
{
	ZeroMemory(&m_infoProgress, sizeof(m_infoProgress));
	ZeroMemory(&m_infoShredRusult, sizeof(m_infoShredRusult));
	m_dwThreadId = GetCurrentThreadId();
	CleanBufferVirtualAlloc();
}

CFileErase::~CFileErase()
{
	if (WAIT_OBJECT_0 != ::WaitForSingleObject(m_hThread, 1000)){
		::TerminateThread(m_hThread, -1);
	}
	for (size_t i = 0; i < 5; i++)
	{
		VirtualFree(m_cleanBuffer[i], 0, MEM_RELEASE);
		m_cleanBuffer[i] = NULL;
	}
}

E_RESULT CFileErase::ShredPath(WCHAR *DirName, HWND hWnd)
{
	FILE_ERASE_INFO fei = { 0 };
	m_strRoot = DirName;
	m_VecFiles.clear();
	m_nTotalShrededSize = 0;
	m_nTotalSize = 0;
	if (!(m_strRoot.length() > 3 && m_strRoot[1] == ':' && m_strRoot[2] == '\\' && m_strRoot.length() <MAX_PATH /*&&  IsWindow(hWnd)*/))  //�ж��ļ�·���Ƿ�Ϸ�
	{
		return FAILE;
	}
	m_hWnd = hWnd;
	CFileEraseHelper::GetEraseInfo(DirName, m_VecFiles, m_nTotalSize);
	if (m_VecFiles.size() == 0)
	{
		wcscpy_s(fei.strFilePath, DirName);
		fei.Filetype = TYPE_UNKNOWN;
		fei.StatusCode = NOTEXIST;
		m_VecFailedFiles.push_back(fei);
		return FAILE;
	}

	return ShredFiles(m_VecFiles);
}

E_RESULT CFileErase::Cancel(BOOL bFlag)
{
	Pause(FALSE);
	m_bContinue = bFlag;
	return SUCCESS;
}

E_RESULT CFileErase::SetEraseCount(size_t n)
{
	if (n > 0 && n < 8)
	{
		m_nEraseCount = n;
		return SUCCESS;
	}
	return FAILE;
}

E_RESULT CFileErase::Pause(BOOL nFlag)
{
	if (m_hThread == INVALID_HANDLE_VALUE)
	{
		return FAILE;
	}
	if (nFlag)
	{
		//���𾀳�
		SuspendThread(m_hThread);
	}
	else
	{
		ResumeThread(m_hThread);
	}
	return SUCCESS;
}

//��������������ַ�
E_RESULT CFileErase::CleanBufferVirtualAlloc()
{
	srand((unsigned)time(0));
	for (size_t i = 0; i < 5; i++)
	{
		m_cleanBuffer[i] = (PBYTE)VirtualAlloc(NULL, CLEAN_BUF_SIZE, MEM_COMMIT, PAGE_READWRITE);
		if (m_cleanBuffer[i] == NULL)
		{
			for (size_t j = 0; j < i; j++)
			{
				VirtualFree(m_cleanBuffer[j], 0, MEM_RELEASE);
				m_cleanBuffer[j] = NULL;
			}
			return FAILE;
		}
		//��һ��00 �ڶ���FF �������Ժ������ 
		switch (i)
		{
		case 0:
			memset(m_cleanBuffer[i], 0, CLEAN_BUF_SIZE);
			break;
		case 1:
			memset(m_cleanBuffer[i], 0xFF, CLEAN_BUF_SIZE);
			break;
		case 2:
			for (int j = 0; j < CLEAN_BUF_SIZE; j++)
				m_cleanBuffer[i][j] = (BYTE)rand();
			break;
		case 3:
			for (int j = 0; j < CLEAN_BUF_SIZE; j++)
				m_cleanBuffer[i][j] = (BYTE)rand();
			break;
		case 4:
			for (int j = 0; j < CLEAN_BUF_SIZE; j++)
				m_cleanBuffer[i][j] = (BYTE)rand();
			break;
		}
	}
	return SUCCESS;
}

//������
E_RESULT CFileErase::SafeRename(const std::wstring& oldFileNamePath, std::wstring& newFileNamePath)
{
	std::wstring name = oldFileNamePath;
	int index = name.find_last_of('\\') + 1;
	//ԭ�ļ�����
	name = name.substr(0, index);
	CFileEraseHelper::GetRadomName(newFileNamePath);
	name.append(newFileNamePath);
	newFileNamePath = name;
	if (!MoveFile(oldFileNamePath.c_str(), name.c_str()))
	{
		return FAILE | RENAMEERROR;
	}
	return SUCCESS;
}

//ÿ�θ���length������2^31 - 1
E_RESULT CFileErase::SafeWrite(const HANDLE &hFile, const LONG &length)
{
	DWORD	dwTotalWritten = 0; //��д
	DWORD	dwPerWritten = 0;  //ÿ��д��
	DWORD   dwRealWriten = 0; //ʵ��д��

	for (size_t i = 0; i < m_nEraseCount; i++)
	{
		if (i != 0)
		{
			//����λ��
			SetFilePointer(hFile, -length, NULL, FILE_CURRENT);
		}
		dwTotalWritten = 0;
		ProcessShredProgress(m_infoProgress);
		while ((dwTotalWritten < (DWORD)length) && (m_bContinue))
		{
			dwPerWritten = (length - dwTotalWritten > 0x100000) ? 0x100000 : length - dwTotalWritten;  //ÿ��Ĭ��д��1M
			if (dwPerWritten > CLEAN_BUF_SIZE)
				dwPerWritten = CLEAN_BUF_SIZE;
			if (!WriteFile(hFile, m_cleanBuffer[CFileEraseHelper::GetRandNumber(0, 5)], dwPerWritten, &dwRealWriten, NULL))
				return FAILE | WRITINGERROR;
			dwTotalWritten += dwRealWriten;
			m_nTotalShrededSize += (dwRealWriten / m_nEraseCount) >> 10;
			m_infoProgress.lFileShredSize += (dwRealWriten / m_nEraseCount);
		}
		m_infoProgress.nFileProgress = (UINT32)(((double)m_infoProgress.lFileShredSize / (double)m_infoProgress.lFilesize) * 100);
		ProcessShredProgress(m_infoProgress);
	}
	return SUCCESS;
}

//�ļ���ȫɾ��
E_RESULT CFileErase::SafeDelete(const std::wstring& FileNamePath)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	UINT64 lTotalWritten = 0;   //��д
	LONG lPerWritten = 0x10000;
	LARGE_INTEGER Filelength = { 0 };

	// FILE_FLAG_WRITE_THROUGH   ָʾϵͳ����д���̡����д��������cache���棬�����ܱ����á�
	hFile = CreateFile(FileNamePath.c_str(), GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FAILE | WRITINGERROR;
	}

	GetFileSizeEx(hFile, &Filelength);
	m_infoProgress.lFilesize = Filelength.QuadPart;
	m_infoProgress.lFileShredSize = lTotalWritten;

	if (Filelength.QuadPart > 0)
	{
		while ((lTotalWritten < (unsigned)Filelength.QuadPart) && m_bContinue)
		{
			lPerWritten = (LONG)(min(Filelength.QuadPart - lTotalWritten, MAX_PER_BUFSIZE));
			if (!SafeWrite(hFile, lPerWritten))
			{
				CloseHandle(hFile);
				return FAILE | WRITINGERROR;
			}
			lTotalWritten += lPerWritten;
		}
		CloseHandle(hFile);
		return SUCCESS;
	}
	CloseHandle(hFile);
	return FAILE | DELETINGERROR;
}

//�����ļ�
E_RESULT CFileErase::ShredFiles(std::vector<FILE_ERASE_INFO> &VecFiles)
{
	ZeroMemory(&m_infoProgress, sizeof(m_infoProgress));
	m_VecFiles = VecFiles;
	DWORD dwThreadId = 0;
	m_hThread = ::CreateThread(NULL
		, 0
		, ShredThreadProc
		, this
		, 0
		, &dwThreadId);
	if (m_hThread == INVALID_HANDLE_VALUE)
	{
		return GetLastError();
	}
	return SUCCESS;
}
E_RESULT CFileErase::Shred()
{
	FILE_ERASE_INFO fei = { 0 };
	E_RESULT status = FAILE;
	std::wstring absolutefile;
	std::wstring newName;
	size_t RootPathLenth = MAX_PATH;
	m_nTotalShrededSize = 0;
	m_VecFailedFiles.clear();

	for (size_t i = 0; m_bContinue && i < m_VecFiles.size(); ++i)
	{
		ZeroMemory(&fei, sizeof(FILE_ERASE_INFO));
		absolutefile = m_VecFiles[i].strFilePath;
		wcscpy_s(m_infoProgress.wszFileName, absolutefile.c_str());
		//���´����ļ���
		m_strCurrentFile = absolutefile;
		//�ж��ļ�����
		if (m_VecFiles[i].Filetype == TYPE_FOLDER)
		{
			//���Ϊ�ļ��У����^
			continue;
		}
		/* _access(char *,int) �ж��ļ��Ƿ����
		���� ����0;������ ����-1.
		_access(const char *path,int mode)
		mode��ֵ:
		00 �Ƿ����
		02 дȨ��
		04 ��Ȩ��
		06 ��дȨ��
		*/
		//�ж��ļ�����
		if (_waccess_s(absolutefile.c_str(), 0))
		{
			m_VecFiles[i].StatusCode = NOTEXIST;
			m_VecFailedFiles.push_back(m_VecFiles[i]);
			continue;
		}

		//�z�y��ʽ�ļ��\����r
		int index = absolutefile.find_last_of('.');
		if (index != std::wstring::npos)
		{
			WCHAR ExpandedName[MAX_PATH];
			ZeroMemory(ExpandedName, MAX_PATH);
			wcscpy_s(ExpandedName, absolutefile.substr(index + 1).c_str());
			_wcslwr_s(ExpandedName, wcslen(ExpandedName) + 1);
			if (ExpandedName[0] == 'e' && ExpandedName[1] == 'x' && ExpandedName[2] == 'e')
			{
				CFileEraseHelper::KillProcess(absolutefile);
				Sleep(100);
			}
		}

		//�z��һ���ļ��Ī�����r
		HANDLE hFileUsed = CreateFile(absolutefile.c_str(), GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
		if (m_bContinue && hFileUsed == INVALID_HANDLE_VALUE)
		{
			//�ļ����׈��н��i
			CFileUnlockerHelper FileUnlockhelper;
			CFileUnlocker *pFileUnlocker = FileUnlockhelper.GetFileUnlocker(absolutefile.c_str());
			if (pFileUnlocker != NULL)
			{
				pFileUnlocker->UnlockAll();
				delete pFileUnlocker;
				pFileUnlocker = NULL;
			}
			else
			{
				m_VecFiles[i].StatusCode = UNLOCKERROR;
				m_VecFailedFiles.push_back(m_VecFiles[i]);
				continue;
			}
		}
		else
		{
			CloseHandle(hFileUsed);
		}

		//ȥ��ֻ������
		DWORD FileAttr = GetFileAttributes(absolutefile.c_str());
		if (FileAttr & FILE_ATTRIBUTE_READONLY)
		{
			//����ʧ��
			if (!SetFileAttributes(absolutefile.c_str(), 0))
			{
				m_VecFiles[i].StatusCode = ONLYREAD;
				m_VecFailedFiles.push_back(m_VecFiles[i]);
				continue;
			}
		}
		// ����HANDLE Ӧ����ȥ��ֻ��֮ǰ 
		//�����ļ�

		if (FileAttr == INVALID_FILE_ATTRIBUTES)
		{
			m_VecFiles[i].StatusCode = GETFILEINFOERROR;
			m_VecFailedFiles.push_back(m_VecFiles[i]);
			continue;
		}

		//�ظ�дĨȥ����
		if (!SafeDelete(absolutefile))
		{
			m_VecFiles[i].StatusCode = WRITINGERROR;
			m_VecFailedFiles.push_back(m_VecFiles[i]);
			continue;
		}

		//������26��	
		if (!m_bContinue)
		{
			return FAILE;
		}
		status = SafeRename(absolutefile, newName);
		if (status != SUCCESS)
		{
			m_VecFiles[i].StatusCode = RENAMEERROR;
			m_VecFailedFiles.push_back(m_VecFiles[i]);
			continue;
		}
		//ɾ���ļ�
		if (!m_bContinue)
		{
			return FAILE;
		}
		//�o���h���\���е�DLL�cexe��ʽ�ļ�
		if (!DeleteFile(newName.c_str()))
		{
			m_VecFiles[i].StatusCode = DELETINGERROR;
			m_VecFailedFiles.push_back(m_VecFiles[i]);
			continue;
		}
	}
	if (m_strRoot != L"")
	{
		//����ɾ����Ŀ¼
		CFileEraseHelper::DeleteDirectory(m_strRoot.c_str());
	}
	m_infoProgress.bFinished = TRUE;
	ProcessShredProgress(m_infoProgress);
	ReportShredResult();
	return SUCCESS;
}
DWORD WINAPI CFileErase::ShredThreadProc(LPVOID lpParameter)
{
	CFileErase* p = (CFileErase *)lpParameter;
	p->Shred();
	return 0;
}
void CFileErase::ProcessShredProgress(PROGRESS_INFO &info)
{
	wcscpy_s(m_infoProgress.wszFileName, m_strCurrentFile.c_str());
	info.nTotalProgress = (UINT32)(((double)m_nTotalShrededSize / (double)m_nTotalSize) * 100);
//	::PostThreadMessage(m_dwThreadId, WM_UPDATE_PROGRESS, (WPARAM)&info, 0);
	if (m_bContinue)
	{
		::SendMessage(m_hWnd, WM_UPDATE_PROGRESS, (WPARAM)&info, 0);
	}
}
void CFileErase::ReportShredResult()
{
	ZeroMemory(&m_infoShredRusult, sizeof(m_infoShredRusult));
	m_infoShredRusult.m_VecFailedFiles = m_VecFailedFiles;
	m_infoShredRusult.nFailedCount = m_VecFailedFiles.size();
	m_infoShredRusult.nTotalCount = m_VecFiles.size();
	m_infoShredRusult.nTotalShredSize = m_nTotalShrededSize;
//	::PostThreadMessage(m_dwThreadId, WM_UPDATE_SHRED_RESULT, (WPARAM)&m_infoShredRusult, 0);
	::SendMessage(m_hWnd, WM_UPDATE_SHRED_RESULT, (WPARAM)&m_infoShredRusult, 0);
}


//�õ��ļ���Ϣ
E_RESULT CFileEraseHelper::GetEraseInfo(WCHAR *wszPath, std::vector<FILE_ERASE_INFO> &VecFiles, UINT64 &nTotalFilesSize)
{
	if (FindFiles(wszPath, VecFiles, nTotalFilesSize))
	{
		return SUCCESS;
	}
	return FAILE;
}

int CFileEraseHelper::GetRandNumber(int range_min, int range_max)
{
	int u = (int)((double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min);
	return u;
}

E_RESULT CFileEraseHelper::GetRadomName(std::wstring &strRandname)
{
	WCHAR wszFileName[32] = { 0 };
	WCHAR wszTemp[8] = { 0 };

	for (size_t i = 0; i < 8; ++i)
	{
		wsprintfW(wszTemp, L"%02d", GetRandNumber(1, 256));
		wcscat_s(wszFileName, wszTemp);
	}
	strRandname = std::wstring(wszFileName);
	return SUCCESS;
}

BOOL  CFileEraseHelper::IsDirectory(const WCHAR *Dir)
{
	WCHAR szCurPath[MAX_PATH + 1];
	ZeroMemory(szCurPath, sizeof(MAX_PATH + 1));
	wcscpy_s(szCurPath, Dir);
	wcscat_s(szCurPath, L"\\*");
	WIN32_FIND_DATA FindFileData;
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATA));

	FILE_ERASE_INFO fei = { 0 };
	HANDLE hFile = FindFirstFile(szCurPath, &FindFileData);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		FindClose(hFile);
		return FALSE;
	}
	else
	{
		FindClose(hFile);
		return TRUE;
	}
}

BOOL  CFileEraseHelper::FindFiles(const WCHAR *DirName, std::vector<FILE_ERASE_INFO> &VecFiles, UINT64 &nTotalFilesSize)
{
	WCHAR szCurPath[MAX_PATH + 1];
	ZeroMemory(szCurPath, sizeof(szCurPath));
	WIN32_FIND_DATA FindFileData = { 0 };
	FILE_ERASE_INFO fei = { 0 };
	HANDLE hFile = INVALID_HANDLE_VALUE;
	//�ж��Ƿ�Ϊ�ļ�
	wcscpy_s(szCurPath, DirName);
	hFile = FindFirstFile(szCurPath, &FindFileData);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		wcscpy_s(fei.strFilePath, DirName);
		fei.StatusCode = INITIAL;
		fei.Filetype = TYPE_FILE;
		VecFiles.push_back(fei);
		nTotalFilesSize += CFileEraseHelper::GetShredFileSize(DirName);
		FindClose(hFile);
		return TRUE;
	}
	else
	{
		wcscpy_s(fei.strFilePath, DirName);
		fei.StatusCode = INITIAL;
		fei.Filetype = TYPE_FOLDER;
		VecFiles.push_back(fei);
		FindClose(hFile);
	}
	//�����ļ�
	wcscat_s(szCurPath, L"\\*.*"); //ƥ���ʽΪ*.*,����Ŀ¼�µ������ļ�  
	hFile = FindFirstFile(szCurPath, &FindFileData);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	while (TRUE)
	{
		if (wcscmp(FindFileData.cFileName, L".") && wcscmp(FindFileData.cFileName, L"..")) //�������"." ".."Ŀ¼  
		{
			std::wstring strFileName(L"");
			strFileName = strFileName + DirName + L"\\" + FindFileData.cFileName;
			std::wstring strTemp = strFileName;
			if (IsDirectory(strFileName.c_str())) //�����Ŀ¼����ݹ�ص���  
			{
				FindFiles(strTemp.c_str(), VecFiles, nTotalFilesSize);
			}
			else
			{
				wcscpy_s(fei.strFilePath, strFileName.c_str());
				fei.StatusCode = INITIAL;
				fei.Filetype = TYPE_FILE;
				nTotalFilesSize += CFileEraseHelper::GetShredFileSize(strFileName.c_str());
				VecFiles.push_back(fei);
			}
		}
		if (!FindNextFile(hFile, &FindFileData))
		{
			break;
		}
	}
	FindClose(hFile);
	return TRUE;
}

BOOL  CFileEraseHelper::DeleteDirectory(const WCHAR *DirName)
{
	std::wstring strDirname(DirName);
	if (strDirname == L"")
	{
		return FALSE;
	}
	WCHAR szCurPath[MAX_PATH + 1];
	ZeroMemory(szCurPath, sizeof(MAX_PATH + 1));
	wcscpy_s(szCurPath, DirName);
	wcscat_s(szCurPath, L"\\*.*");
	WIN32_FIND_DATA FindFileData;
	ZeroMemory(&FindFileData, sizeof(WIN32_FIND_DATA));
	HANDLE hFile = FindFirstFile(szCurPath, &FindFileData);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	while (TRUE)
	{
		if (wcscmp(FindFileData.cFileName, L".") && wcscmp(FindFileData.cFileName, L"..")) //�������"." ".."Ŀ¼  
		{
			std::wstring strFileName(L"");
			strFileName = strFileName + DirName + L"\\" + FindFileData.cFileName;
			std::wstring strTemp;
			strTemp = strFileName;
			if (IsDirectory(strFileName.c_str())) //�����Ŀ¼����ݹ�ص���  
			{
				DeleteDirectory(strTemp.c_str());
			}
		}
		if (!FindNextFile(hFile, &FindFileData))
		{
			break;
		}
	}
	FindClose(hFile);
	//�������ļ��A
	for (size_t i = 0; i < 26; i++)
	{
		std::wstring oldDirPath = strDirname;
		std::wstring newDirPath;
		int index = oldDirPath.find_last_of('\\') + 1;
		//ԭ�ļ�����
		oldDirPath = oldDirPath.substr(0, index);
		CFileEraseHelper::GetRadomName(newDirPath);
		oldDirPath.append(newDirPath);
		newDirPath = oldDirPath;
		MoveFile(strDirname.c_str(), newDirPath.c_str());
		strDirname = newDirPath;
	}
	BOOL bRet = RemoveDirectory(strDirname.c_str());
	if (bRet == FALSE) //ɾ��Ŀ¼  
	{
		return FALSE;
	}
	return TRUE;
}

UINT64 CFileEraseHelper::GetShredFileSize(const std::wstring& strFile)
{
	HANDLE hFile = CreateFile(strFile.c_str()
		, GENERIC_READ 
		, FILE_SHARE_READ 
		, NULL
		, OPEN_EXISTING
		, 0
		, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	LARGE_INTEGER nFileSize = { 0 };
	nFileSize.LowPart = GetFileSize(hFile, (LPDWORD)&nFileSize.HighPart);
	CloseHandle(hFile);
	return ((nFileSize.HighPart << 22) + (nFileSize.LowPart >> 10));  //����KB
}

E_RESULT CFileEraseHelper::KillProcess(std::wstring ApplicationPath)
{
	ULONG ulSize = 0x1000;
	PSYSTEM_PROCESS_INFORMATION pSystemProcessInformation = (PSYSTEM_PROCESS_INFORMATION)malloc(ulSize);
	NTSTATUS status;
	int index = ApplicationPath.find_last_of(L"/\\");
	if (index == std::string::npos)
	{
		return FAILE;
	}
	std::wstring ApplicationName = ApplicationPath.substr(index + 1);
	WCHAR Path[MAX_PATH];

	NTQUERYSYSTEMINFORMATION NtQuerySystemInformation = (NTQUERYSYSTEMINFORMATION)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtQuerySystemInformation");
	if (NtQuerySystemInformation == NULL)
	{
		return FAILE;
	}
	while ((status = NtQuerySystemInformation(SystemProcessInformation, pSystemProcessInformation, ulSize, &ulSize) == STATUS_INFO_LENGTH_MISMATCH))
	{
		pSystemProcessInformation = (PSYSTEM_PROCESS_INFORMATION)realloc(pSystemProcessInformation, ulSize);
	}
	while (pSystemProcessInformation->NextEntryOffset)
	{
		pSystemProcessInformation = (PSYSTEM_PROCESS_INFORMATION)((PCHAR)pSystemProcessInformation + pSystemProcessInformation->NextEntryOffset);
		if (pSystemProcessInformation->ProcessName.Length)
		{
			std::wstring temp(pSystemProcessInformation->ProcessName.Buffer);
			if (temp == ApplicationName)
			{
				HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pSystemProcessInformation->dUniqueProcessId);
				if (hProcess == INVALID_HANDLE_VALUE)
				{
					return FAILE;
				}
				GetModuleFileNameEx(hProcess, NULL, Path, sizeof(Path));
				if (!wcscmp(Path, ApplicationPath.c_str()))
				{
					TerminateProcess(hProcess, 0);
				}
				CloseHandle(hProcess);
			}
		}
	}
	return SUCCESS;
}
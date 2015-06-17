#include "StdAfx.h"
#include "WriteLogError.h"
#include "DoFile.h"
#include "DealTime.h"
#include "CommonConvert.h"

CWriteLogError::CWriteLogError(void)
{
	CDoFile doFile;
	m_strLogFilePath = doFile.MyGetExeFullFilePath();
	m_strLogFilePath.append(_T("\\log\\"));
	doFile.MyCreateDirectory(m_strLogFilePath);
}

CWriteLogError::~CWriteLogError(void)
{
}

void CWriteLogError::WriteErrLog(const std::wstring strSockLog)
{
	std::wstring str = _T("Error: ");
	str.append(strSockLog);
	WriteLogWithTime(str);
}

void CWriteLogError::WriteLogWithTime(const std::wstring& strSockLog)
{
	CDealTime time = CDealTime::GetCurrentTime();
	wchar_t wstrTime[255]={0};
	swprintf_s(wstrTime,_T("%d-%d-%d %d:%d:%d"),time.GetYear(),time.GetMonth(),time.GetDay(),
		time.GetHour(),time.GetMin(),time.GetSec());
	std::wstring wStrTime(wstrTime);
	wStrTime+=_T("\r\n");wStrTime+=strSockLog;wStrTime+=_T("\r\n\r\n");

	wchar_t wstrLogFile[255]={0};
	swprintf_s(wstrLogFile,_T("log_%d%d%d.log"),time.GetYear(),time.GetMonth(),time.GetDay());
	std::wstring wStrLogFile(wstrLogFile);
//	CString str = time.Format(_T("%Y-%m-%d %H:%M:%S")) + _T("  \t") + strSockLog + _T("\r\n\r\n");
//	CString strLogFile = time.Format(_T("log_%Y%m%d.log"));
// 	if(!CommonStrMethod::PathFileExist(m_strLogFilePath))
// 	{
// 		if(!CommonStrMethod::CreatePath(m_strLogFilePath))
// 		{
// 			return;
// 		}
// 	}
	
	AppendWriteFile(wStrTime, m_strLogFilePath + wStrLogFile);	
}

BOOL CWriteLogError::AppendWriteFile(const std::wstring& strText,const std::wstring& strFileName)
{
	/*
	CFile file;
	if(!file.Open(strFileName, 
		CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
	{
		//CString str;
		//str.Format(_T("创建或打开日志文件失败,您所使用的计算机帐号没有相应的磁盘写权限:\r\n\t%s"), g_pControl->m_strLogFilePath);
		//AfxMessageBox(str);
		return FALSE;
	}
	CHAR* szBuf = (CHAR*)malloc(strText.GetLength()*2 + 1);
	memset(szBuf, 0, strText.GetLength()*2 + 1);
	CommonStrMethod::WChar2Char(szBuf, 
		strText.GetLength()*2 + 1, strText.GetBuffer());
	file.SeekToEnd();
	file.Write(szBuf, strlen(szBuf));
	file.Close();
	*/
	CDoFile doFile;
	if(!doFile.MyOpenFile(strFileName,CDoFile::modeCreate | CDoFile::modeNoTruncate | CDoFile::modeWrite))
	{
		return FALSE;
	}
	int len = CCommonConvert::WStringToChar(strText,NULL,0);
	char* szBuf = new char[len+1];
	memset(szBuf,0,len+1);
	CCommonConvert::WStringToChar(strText,szBuf,len+1);
	doFile.MyFileSeekToEnd();
	BOOL flag = doFile.MyWriteFile(szBuf,len+1);
	doFile.MyCloseFile();
	return flag;
}
#include "stdafx.h"
#include "ConfigWnd.h"
#include "ConectToMySql.h"
#include "CommonConvert.h"

CConfigWnd::CConfigWnd(void){}
CConfigWnd::~CConfigWnd(){}
CDuiString CConfigWnd::m_strUserName = _T("");
CDuiString CConfigWnd::m_strPassWord = _T("");
CDuiString CConfigWnd::m_strIP = _T("");
CDuiString CConfigWnd::m_strPort = _T("");

void CConfigWnd::GetData()
{
	CEditUI* pName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("UserName")));
	CConfigWnd::m_strUserName = pName->GetText();
	CEditUI* pWord = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("ServicePassword")));
	CConfigWnd::m_strPassWord = pWord->GetText();
	CEditUI* pIPaddr = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("ServiceIP")));
	CConfigWnd::m_strIP = pIPaddr->GetText();
	CEditUI* pPort = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Port")));
	CConfigWnd::m_strPort = pPort->GetText();
}



LRESULT CConfigWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 屏蔽双击标题栏消息
	switch (uMsg)
	{
	case WM_NCLBUTTONDBLCLK:
		return 0;
		break;
	}
	return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CConfigWnd::InitWindow()
{
	// 读取配置文件 显示到界面
	CEditUI* pName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("UserName")));
	CEditUI* pWord = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("ServicePassword")));
	CEditUI* pIPaddr = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("ServiceIP")));
	CEditUI* pPort = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Port")));
	HANDLE hFile = CreateFile(L"..\\sqlconfig.dat",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	char buf[64];
	memset(buf,0,sizeof(buf));
	DWORD len = sizeof(buf);
	ReadFile(hFile,buf,len,&len,NULL);
	WCHAR text[128];
	CCommonConvert::Char2WChar(buf,sizeof(buf),text);
	pName->SetText(text);
	SetFilePointer(hFile,50,NULL,FILE_BEGIN);
	ReadFile(hFile,buf,len,&len,NULL);
	CCommonConvert::Char2WChar(buf,sizeof(buf),text);
	pWord->SetText(text);
	SetFilePointer(hFile,100,NULL,FILE_BEGIN);
	ReadFile(hFile,buf,len,&len,NULL);
	CCommonConvert::Char2WChar(buf,sizeof(buf),text);
	pIPaddr->SetText(text);
	SetFilePointer(hFile,150,NULL,FILE_BEGIN);
	ReadFile(hFile,buf,len,&len,NULL);
	CCommonConvert::Char2WChar(buf,sizeof(buf),text);
	pPort->SetText(text);
	CloseHandle(hFile);
}

void CConfigWnd::WriteConfig()
{
	HANDLE hFile = CreateFile(L"..\\sqlconfig.dat",GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	char buf[48];
	strcpy_s(buf,48,m_strUserName.GetStringA().c_str());
	DWORD len = strlen(buf)+1;
	WriteFile(hFile,buf,len,&len,NULL);
	SetFilePointer(hFile,50,NULL,FILE_BEGIN);
	strcpy_s(buf,48,m_strPassWord.GetStringA().c_str());
	len = strlen(buf)+1;
	WriteFile(hFile,buf,len,&len,NULL);
	SetFilePointer(hFile,100,NULL,FILE_BEGIN);
	strcpy_s(buf,48,m_strIP.GetStringA().c_str());
	len = strlen(buf)+1;
	WriteFile(hFile,buf,len,&len,NULL);
	SetFilePointer(hFile,150,NULL,FILE_BEGIN);
	strcpy_s(buf,48,m_strPort.GetStringA().c_str());
	len = strlen(buf)+1;
	WriteFile(hFile,buf,len,&len,NULL);
	CloseHandle(hFile);
}

void CConfigWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		CDuiString str = msg.pSender->GetName();
		if (str == _T("btn_Cancel"))
		{
			::DestroyWindow(m_hWnd);
		}
		else if (str == _T("btn_OK"))
		{
			GetData();
			// 将数据库配置信息写到配置文件中
			WriteConfig();
			::DestroyWindow(m_hWnd);
		}
	}
	return WindowImplBase::Notify(msg);
}
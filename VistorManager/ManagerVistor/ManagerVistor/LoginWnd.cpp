#include "stdafx.h"
#include "LoginWnd.h"
#include "CommonConvert.h"
#include "MainFrame.h"
#include "ConfigWnd.h"

CLoginWnd::CLoginWnd(void)
{
	m_IsLeftSelected = false;
	m_IsRightSeleced = false;
}
CLoginWnd::~CLoginWnd(){}
int CLoginWnd::m_iPower = STAFF_POWER;
CConectToMySql CLoginWnd::m_mySql;
CDuiString CLoginWnd::m_strAccount = _T("");
CDuiString CLoginWnd::m_strPassWord = _T("");

LRESULT CLoginWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

void CLoginWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType ==DUI_MSGTYPE_CLICK)
	{
		CDuiString str = msg.pSender->GetName();
		if ( str == _T("configbtn"))
		{
			CConfigWnd* pConfig = new CConfigWnd;
			pConfig->Create(NULL,_T("服务器配置"),UI_WNDSTYLE_FRAME,WS_EX_WINDOWEDGE);
			pConfig->CenterWindow();
			pConfig->ShowModal();
			delete pConfig;
		}
		else if ( str == _T("login"))
		{
			Login();
		}

	}
	if (msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		if (msg.pSender->GetName()==_T("autolog"))
		{
			COptionUI* pLeft = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("remember")));
			COptionUI* pRight = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("autolog")));
			if (!pLeft->IsSelected())
			{
				pLeft->Selected(true);
			}
		}
	}
	return WindowImplBase::Notify(msg);
}

void CLoginWnd::InitWindow()
{
	COptionUI* pLeft = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("remember")));
	COptionUI* pRight = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("autolog")));
	CEditUI* pName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("account")));
	CEditUI* pPassWord = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("password")));

	HANDLE hFile = CreateFile(L"..\\logstate.dat",GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_EXISTING,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	char account[128];
	memset(account,0,sizeof(account));
	char password[128];
	memset(password,0,sizeof(password));
	char left[32];
	memset(left,0,sizeof(left));
	char right[32];
	memset(right,0,sizeof(right));
	DWORD num = 0;
	ReadFile(hFile,account,sizeof(account),&num,NULL);
	SetFilePointer(hFile,150,NULL,FILE_BEGIN);
	ReadFile(hFile,password,sizeof(password),&num,NULL);
	SetFilePointer(hFile,300,NULL,FILE_BEGIN);
	ReadFile(hFile,left,sizeof(left),&num,NULL);
	SetFilePointer(hFile,350,NULL,FILE_BEGIN);
	ReadFile(hFile,right,sizeof(right),&num,NULL);
	CloseHandle(hFile);
	if (strcmp(left,"yes")==0)
	{
		pLeft->Selected(true);
		WCHAR wAccount[256];
		memset(wAccount,0,sizeof(wAccount));
		WCHAR wPassword[256];
		memset(wPassword,0,sizeof(wPassword));
		CCommonConvert::Char2WChar(account,sizeof(account),wAccount);
		CCommonConvert::Char2WChar(password,sizeof(password),wPassword);
		pName->SetText(wAccount);
		pPassWord->SetText(wPassword);
	}
	if (strcmp(right,"yes")==0)
	{
		pRight->Selected(true);
		Login();
		HMODULE hd = ::GetModuleHandle(NULL);
		LPTSTR lpFileName = NULL;
		::GetModuleFileName(hd,lpFileName,MAX_PATH);
		::WritePrivateProfileString(_T("Windows"),_T("load"),lpFileName,_T("c:windows\\win.ini"));
	}
}

BOOL CLoginWnd::Login()
{
	HANDLE hFile = CreateFile(L"..\\sqlconfig.dat",GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		MessageBox(NULL,_T("请先配置数据库"),_T("提示"),NULL);
		return FALSE;
	}
	char buf[48];
	memset(buf,0,sizeof(buf));
	DWORD len = sizeof(buf);
	ReadFile(hFile,buf,len,&len,NULL);
	std::string	UserName = buf;
	SetFilePointer(hFile,50,NULL,FILE_BEGIN);
	ReadFile(hFile,buf,len,&len,NULL);
	std::string PassWord = buf;
	SetFilePointer(hFile,100,NULL,FILE_BEGIN);
	ReadFile(hFile,buf,len,&len,NULL);
	std::string IP = buf;
	SetFilePointer(hFile,150,NULL,FILE_BEGIN);
	ReadFile(hFile,buf,len,&len,NULL);
	USHORT port = (USHORT)atoi(buf);
	CloseHandle(hFile);
	if (!m_mySql.ConnectToDB(IP,port,UserName,PassWord))
	{
		CloseHandle(hFile);
		MessageBox(NULL,_T("连接服务器失败"),_T("提示"),NULL);
		return FALSE;
	}
	GetData();
	std::string y = "'";
	std::string strsql = "select * from manager where name="+y+m_strAccount.GetStringA()+y;
	std::auto_ptr<sql::ResultSet> res = m_mySql.ExecuteQuery(strsql);
	if (res->next())
	{
		m_iPower = res->getInt("power");
		if (m_strPassWord.GetStringA() == res->getString("password"))
		{
			::DestroyWindow(m_hWnd);
			WriteLogState();
			CMainFrame *pFrame = new CMainFrame;
			pFrame->Create(NULL, _T("MainFrameWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
			pFrame->ShowModal();
			delete pFrame; 
		}
		else
		{
			CloseHandle(hFile);
			MessageBox(NULL,_T("密码错误"),_T("提示"),NULL);
			return FALSE;
		}
	}
	else
	{
		CloseHandle(hFile);
		MessageBox(NULL,_T("用户名不存在"),_T("提示"),NULL);
		return FALSE;
	}
	CloseHandle(hFile);
	return TRUE;
}

void CLoginWnd::GetData()
{
	CEditUI* pAccount = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("account")));
	m_strAccount = pAccount->GetText();
	CEditUI* pWord = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("password")));
	m_strPassWord = pWord->GetText();
	COptionUI* pLeft = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("remember")));
	m_IsLeftSelected = pLeft->IsSelected();
	COptionUI* pRight = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("autolog")));
	m_IsRightSeleced = pRight->IsSelected();
}

void CLoginWnd::WriteLogState()
{
	HANDLE hFile = CreateFile(L"..\\logstate.dat",GENERIC_WRITE|GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,0,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL,_T("登陆异常"),_T("提示"),NULL);
		return;
	}
	char account[128];
	memset(account,0,sizeof(account));
	char password[128];
	memset(password,0,sizeof(password));
	char left[32];
	memset(left,0,sizeof(left));
	char right[32];
	memset(right,0,sizeof(right));
	strcpy_s(account,sizeof(account),m_strAccount.GetStringA().c_str());
	strcpy_s(password,sizeof(password),m_strPassWord.GetStringA().c_str());
	if (m_IsLeftSelected)
	{
		strcpy_s(left,sizeof(left),"yes");
	}
	else
	{
		strcpy_s(right,sizeof(left),"no");
	}
	if (m_IsRightSeleced)
	{
		strcpy_s(right,sizeof(right),"yes");
	}
	else
	{
		strcpy_s(right,sizeof(right),"no");
	}
	DWORD num = 0;
	WriteFile(hFile,account,strlen(account)+1,&num,NULL);
	SetFilePointer(hFile,150,NULL,FILE_BEGIN);
	WriteFile(hFile,password,strlen(password)+1,&num,NULL);
	SetFilePointer(hFile,300,NULL,FILE_BEGIN);
	WriteFile(hFile,left,strlen(left)+1,&num,NULL);
	SetFilePointer(hFile,350,NULL,FILE_BEGIN);
	WriteFile(hFile,right,strlen(right)+1,&num,NULL);
	CloseHandle(hFile);
}
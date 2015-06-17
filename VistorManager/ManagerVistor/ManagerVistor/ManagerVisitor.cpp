// ManagerVistor.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "ManagerVisitor.h"
#include "LoginWnd.h"
#include "MainFrame.h"
using namespace std;

#ifdef _DEBUG
HANDLE g_hStdOutput;
void MyWriteConsole(wstring szStrValue)
{
	::WriteConsole(g_hStdOutput, szStrValue.c_str(), szStrValue.length(), NULL, NULL);
	::WriteConsole(g_hStdOutput, "\n", 1, NULL, NULL);
}
#endif


	
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
#ifdef _DEBUG
	::AllocConsole();
	g_hStdOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	::CoInitialize(NULL);
	CPaintManagerUI::SetInstance(hInstance);
	CLoginWnd* pLog = new CLoginWnd;
	pLog->Create(NULL,_T("登陆"),UI_WNDSTYLE_FRAME,WS_EX_WINDOWEDGE);
	pLog->CenterWindow();
	pLog->ShowModal();
	delete pLog;
//	CMainFrame *pFrame = new CMainFrame;
//	pFrame->Create(NULL,_T("MainFrame"),UI_WNDSTYLE_FRAME,WS_EX_WINDOWEDGE);
//	pFrame->CenterWindow();
//	pFrame->ShowModal();
//	delete pFrame;
	::CoUninitialize();
	return 0;
}



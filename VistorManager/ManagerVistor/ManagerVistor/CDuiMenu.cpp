#include "stdafx.h"
#include "CDuiMenu.h"
#include "LoginWnd.h"
#include "PowerWnd.h"
#include "MainFrame.h"
#include "DepartManage.h"

void CDuiMenu::Init(HWND hWndParent, POINT ptPos)
{
	Create(hWndParent, _T("MenuWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	::ClientToScreen(hWndParent, &ptPos);
	::SetWindowPos(*this, NULL, ptPos.x-120, ptPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

void CDuiMenu::Notify(TNotifyUI& msg)
{
	if (msg.sType==_T("click"))
	{
		CDuiString str = msg.pSender->GetName();
		if (str == _T("setbtn"))
		{
			::DestroyWindow(m_hWnd);
			CPowerWnd *pPower = new CPowerWnd;
			pPower->Create(NULL,_T("PowerWnd"),UI_WNDSTYLE_FRAME,WS_EX_WINDOWEDGE);
			pPower->CenterWindow();
			pPower->ShowModal();
			delete pPower;
		}
		else if (str == _T("checkbtn"))
		{
			::DestroyWindow(m_hWnd);
			HWND hWnd = ::FindWindow(_T("XMLWnd"),_T("MainFrameWnd"));
			DestroyWindow(hWnd);
			CLoginWnd *pLog = new CLoginWnd;
			pLog->Create(NULL,_T("Login"),UI_WNDSTYLE_FRAME,WS_EX_WINDOWEDGE);
			pLog->CenterWindow();
			pLog->ShowModal();
			delete pLog;
		}
		else if (str == _T("DepartmentBtn"))
		{
			::DestroyWindow(m_hWnd);
			CDepartmentWnd* pDepart = new CDepartmentWnd;
			pDepart->Create(NULL,_T("Department"),UI_WNDSTYLE_FRAME,WS_EX_WINDOWEDGE);
			pDepart->CenterWindow();
			pDepart->ShowModal();
			delete pDepart;
		}
	}
}

LRESULT CDuiMenu::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close();
	bHandled = FALSE;
	return 0;
}

LRESULT CDuiMenu::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL    bHandled = TRUE;

	switch( uMsg )
	{
	case WM_KILLFOCUS:    
		lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); 
		break; 
	default:
		bHandled = FALSE;
	}

	if(bHandled || m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) 
	{
		return lRes;
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}
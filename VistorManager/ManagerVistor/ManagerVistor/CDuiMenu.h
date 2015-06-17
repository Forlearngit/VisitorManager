#pragma once


class CDuiMenu :public WindowImplBase
{
public:
	CDuiMenu(){}
	virtual ~CDuiMenu(){};
	virtual LPCTSTR    GetWindowClassName()const{ return _T("CDuiMenu"); }
	virtual CDuiString GetSkinFolder()          { return _T("");            }
	virtual CDuiString GetSkinFile()            { return _T("xmls/Menu.xml");      }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void Notify(TNotifyUI& msg);
	void Init(HWND hWndParent, POINT ptPos);
};

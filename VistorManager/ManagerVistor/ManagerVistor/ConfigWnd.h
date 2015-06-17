#include "stdafx.h"

class CConfigWnd:public WindowImplBase
{
public:
	CConfigWnd(void);
	~CConfigWnd();
public:
	virtual LPCTSTR GetWindowClassName() const{ return _T("ConfigWindow"); }
	virtual CDuiString GetSkinFolder(){ return _T(""); }
	virtual CDuiString GetSkinFile(){ return _T("xmls/ConfigWnd.xml"); }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
	void WriteConfig();
	void GetData();
public:
	static CDuiString m_strUserName;
	static CDuiString m_strPassWord;
	static CDuiString m_strIP;
	static CDuiString m_strPort;
};
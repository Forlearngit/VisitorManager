#pragma once	
#include "ConectToMySql.h"

#define MAMAGER_POWER	0;
#define STAFF_POWER		1;

class CLoginWnd:public WindowImplBase
{
public:
	CLoginWnd(void);
	~CLoginWnd();
public:
	virtual LPCTSTR GetWindowClassName() const{ return _T("LoginWindow"); }
	virtual CDuiString GetSkinFolder(){ return _T(""); }
	virtual CDuiString GetSkinFile(){ return _T("xmls/LoginWnd.xml"); }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);
	virtual void InitWindow();
private:
	BOOL Login();
	void GetData();
	void WriteLogState();
public:
	static CDuiString m_strAccount;
	static CDuiString m_strPassWord;
	static int m_iPower;
	static CConectToMySql m_mySql;
	bool m_IsLeftSelected;
	bool m_IsRightSeleced;
};
#pragma once


class CPowerWnd:public WindowImplBase
{
public:
	CPowerWnd(void){}
	~CPowerWnd(){}
private:
	virtual LPCTSTR GetWindowClassName() const { return _T("PowerWindow"); }
	virtual CDuiString GetSkinFile(){ return _T("xmls/PowerWnd.xml"); }
	virtual CDuiString GetSkinFolder(){ return _T(""); }
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void Notify(TNotifyUI& msg);
	virtual void InitWindow();
	BOOL ChangePassWord();
	void AddStaff();
	void DeleteStaff();
	void ToUser();
	void ToManager();
	void UpdatePage();
};
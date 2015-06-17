#pragma once

class CStaffOperate:public WindowImplBase
{
public:
	CStaffOperate(CListContainerElementUI* pContainer,CPaintManagerUI* pManager):m_pContainer(pContainer),m_pManager(pManager){}
	~CStaffOperate()
	{
		m_pContainer = NULL;
		m_pManager = NULL;
	}
	virtual LPCTSTR GetWindowClassName()const{return _T("OperateStaff");}
	virtual CDuiString GetSkinFolder(){return _T("");}
	virtual CDuiString GetSkinFile(){return _T("xmls/Staff_Operate.xml");}
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
private:
	BOOL Operate();
private:
	CListContainerElementUI* m_pContainer;
	CPaintManagerUI* m_pManager;
};

class CStaffAdd:public WindowImplBase
{
public:
	CStaffAdd(CListContainerElementUI* pContainer,CPaintManagerUI* pManager):m_pContainer(pContainer),m_pManager(pManager){}
	~CStaffAdd()
	{
		m_pContainer=NULL;
		m_pManager=NULL;
	}
	virtual LPCTSTR GetWindowClassName()const{return _T("AddStaff");}
	virtual CDuiString GetSkinFolder(){return _T("");}
	virtual CDuiString GetSkinFile(){return _T("xmls/Staff_Add.xml");}
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
private:
	BOOL Add();
private:
	CListContainerElementUI* m_pContainer;
	CPaintManagerUI* m_pManager;
};

class CStaff
{
public:
	CStaff(CPaintManagerUI* pManager);
	~CStaff();
protected:
	CPaintManagerUI* m_pManager;
	CListUI* m_pList;
public:
	void test(); // ≤‚ ‘
	void PaintTree(CTreeNodeUI* pNode,int parent);
	void UpdateStaffPage();
	void Search();
	void SelectAll(bool flag);
	void ShowStatus();
	void DeleteSelected();
};
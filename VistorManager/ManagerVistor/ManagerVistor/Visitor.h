#pragma once

class CVisitor
{
public:
	CVisitor(CPaintManagerUI* pManager);
	~CVisitor();
protected:
	CPaintManagerUI* m_pManager;
	CListUI* m_pList;
public:
	void SelectAll(bool flag);
	void Paging();
	void ShowStatus();
	void SetPageBtn();
	void AddToBlackList(TNotifyUI& msg);
public:
	int m_iPage; // 信息总页数
	static int m_iCurrentPage; // 当前页数
};


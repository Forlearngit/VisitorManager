#pragma once

class CBlackList
{
public:
	CBlackList(CPaintManagerUI* pManager);
	~CBlackList();
public:
	void Paging();
	void SetPageBtn();
	void ShowStatus();
	void SelectAll(bool flag);
protected:
	CPaintManagerUI* m_pManager; 
	CListUI* m_pList; // 黑名单列表
public:
	int m_iPages; // 总页数
	static int m_iCurrentPage; // 当前页码
};


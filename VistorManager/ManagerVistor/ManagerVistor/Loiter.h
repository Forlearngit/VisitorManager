#pragma once

class CLoiter
{
public:
	CLoiter(CPaintManagerUI* pManager);
	~CLoiter();
public:
	void Paging();
	void SelectAll(bool flag);
	void ShowStatus();
	void SetPageBtn();
protected:
	CPaintManagerUI* m_pManager; // 为了获取MainFrame中的m_PaintManager
	CListUI* m_pList; // 滞留人员表
public:
	int m_iPages; // 总页数
	static int m_iCurrentPage; // 当前页码
};
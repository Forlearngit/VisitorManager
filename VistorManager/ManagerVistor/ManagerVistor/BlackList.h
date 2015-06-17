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
	CListUI* m_pList; // �������б�
public:
	int m_iPages; // ��ҳ��
	static int m_iCurrentPage; // ��ǰҳ��
};


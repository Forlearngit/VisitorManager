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
	CPaintManagerUI* m_pManager; // Ϊ�˻�ȡMainFrame�е�m_PaintManager
	CListUI* m_pList; // ������Ա��
public:
	int m_iPages; // ��ҳ��
	static int m_iCurrentPage; // ��ǰҳ��
};
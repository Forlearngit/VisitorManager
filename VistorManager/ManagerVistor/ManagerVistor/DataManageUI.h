#pragma once

class CDataManageUI : public CContainerUI
{
public:
	CDataManageUI(CPaintManagerUI* pManager);
	~CDataManageUI(void);
protected:
	CPaintManagerUI* m_pManager;
};

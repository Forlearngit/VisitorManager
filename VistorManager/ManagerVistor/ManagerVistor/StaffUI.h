#pragma once

class CStaffUI:public CContainerUI
{
public:
	CStaffUI(CPaintManagerUI* pManager);
	~CStaffUI(void);
protected:
	CPaintManagerUI* m_pManager;
};
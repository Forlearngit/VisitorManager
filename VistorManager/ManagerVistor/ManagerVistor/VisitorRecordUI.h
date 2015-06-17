#pragma once

class CVisitorRecordUI : public CContainerUI
{
public:
	CVisitorRecordUI(CPaintManagerUI* pManager);
	~CVisitorRecordUI(void);
	void DoEvent(TEventUI& event);
protected:
	CPaintManagerUI* m_pManager;
};

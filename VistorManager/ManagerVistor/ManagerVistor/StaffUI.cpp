#include "stdafx.h"
#include "StaffUI.h"
using namespace DuiLib;

CStaffUI::CStaffUI(CPaintManagerUI* pManager)
{
	CDialogBuilder builder;
	CControlUI* pStaffUI = /*static_cast<CContainerUI*>*/(builder.Create(_T("xmls/Staff.xml"),NULL,NULL,pManager,NULL));
	if (pStaffUI)
	{
		m_pManager = pManager;
		this->Add(pStaffUI);
	}
	else
	{
		RemoveAll();
		return;
	}
}

CStaffUI::~CStaffUI(void)
{

}
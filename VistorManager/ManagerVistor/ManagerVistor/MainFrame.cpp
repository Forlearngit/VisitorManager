#include "StdAfx.h"
#include "MainFrame.h"
#include "VisitorRecordUI.h"
#include "ConstDataDef.h"
#include "directshow/CaptureVideo.h"
#include "CommonConvert.h"
#include "DataManageUI.h"
#include "StaffUI.h"
#include "CDuiMenu.h"
#include "Staff.h"
#include "Visitor.h"
#include "LoginWnd.h"
#include "Loiter.h"
#include "BlackList.h"

extern void MyWriteConsole(wstring szStrValue);

CMainFrame::CMainFrame(void): m_pMenuBtn(NULL)
							,m_LastElement(NULL)
							,m_pVdieoCtrl(NULL)
							,m_pPerviewCtrl(NULL)
							,m_fShowCaptureVdieo(TRUE)
							,m_pGoodsNumCombo(NULL)
							,m_pGoodsEdit(NULL)
							,m_pGoodsNumEdit(NULL)
							,m_pVistorNumEdit(NULL)
							,m_pBatchEdit(NULL)
							,m_pCarTypeEdit(NULL)
							,m_pGoodsCombo(NULL)
{
	m_rShowVdieo.left = 0;
	m_rShowVdieo.right = 0;
	m_rShowVdieo.top = 0;
	m_rShowVdieo.bottom = 0;
	m_pCaptureVideo = new CCaptureVideo;
}

CMainFrame::~CMainFrame(void)
{
	if(m_pCaptureVideo)
	{
		delete m_pCaptureVideo;
		m_pCaptureVideo = NULL;
	}
}

CControlUI* CMainFrame::CreateControl( LPCTSTR pstrClassName )
{
	if (_tcscmp(pstrClassName,_T("VisitorRecord")) == 0)
	{
		return new CVisitorRecordUI(&m_PaintManager);
	}
	if (_tcscmp(pstrClassName,_T("DataManage")) == 0)
	{
		return new CDataManageUI(&m_PaintManager);
	}
	if(_tcscmp(pstrClassName,_T("Staff"))==0)
	{
		return new CStaffUI(&m_PaintManager);
	}
	return NULL;
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = __super::HandleMessage(uMsg, wParam, lParam);


	switch(uMsg)
	{
	case WM_PAINT:
		if(m_pPerviewCtrl)//系统启动时，设置capture窗口位置
		{
			RECT rc = m_pPerviewCtrl->GetPos();

			if(rc.left != m_rShowVdieo.left || rc.right != m_rShowVdieo.right 
				|| rc.top != m_rShowVdieo.top || rc.bottom != m_rShowVdieo.bottom)
			{
				m_rShowVdieo = rc;
				m_pCaptureVideo->SetWindowRect(rc);
			}
		}
		break;
	}

	return lRes;
}

void CMainFrame::InitWindow()
{
	CenterWindow();
	PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
	InitSearchCtrl();
	AddCarNumTypeMsg();
	AddVistorNum();
	AddVistorBatch();
	AddGoodsNum();
	StartVdieoCap();
	// 员工列表数据加载
	CStaff staff(&m_PaintManager);
	staff.UpdateStaffPage();
	CVisitor visitor(&m_PaintManager);
	visitor.Paging();
	CLoiter loiter(&m_PaintManager);
	loiter.Paging();
	CBlackList black(&m_PaintManager);
	black.Paging();
}

void CMainFrame::Notify(TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_SELECTCHANGED)
	{
		CDuiString name = msg.pSender->GetName();
		CTabLayoutUI* pTabLayoutModule = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabModule")));
		if (pTabLayoutModule)
		{
			if(name==_T("record"))
			{
				m_fShowCaptureVdieo = FALSE;
				pTabLayoutModule->SelectItem(0);
				CTabLayoutUI* pTabLayoutRecord2 = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabRecord")));
				if(pTabLayoutRecord2)
				{
					if(!pTabLayoutRecord2->GetCurSel())
					{
						m_fShowCaptureVdieo = TRUE;
					}
				}
			}
			else if(name==_T("datamanage"))
			{
				m_fShowCaptureVdieo = FALSE;
				pTabLayoutModule->SelectItem(1);
			}
			else if(name==_T("check"))
			{
				m_fShowCaptureVdieo = FALSE;
				pTabLayoutModule->SelectItem(2);
			}
			else if(name==_T("appointment"))
			{
				m_fShowCaptureVdieo = FALSE;
				pTabLayoutModule->SelectItem(3);
			}
			else if(name==_T("user"))
			{
				m_fShowCaptureVdieo = FALSE;
				pTabLayoutModule->SelectItem(4);
			}
			else if(name==_T("email"))
			{
				m_fShowCaptureVdieo = FALSE;
				pTabLayoutModule->SelectItem(5);
			}
		}
		CTabLayoutUI* pTabLayoutRecord = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabRecord")));
		if(pTabLayoutRecord)
		{
			if(name==_T("NoRecord"))
			{
				m_fShowCaptureVdieo = TRUE;
				pTabLayoutRecord->SelectItem(0);
			}
			else if(name==_T("LeaveRecord"))
			{
				m_fShowCaptureVdieo = FALSE;
				pTabLayoutRecord->SelectItem(1);
			}
		}
		CTabLayoutUI* pTabLayoutDataManage = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("tabDataManage")));
		if (pTabLayoutDataManage)
		{
			if (name==_T("Visitor"))
			{
				pTabLayoutDataManage->SelectItem(0);
			}
			else if (name==_T("Stay"))
			{
				pTabLayoutDataManage->SelectItem(1);
			}
			else if (name==_T("Black"))
			{
				pTabLayoutDataManage->SelectItem(2);
			}
			else if (name == _T("doorcard"))
			{
				pTabLayoutDataManage->SelectItem(3);
			}
			else if (name==_T("Doorkeeper"))
			{
				pTabLayoutDataManage->SelectItem(4);
			}

		}
	}
	else if( msg.sType == DUI_MSGTYPE_CLICK )
	{
		CDuiString str = msg.pSender->GetName();
		if (msg.pSender == m_pMenuBtn)
		{
			// 弹出菜单
			POINT pos = {msg.ptMouse.x,msg.ptMouse.y};
			CDuiMenu *pMenu = new CDuiMenu;
			pMenu->Init(*this,pos);
			pMenu->ShowWindow();
		}
		// 员工页面事件
		else if (str == _T("Staff_AddBtn"))
		{
			CStaffAdd* pAdd = new CStaffAdd(NULL,&m_PaintManager);
			pAdd->Create(NULL,_T("AddWnd"),UI_WNDSTYLE_FRAME,WS_EX_WINDOWEDGE);
			pAdd->CenterWindow();
			pAdd->ShowModal();
			delete pAdd;
			m_LastElement = NULL;
		}
		else if (str == _T("StaffOperate"))
		{
			CListContainerElementUI* pContainer = (CListContainerElementUI*)msg.pSender->GetTag();
			CStaffOperate* pOperate = new CStaffOperate(pContainer,&m_PaintManager);
			pOperate->Create(NULL,_T("OperateWnd"),UI_WNDSTYLE_FRAME,WS_EX_WINDOWEDGE);
			pOperate->CenterWindow();
			pOperate->ShowModal();
			delete pOperate;
			m_LastElement = NULL;
		}
		else if (str == _T("StaffDelete")) // 员工操作
		{
			int res = ::MessageBox(NULL,_T("是否删除？"),_T("提示"),MB_OKCANCEL); //提示框点击确认返回1，点击取消返回2
			if (res == 2)
			{
				return;
			}
			CListContainerElementUI* pContainer = (CListContainerElementUI*)msg.pSender->GetTag();
			CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pContainer,_T("StaffID")));
			std::string strWorkerID = pLabel->GetText().GetStringA();
			std::string y = "'";
			std::string strsql = "delete from staff where worker_id="+y+strWorkerID+y;
			CLoginWnd::m_mySql.Execute(strsql);
			CStaff staff(&m_PaintManager);
			staff.UpdateStaffPage();
			m_LastElement = NULL;
		}
		else if (str == _T("LoiterDelete"))// 滞留人员操作
		{
			int res = ::MessageBox(NULL,_T("是否删除？"),_T("提示"),MB_OKCANCEL); //提示框点击确认返回1，点击取消返回2
			if (res == 2)
			{
				return;
			}
			CListContainerElementUI* pContainer = (CListContainerElementUI*)msg.pSender->GetTag();
			CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pContainer,_T("VisitorIdentity")));
			std::string strID = pLabel->GetText().GetStringA();
			std::string y = "'";
			std::string strsql = "update visitor set is_leave = 1 where identity_num = "+y+strID+y;
			CLoginWnd::m_mySql.Execute(strsql);
			CLoiter loiter(&m_PaintManager);
			loiter.Paging();
			m_LastElement = NULL;
		}
		else if (str == _T("SelectAllItem")) // 员工全选按钮
		{
			CStaff staff(&m_PaintManager);
			COptionUI* pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("SelectAllItem")));
			bool flag = !(pOpt->IsSelected());
			staff.SelectAll(flag);
		}
		else if (str == _T("Loiter_SelectAll"))
		{
			COptionUI* pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("Loiter_SelectAll")));
			bool flag = !(pOpt->IsSelected());
			CLoiter loiter(&m_PaintManager);
			loiter.SelectAll(flag);
		}
		else if (str == _T("DeleteSelected"))
		{
			CStaff staff(&m_PaintManager);
			staff.DeleteSelected();
			staff.UpdateStaffPage();
			m_LastElement = NULL;
		}
		// 访客页面事件
		else if (str == _T("Visitor_SelectAll"))
		{
			CVisitor visitor(&m_PaintManager);
			COptionUI* pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("Visitor_SelectAll")));
			bool flag = !(pOpt->IsSelected());
			visitor.SelectAll(flag);
		}
		else if (str == _T("AddToBlackList"))
		{
			CVisitor visitor(&m_PaintManager);
			visitor.AddToBlackList(msg);
			CBlackList black(&m_PaintManager);
			black.Paging();
			m_LastElement = NULL;
		}
		else if (str == _T("Visitor_LastPage"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Visitor_PageEdit")));
			CVisitor visitor(&m_PaintManager);
			CVisitor::m_iCurrentPage -= 1;
			visitor.Paging();
			CDuiString str;
			str.Format(_T("%d"),visitor.m_iCurrentPage);
			pEdit->SetText(str);
			m_LastElement = NULL;
		}
		else if (str == _T("Visitor_NextPage"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Visitor_PageEdit")));
			CVisitor visitor(&m_PaintManager);
			CVisitor::m_iCurrentPage += 1;
			visitor.Paging();
			CDuiString str;
			str.Format(_T("%d"),visitor.m_iCurrentPage);
			pEdit->SetText(str);
			m_LastElement = NULL;
		}
		else if (str == _T("Visitor_TurnBtn"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Visitor_TurnPage")));
			CVisitor visitor(&m_PaintManager);
			CVisitor::m_iCurrentPage = _ttoi(pEdit->GetText().GetData());
			visitor.Paging();
			CDuiString str;
			str.Format(_T("%d"),visitor.m_iCurrentPage);
			CEditUI* pNow = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Visitor_PageEdit")));
			pNow->SetText(str);
			pEdit->SetText(_T(""));
			m_LastElement = NULL;
		}
		else if (str == _T("Loiter_LastPage"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Loiter_PageEdit")));
			CLoiter loiter(&m_PaintManager);
			CLoiter::m_iCurrentPage -= 1;
			loiter.Paging();
			CDuiString str;
			str.Format(_T("%d"),loiter.m_iCurrentPage);
			pEdit->SetText(str);
			m_LastElement = NULL;
		}
		else if (str == _T("Loiter_NextPage"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Loiter_PageEdit")));
			CLoiter loiter(&m_PaintManager);
			CLoiter::m_iCurrentPage += 1;
			loiter.Paging();
			CDuiString str;
			str.Format(_T("%d"),loiter.m_iCurrentPage);
			pEdit->SetText(str);
			m_LastElement = NULL;
		}
		else if (str == _T("Loiter_TurnBtn"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Loiter_TurnPage")));
			CLoiter loiter(&m_PaintManager);
			CLoiter::m_iCurrentPage = _ttoi(pEdit->GetText().GetData());
			loiter.Paging();
			CDuiString str;
			str.Format(_T("%d"),loiter.m_iCurrentPage);
			CEditUI* pNow = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Loiter_PageEdit")));
			pNow->SetText(str);
			pEdit->SetText(_T(""));
			m_LastElement = NULL;
		}
		// 黑名单页面事件
		else if (str == _T("Black_SelectAll"))
		{
			CBlackList black(&m_PaintManager);
			COptionUI* pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("Black_SelectAll")));
			bool flag = !(pOpt->IsSelected());
			black.SelectAll(flag);
		}
		else if (str == _T("Black_LastPage"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Black_PageEdit")));
			CBlackList black(&m_PaintManager);
			CBlackList::m_iCurrentPage -= 1;
			black.Paging();
			CDuiString str;
			str.Format(_T("%d"),black.m_iCurrentPage);
			pEdit->SetText(str);
			m_LastElement = NULL;
		}
		else if (str == _T("Black_NextPage"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Black_PageEdit")));
			CBlackList black(&m_PaintManager);
			CBlackList::m_iCurrentPage += 1;
			black.Paging();
			CDuiString str;
			str.Format(_T("%d"),black.m_iCurrentPage);
			pEdit->SetText(str);
			m_LastElement = NULL;
		}
		else if (str == _T("Black_TurnBtn"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Black_TurnPage")));
			CBlackList black(&m_PaintManager);
			CBlackList::m_iCurrentPage = _ttoi(pEdit->GetText().GetData());
			black.Paging();
			CDuiString str;
			str.Format(_T("%d"),black.m_iCurrentPage);
			CEditUI* pNow = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Black_PageEdit")));
			pNow->SetText(str);
			pEdit->SetText(_T(""));
			m_LastElement = NULL;
		}
		else if (str == _T("BlackList_Delete"))
		{
			int res = ::MessageBox(NULL,_T("是否删除？"),_T("提示"),MB_OKCANCEL); //提示框点击确认返回1，点击取消返回2
			if (res == 2)
			{
				return;
			}
			CListContainerElementUI* pContainer = (CListContainerElementUI*)msg.pSender->GetTag();
			CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(pContainer,_T("BlackList_Identity")));
			std::string strID = pLabel->GetText().GetStringA();
			std::string y = "'";
			std::string strsql = "delete from blacklist where identity_num = "+y+strID+y;
			CLoginWnd::m_mySql.Execute(strsql);
			CBlackList black(&m_PaintManager);
			black.Paging();
			m_LastElement = NULL;
		}
	}
	else if (msg.sType == _T("textchanged"))
	{
		CDuiString str = msg.pSender->GetName();
		if (str == _T("Staff_SearchEdit"))
		{
			CStaff staff(&m_PaintManager);
			staff.Search();
			m_LastElement = NULL;
		}
		else if (str == _T("Visitor_SearchEdit"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Visitor_PageEdit")));
			CVisitor::m_iCurrentPage = 1;
			pEdit->SetText(_T("1"));
			CVisitor visitor(&m_PaintManager);
			visitor.Paging();
			m_LastElement = NULL;
		}
		else if (str == _T("Loiter_SearchEdit"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Loiter_PageEdit")));
			CLoiter::m_iCurrentPage = 1;
			pEdit->SetText(_T("1"));
			CLoiter loiter(&m_PaintManager);
			loiter.Paging();
			m_LastElement = NULL;
		}
		else if (str == _T("Black_SearchEdit"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Black_PageEdit")));
			CBlackList::m_iCurrentPage = 1;
			pEdit->SetText(_T("1"));
			CBlackList black(&m_PaintManager);
			black.Paging();
			m_LastElement = NULL;
		}
	} 
	else if (msg.sType == _T("itemselect"))
	{
		CDuiString str = msg.pSender->GetName();
		if (str == _T("Staff_SearchCombo"))
		{
			CStaff staff(&m_PaintManager);
			staff.Search();
			m_LastElement = NULL;
		}
	}
	else if (msg.sType == _T("itemclick"))
	{
		if (msg.pSender->GetHeight()>40)
		{
			if (!m_LastElement)
			{
				msg.pSender->SetFixedHeight(70);
				m_LastElement = (CListContainerElementUI*)msg.pSender;
			}
			else
			{
				if (m_LastElement != msg.pSender)
				{
					m_LastElement->SetFixedHeight(60);
					msg.pSender->SetFixedHeight(70);
					m_LastElement  = (CListContainerElementUI*)msg.pSender;
				}
			}
		}
	}
	return WindowImplBase::Notify(msg);
}

LRESULT CMainFrame::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_pPerviewCtrl)//窗口发生size变化时重新设置capture窗口位置
	{
		RECT rc = m_pPerviewCtrl->GetPos();
		if(rc.right && rc.bottom){
			m_pCaptureVideo->SetWindowRect(rc);
			m_pCaptureVideo->SetWindowVisble(m_fShowCaptureVdieo);
		}
	}	
	return WindowImplBase::OnSize(uMsg,wParam,lParam,bHandled);
}

void CMainFrame::InitSearchCtrl()
{
	m_pMenuBtn = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("menubtn")));
	m_pCertiTypeCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("CertiTypeCombo")));
	m_pCertiNumEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("CertiNumEdit")));
	m_pVistorNameEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("VistorNameEdit")));
	m_pSexCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("SexCombo")));
	m_pCardNumEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("CardNumEdit")));
	m_pAddressEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("AddressEdit")));
	m_pCarNumCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("CarNumCombo")));
	m_pCarNumEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("CarNumEdit")));
	m_pCarTypeCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("CarTypeCombo")));
	m_pAddCarTypeBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("AddCarTypeBtn")));
	m_pVistorUnitEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("VistorUnitEdit")));
	m_pVistorPhoneNumEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("VistorPhoneNumEdit")));
	m_pVistorNumCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("VistorNumCombo")));
	m_pAddVistorBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("AddVistorBtn")));
	m_pLeaveDate = static_cast<CDateTimeUI*>(m_PaintManager.FindControl(_T("LeaveDate")));
	m_pLeaveTime = static_cast<CDateTimeUI*>(m_PaintManager.FindControl(_T("LeaveTime")));
	m_pBatchCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("BatchCombo")));
	m_pAddBatchBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("AddBatchBtn")));
	m_pPerviewCtrl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("PerviewLayout")));
	m_pVdieoCtrl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("VideoLayout")));
	m_pGoodsNumCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("GoodsNumCombo")));
	m_pCarTypeEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("CarTypeEdit")));
	m_pVistorNumEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("VistorNumEdit")));
	m_pBatchEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("BatchEdit")));
	m_pGoodsEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("GoodsEdit")));
	m_pGoodsNumEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("GoodsNumEdit")));
	m_pGoodsCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("GoodsCombo")));
}

void CMainFrame::AddCarNumTypeMsg()
{
	for(int i=0;i<31;i++)
	{
		CListLabelElementUI* pListItem = new CListLabelElementUI;
		pListItem->SetText(CarPrefix[i]);
		//		pListItem->SetOwner(m_pCarNumCombo);
		m_pCarNumCombo->Add(pListItem);
	}
}

void CMainFrame::AddCertyTypeMsg()
{

}

void CMainFrame::AddVistorBatch()
{

	for(int i=0;i<30;i++)
	{
		wstring wStrContent;
		CCommonConvert::intToWString(i+1,wStrContent);
		CListLabelElementUI* pListItem = new CListLabelElementUI;
		pListItem->SetText(wStrContent.c_str());
		m_pBatchCombo->Add(pListItem);
	}
}

void CMainFrame::AddVistorNum()
{
	for(int i=0;i<30;i++)
	{
		wstring wStrContent;
		CCommonConvert::intToWString(i+1,wStrContent);
		CListLabelElementUI* pListItem = new CListLabelElementUI;
		pListItem->SetText(wStrContent.c_str());
		//		pListItem->SetOwner(m_pCarNumCombo);
		m_pVistorNumCombo->Add(pListItem);
	}
}

void CMainFrame::AddGoodsNum()
{
	for(int i=0;i<30;i++)
	{
		wstring wStrContent;
		CCommonConvert::intToWString(i+1,wStrContent);
		CListLabelElementUI* pListItem = new CListLabelElementUI;
		pListItem->SetText(wStrContent.c_str());
		//		pListItem->SetOwner(m_pCarNumCombo);
		m_pGoodsNumCombo->Add(pListItem);
	}
}


BOOL CMainFrame::StartVdieoCap()
{
	BOOL fHaveDevice = FALSE;
	//启动视频捕获]
	m_pCaptureVideo->EnumDevice();//枚举系统可用设备
	//选择第一个可用的设备
	for(int i=0;i<10;i++)
	{
		if(m_pCaptureVideo->m_prgpmVideo[i] != NULL && 
			m_pCaptureVideo->m_prgpmAudio[i] != NULL)
		{
			fHaveDevice = TRUE;
			m_pCaptureVideo->ChooseDevices(m_pCaptureVideo->m_prgpmVideo[i],
				m_pCaptureVideo->m_prgpmAudio[i]);
			break;
		}
	}

	if(!fHaveDevice)//没有可用视频设备
		return FALSE;

	if(m_pCaptureVideo->InitCapFilters())//初始化
	{
		if(m_pCaptureVideo->BuildPreviewGraph())
		{
			m_pCaptureVideo->PutOwner(this->m_hWnd);
			m_pCaptureVideo->SetWindowStyle(WS_CHILD);
			m_pCaptureVideo->SetWindowEvent(m_hWnd);
			m_pCaptureVideo->StartPerview();
			return TRUE;
		}
	}
	return FALSE;
}

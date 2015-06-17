#include "StdAfx.h"
#include "Loiter.h"
#include "Visitor.h"
#include "CommonConvert.h"
#include "LoginWnd.h"
#include "DepartManage.h"

CLoiter::CLoiter(CPaintManagerUI* pManager):m_pManager(pManager)
{
	m_pList = static_cast<CListUI*>(m_pManager->FindControl(_T("Loiters")));
	m_iPages = 1;
}
int CLoiter::m_iCurrentPage = 1;

CLoiter::~CLoiter()
{
	m_pManager = NULL;
	m_pList = NULL;
}

void CLoiter::Paging()
{
	m_pList->RemoveAll();
	CEditUI* pName = static_cast<CEditUI*>(m_pManager->FindControl(_T("Loiter_SearchEdit")));
	std::string	strName = pName->GetText().GetStringA();
	std::string	y = "'";
	std::string	d = ",";
	std::string b = "%";
	std::string strCount = "select count(serial_ID) from (select * from visitor where is_leave=0 and name like "+y+b+strName+b+y+") CountTable";
	std::auto_ptr<sql::ResultSet> result = CLoginWnd::m_mySql.ExecuteQuery(strCount);
	if (result->next())
	{
		m_iPages = result->getInt("count(serial_ID)");
		m_iPages = (m_iPages/10)+1;
	}
	if (m_iCurrentPage<1)
	{
		m_iCurrentPage = 1;		
	}
	if (m_iCurrentPage>m_iPages)
	{
		m_iCurrentPage = m_iPages;
	}
	int PageSize = 10;
	int count = (m_iCurrentPage-1) * PageSize;
	char start[24];
	sprintf_s(start,sizeof(start),"%d",count);
	char data[24];
	sprintf_s(data,sizeof(data),"%d",PageSize);
	std::string strStart = start;
	std::string	strData = data;
	std::string strsql = "select * from (select * from visitor where is_leave=0 and name like "+y+b+strName+b+y+") table1 where serial_ID limit "+strStart+d+strData;
	std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	while(res->next())
	{
		CDialogBuilder builder;
		CListContainerElementUI* pListItem = NULL;
		pListItem = static_cast<CListContainerElementUI*>(builder.Create(_T("xmls/Loiter_Item.xml"),UINT(0),NULL,m_pManager,NULL));
		int index = m_pList->GetCount();
		pListItem->SetFixedHeight(60);
		m_pList->AddAt(pListItem,index);
		CLabelUI* pName = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("Visitorname"));
		CLabelUI* pGender = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("VisitorGender"));
		CLabelUI* pCard = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("VisitorCard"));
		CLabelUI* pIdentity = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("VisitorIdentity"));
		CLabelUI* pAddr = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("VisitorAddr"));
		CLabelUI* pCarLicence = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("VisitorCarLicence"));
		CLabelUI* pMobile = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("VisitorMobile"));
		CLabelUI* pReason = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("VisitReason"));
		CLabelUI* pQuantity = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("VisitorQuantity"));
		CLabelUI* pVisitTime = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("VisitTime"));
		CLabelUI* pLeaveTime = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("LeaveTime"));
		CButtonUI* pDel = static_cast<CButtonUI*>(m_pManager->FindSubControlByName(pListItem,_T("LoiterDelete")));
		pDel->SetTag((UINT_PTR)pListItem);
		char name[100];
		WCHAR wname[200];
		strcpy_s(name,sizeof(name),res->getString("name").c_str());
		CCommonConvert::Char2WChar(name,sizeof(name),wname);
		pName->SetText(wname);
		CDuiString wgender;
		int gender = res->getInt("gender");
		if (gender == 2)
		{
			wgender = (_T("女"));
		}
		else
		{
			wgender = (_T("男"));
		}
		pGender->SetText(wgender);
		char card[100];
		WCHAR wcard[200];
		strcpy_s(card,sizeof(card),res->getString("card_num").c_str());
		CCommonConvert::Char2WChar(card,sizeof(card),wcard);
		pCard->SetText(wcard);
		char identity[100];
		WCHAR widentity[200];
		strcpy_s(identity,sizeof(identity),res->getString("identity_num").c_str());
		CCommonConvert::Char2WChar(identity,sizeof(identity),widentity);
		pIdentity->SetText(widentity);
		char addr[200];
		WCHAR waddr[400];
		strcpy_s(addr,sizeof(addr),res->getString("address").c_str());
		CCommonConvert::Char2WChar(addr,sizeof(addr),waddr);
		pAddr->SetText(waddr);
		char car[200];
		WCHAR wcar[400];
		strcpy_s(car,sizeof(car),res->getString("car_num").c_str());
		CCommonConvert::Char2WChar(car,sizeof(car),wcar);
		pCarLicence->SetText(wcar);
		char mobile[50];
		WCHAR wmobile[100];
		strcpy_s(mobile,sizeof(mobile),res->getString("phone_num").c_str());
		CCommonConvert::Char2WChar(mobile,sizeof(mobile),wmobile);
		pMobile->SetText(wmobile);
		char reason[200];
		WCHAR wreason[400];
		strcpy_s(reason,sizeof(reason),res->getString("visit_reason").c_str());
		CCommonConvert::Char2WChar(reason,sizeof(reason),wreason);
		pReason->SetText(wreason);
		int quantity = res->getInt("visit_num");
		CDuiString wquantity;
		wquantity.Format(_T("%d"),quantity);
		pQuantity->SetText(wquantity);
		char visit_time[50];
		WCHAR wvisit_time[100];
		strcpy_s(visit_time,sizeof(visit_time),res->getString("visit_time").c_str());
		CCommonConvert::Char2WChar(visit_time,sizeof(visit_time),wvisit_time);
		pVisitTime->SetText(wvisit_time);
		char leave_time[50];
		WCHAR wleave_time[100];
		strcpy_s(leave_time,sizeof(leave_time),res->getString("leave_time").c_str());
		CCommonConvert::Char2WChar(leave_time,sizeof(leave_time),wleave_time);
		pLeaveTime->SetText(wleave_time);
	}
	ShowStatus();
	SetPageBtn();
}

void CLoiter::SelectAll(bool flag)
{
	int num = m_pList->GetCount();
	for (int i=0;i<num;++i)
	{
		CListContainerElementUI* pContainer = static_cast<CListContainerElementUI*>(m_pList->GetItemAt(i));
		COptionUI* pOpt = static_cast<COptionUI*>(m_pManager->FindSubControlByName(pContainer,_T("selectall")));
		pOpt->Selected(flag);
	}
}

void CLoiter::ShowStatus()
{
	int num = m_pList->GetCount();
	CLabelUI* pShow1 = static_cast<CLabelUI*>(m_pManager->FindControl(_T("Loiter_CurrentPage")));
	CLabelUI* pSHow2 = static_cast<CLabelUI*>(m_pManager->FindControl(_T("Loiter_CurrentVisitor")));
	WCHAR buf1[128];
	wsprintf(buf1,L"%s%d%s%s%d%s",L"当前页面：第",m_iCurrentPage,L"页,",L"共",m_iPages,L"页");
	WCHAR buf2[128];
	wsprintf(buf2,L"%s%d%s",L"当前页面信息记录：",num,L"条");
	pShow1->SetText(buf1);
	pSHow2->SetText(buf2);
}

void CLoiter::SetPageBtn()
{
	CButtonUI* pLast = static_cast<CButtonUI*>(m_pManager->FindControl(_T("Loiter_LastPage")));
	CButtonUI* pNext = static_cast<CButtonUI*>(m_pManager->FindControl(_T("Loiter_NextPage")));
	if (m_iCurrentPage == 1)
	{
		pLast->SetEnabled(false);
	}
	else
	{
		pLast->SetEnabled(true);
	}
	if (m_iCurrentPage >= m_iPages)
	{
		pNext->SetEnabled(false);
	}
	else
	{
		pNext->SetEnabled(true);
	}
}


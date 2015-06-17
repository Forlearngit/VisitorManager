#include "StdAfx.h"
#include "BlackList.h"
#include "CommonConvert.h"
#include "LoginWnd.h"
#include "DepartManage.h"

CBlackList::CBlackList(CPaintManagerUI* pManager):m_pManager(pManager)
{
	m_pList = static_cast<CListUI*>(m_pManager->FindControl(_T("BlackList")));
	m_iPages = 1;
}
int CBlackList::m_iCurrentPage = 1;

CBlackList::~CBlackList()
{
	m_pManager = NULL;
	m_pList = NULL;
}

void CBlackList::Paging()
{
	m_pList->RemoveAll();
	CEditUI* pName = static_cast<CEditUI*>(m_pManager->FindControl(_T("Black_SearchEdit")));
	std::string	strName = pName->GetText().GetStringA();
	std::string	y = "'";
	std::string	d = ",";
	std::string b = "%";
	std::string strCount = "select count(ID) from blacklist where name like "+y+b+strName+b+y;
	std::auto_ptr<sql::ResultSet> result = CLoginWnd::m_mySql.ExecuteQuery(strCount);
	if (result->next())
	{
		m_iPages = result->getInt("count(ID)");
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
	std::string strsql = "select * from (select * from blacklist where name like "+y+b+strName+b+y+") table1 where ID limit "+strStart+d+strData;
	std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	while(res->next())
	{
		CDialogBuilder builder;
		CListContainerElementUI* pListItem = NULL;
		pListItem = static_cast<CListContainerElementUI*>(builder.Create(_T("xmls/BlackList_Item.xml"),UINT(0),NULL,m_pManager,NULL));
		int index = m_pList->GetCount();
		pListItem->SetFixedHeight(60);
		m_pList->AddAt(pListItem,index);
		CLabelUI* pName = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(pListItem,_T("BlackList_Name")));
		CLabelUI* pGender = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(pListItem,_T("BlackList_Gender")));
		CLabelUI* pIdentity = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(pListItem,_T("BlackList_Identity")));
		CLabelUI* pAddr = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(pListItem,_T("BlackList_Addr")));
		CLabelUI* pPhone = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(pListItem,_T("BlackList_Phone")));
		CButtonUI* pDel = static_cast<CButtonUI*>(m_pManager->FindSubControlByName(pListItem,_T("BlackList_Delete")));
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
		char phone[200];
		WCHAR wphone[400];
		strcpy_s(phone,sizeof(phone),res->getString("phone").c_str());
		CCommonConvert::Char2WChar(phone,sizeof(phone),wphone);
		pPhone->SetText(wphone);
	}
	ShowStatus();
	SetPageBtn();
}

void CBlackList::SetPageBtn()
{
	CButtonUI* pLast = static_cast<CButtonUI*>(m_pManager->FindControl(_T("Black_LastPage")));
	CButtonUI* pNext = static_cast<CButtonUI*>(m_pManager->FindControl(_T("Black_NextPage")));
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

void CBlackList::ShowStatus()
{
	int num = m_pList->GetCount();
	CLabelUI* pShow1 = static_cast<CLabelUI*>(m_pManager->FindControl(_T("Black_CurrentPage")));
	CLabelUI* pSHow2 = static_cast<CLabelUI*>(m_pManager->FindControl(_T("Black_CurrentPerson")));
	WCHAR buf1[128];
	wsprintf(buf1,L"%s%d%s%s%d%s",L"当前页面：第",m_iCurrentPage,L"页,",L"共",m_iPages,L"页");
	WCHAR buf2[128];
	wsprintf(buf2,L"%s%d%s",L"当前页面信息记录：",num,L"条");
	pShow1->SetText(buf1);
	pSHow2->SetText(buf2);
}

void CBlackList::SelectAll(bool flag)
{
	int num = m_pList->GetCount();
	for (int i=0;i<num;++i)
	{
		CListContainerElementUI* pContainer = static_cast<CListContainerElementUI*>(m_pList->GetItemAt(i));
		COptionUI* pOpt = static_cast<COptionUI*>(m_pManager->FindSubControlByName(pContainer,_T("selectall")));
		pOpt->Selected(flag);
	}
}
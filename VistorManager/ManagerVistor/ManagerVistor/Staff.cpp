#include "StdAfx.h"
#include "Staff.h"
#include "ConectToMySql.h"
#include "LoginWnd.h"
#include "DepartManage.h"
#include "CommonConvert.h"

void CStaffOperate::InitWindow()
{
	// 初始化部门Combo控件
	CComboUI* pParent = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("Staff_Operate_Depart_Combo")));
	pParent->RemoveAll();
	std::string strsql = "select * from depart";
	WCHAR text[100];
	int i=0;
	std::auto_ptr<sql::ResultSet> result = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	while (result->next())
	{
		std::string strDepart = result->getString("name");
		char name[48];
		strcpy_s(name,sizeof(name),strDepart.c_str());
		CCommonConvert::Char2WChar(name,sizeof(name),text);
		CListLabelElementUI* pLabel = new CListLabelElementUI;
		pLabel->SetText(text);
		pLabel->SetTag(i);
		pParent->Add(pLabel);
		pParent->SetItemIndex(pLabel,i);
		++i;
	}
	// 界面控件
	CEditUI* pName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Name_Edit")));
	CEditUI* pCard = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Card_Edit")));
	CEditUI* pID = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_ID_Edit")));
	CEditUI* pTel = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_TelePhone_Edit")));
	CEditUI* pMobile = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_MobilePhone_Edit")));
	CEditUI* pRoom = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Room_Edit")));
	CEditUI* pAddr = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Addr_Edit")));
	CEditUI* pWord = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Password_Edit")));
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Operate_Depart_Edit")));
	CComboUI* pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("Staff_Operate_Depart_Combo")));
	CComboUI* pGender = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("Staff_Gender_Combo")));
	pEdit->SetText(pCombo->GetText());
	// ListContainerElement中的控件
	CLabelUI* cName = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(m_pContainer,_T("StaffName")));
	CLabelUI* cCard = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(m_pContainer,_T("StaffCard")));
	CLabelUI* cID = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(m_pContainer,_T("StaffID")));
	CLabelUI* cTel = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(m_pContainer,_T("StaffPhone")));
	CLabelUI* cMobile = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(m_pContainer,_T("StaffMobile")));
	CLabelUI* cRoom = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(m_pContainer,_T("StaffRoom")));
	CLabelUI* cAddr = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(m_pContainer,_T("StaffAddr")));
	CLabelUI* cWord = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(m_pContainer,_T("StaffPassword")));
	CLabelUI* cDepart = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(m_pContainer,_T("StaffDepart")));
	CLabelUI* cGender = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(m_pContainer,_T("StaffGender")));
	// 显示到界面
	pName->SetText(cName->GetText());
	pCard->SetText(cCard->GetText());
	pID->SetText(cID->GetText());
	pTel->SetText(cTel->GetText());
	pMobile->SetText(cMobile->GetText());
	pRoom->SetText(cRoom->GetText());
	pAddr->SetText(cAddr->GetText());
	pWord->SetText(cWord->GetText());
	pEdit->SetText(cDepart->GetText());
	if (_tcscmp(cGender->GetText().GetData(),_T("男"))==0)
	{
		pGender->SelectItem(0);
	}
	else
	{
		pGender->SelectItem(1);
	}
}

void CStaffOperate::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		CDuiString str = msg.pSender->GetName();
		if (str == _T("Staff_Confirm_btn"))
		{
			Operate();
		}
	}
	if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender->GetName() == _T("Staff_Operate_Depart_Combo"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Operate_Depart_Edit")));
			CComboUI* pCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("Staff_Operate_Depart_Combo")));
			pEdit->SetText(pCombo->GetText());
		}
	}
	return WindowImplBase::Notify(msg);
}

BOOL CStaffOperate::Operate()
{
	// 获取原来的员工编号
	CLabelUI* pLabel = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(m_pContainer,_T("StaffID")));
	std::string strWorkerID = pLabel->GetText().GetStringA();
	std::string y = "'";
	std::string d = ",";
	// 获取编辑后的数据
	CEditUI* pName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Name_Edit")));
	CEditUI* pCard = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Card_Edit")));
	CEditUI* pID = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_ID_Edit")));
	CEditUI* pTel = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_TelePhone_Edit")));
	CEditUI* pMobile = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_MobilePhone_Edit")));
	CEditUI* pRoom = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Room_Edit")));
	CEditUI* pAddr = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Addr_Edit")));
	CEditUI* pWord = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Password_Edit")));
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Operate_Depart_Edit")));
	CComboUI* pGender = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("Staff_Gender_Combo")));
	std::string strName = pName->GetText().GetStringA();
	std::string strCard = pCard->GetText().GetStringA();
	std::string strID = pID->GetText().GetStringA();
	std::string strTel = pTel->GetText().GetStringA();
	std::string strMobile = pMobile->GetText().GetStringA();
	std::string strRoom = pRoom->GetText().GetStringA();
	std::string strAddr = pAddr->GetText().GetStringA();
	std::string strWord = pWord->GetText().GetStringA();
	std::string strDepart = CDepartmentWnd::DepartName2ID(pEdit->GetText().GetStringA());
	std::string strGender;
	if (_tcscmp(pGender->GetText().GetData(),_T(" 男")) == 0)
	{
		strGender = "1";
	}
	else
	{
		strGender = "2";
	}
//	std::string	sqlstr = "insert into staff(name,gender,address,phonenum,callnum,cardnum,depart_id,worker_id,password,room_num) values("+y+strName+y+d+y+strGender+y+d+y+strAddr+y+d+y+strMobile+y+d+y+strTel+y+d+y+strCard+y+d+y+strDepart+y+d+y+strID+y+d+y+strWord+y+d+y+strRoom+y+")";
	std::string	sqlstr = "update staff set name="+y+strName+y+d+"gender="+y+strGender+y+d+"address="+y+strAddr+y+d+"phonenum="+y+strMobile+y+d+"callnum="+y+strTel+y+d+"cardnum="+y+strCard+y+d+"depart_id="+y+strDepart+y+d+"worker_id="+y+strID+y+d+"password="+y+strWord+y+d+"room_num="+y+strRoom+y+"where worker_id="+y+strWorkerID+y;
	if (!CLoginWnd::m_mySql.Execute(sqlstr))
	{
		return FALSE;
	}
	CStaff staff(m_pManager);
	staff.UpdateStaffPage();
	::DestroyWindow(m_hWnd);
	return TRUE;
}

void CStaffAdd::InitWindow()
{
	CComboUI* pParent = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("Staff_Add_Depart_Combo")));
	pParent->RemoveAll();
	std::string strsql = "select * from depart";
	WCHAR text[100];
	int i=0;
	std::auto_ptr<sql::ResultSet> result = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	while (result->next())
	{
		std::string strDepart = result->getString("name");
		char name[48];
		strcpy_s(name,sizeof(name),strDepart.c_str());
		CCommonConvert::Char2WChar(name,sizeof(name),text);
		CListLabelElementUI* pLabel = new CListLabelElementUI;
		pLabel->SetText(text);
		pLabel->SetTag(i);
		pParent->Add(pLabel);
		pParent->SetItemIndex(pLabel,i);
		++i;
	}
}

void CStaffAdd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		CDuiString str = msg.pSender->GetName();
		if (str == _T("Staff_add_btn"))
		{
			Add();
		}
	}
	return WindowImplBase::Notify(msg);
}

BOOL CStaffAdd::Add()
{
	CEditUI* pName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Name_Edit")));
	CEditUI* pCard = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Card_Edit")));
	CEditUI* pID = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_ID_Edit")));
	CEditUI* pTel = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_TelePhone_Edit")));
	CEditUI* pMobile = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_MobilePhone_Edit")));
	CEditUI* pRoom = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Room_Edit")));
	CEditUI* pAddr = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Addr_Edit")));
	CEditUI* pWord = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Password_Edit")));
	CEditUI* pDepart = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("Staff_Add_Depart_Combo")));
	CComboUI* pGender = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("Staff_Gender_Combo")));
	std::string strName = pName->GetText().GetStringA();
	std::string strCard = pCard->GetText().GetStringA();
	std::string strID = pID->GetText().GetStringA();
	std::string strTel = pTel->GetText().GetStringA();
	std::string strMobile = pMobile->GetText().GetStringA();
	std::string strRoom = pRoom->GetText().GetStringA();
	std::string strAddr = pAddr->GetText().GetStringA();
	std::string strWord = pWord->GetText().GetStringA();
	std::string strDepart = CDepartmentWnd::DepartName2ID(pDepart->GetText().GetStringA());
	std::string strGender;
	if (_tcscmp(pGender->GetText().GetData(),_T(" 男")) == 0)
	{
		strGender = "1";
	}
	else
	{
		strGender = "2";
	}
	std::string y = "'";
	std::string d = ",";
	std::string	sqlstr = "insert into staff(name,gender,address,phonenum,callnum,cardnum,depart_id,worker_id,password,room_num) values("+y+strName+y+d+y+strGender+y+d+y+strAddr+y+d+y+strMobile+y+d+y+strTel+y+d+y+strCard+y+d+y+strDepart+y+d+y+strID+y+d+y+strWord+y+d+y+strRoom+y+")";
	if (!CLoginWnd::m_mySql.Execute(sqlstr))
	{
		return FALSE;
	}
	CStaff staff(m_pManager);
	staff.UpdateStaffPage();
	::DestroyWindow(m_hWnd);
	return TRUE;
}

CStaff::CStaff(CPaintManagerUI* pManager):m_pManager(pManager)
{
	m_pList = static_cast<CListUI*>(m_pManager->FindControl(_T("Staff_List")));
}
CStaff::~CStaff()
{
	m_pManager = NULL;
	m_pList = NULL;
}

void CStaff::test()
{
	CDialogBuilder builder;
	CListContainerElementUI* pListItem = NULL;
	pListItem = static_cast<CListContainerElementUI*>(builder.Create(_T("xmls/Staff_Item.xml"),UINT(0),NULL,m_pManager,NULL));
	pListItem->SetFixedHeight(60);
	CLabelUI* pName = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffName"));
	CLabelUI* pGender = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffGender"));
	CLabelUI* pCard = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffCard"));
	CLabelUI* pWorkID = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffID"));
	CLabelUI* pDepart = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffDepart"));
	CLabelUI* pPhone = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffPhone"));
	CLabelUI* pMobile = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffMobile"));
	CLabelUI* pRoom = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffRoom"));
	CLabelUI* pAddr = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffAddr"));
	CLabelUI* pWord = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffPassword"));
	pName->SetText(_T("韦小宝"));
	pGender->SetText(_T("男"));
	pCard->SetText(_T("1001001010100"));
	pWorkID->SetText(_T("007"));
	pDepart->SetText(_T("锦衣卫"));
	pPhone->SetText(_T("010-4884888"));
	pMobile->SetText(_T("15832146033"));
	pRoom->SetText(_T("3-1-901"));
	pAddr->SetText(_T("北京市长安街1号"));
	pWord->SetText(_T("110"));
	int index = m_pList->GetCount();
	m_pList->AddAt(pListItem,index);
}

void CStaff::PaintTree(CTreeNodeUI* pNode,int parent)
{
	std::string	strparent;
	char num[24];
	sprintf_s(num,sizeof(num),"%d",parent);
	strparent = num;
	WCHAR Wname[128];
	std::string	strsql = "select * from depart where parent='"+strparent+"'";
	std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	while(res->next())
	{
		CTreeNodeUI* pchild = new CTreeNodeUI;
		pNode->AddChildNode(pchild);
		char name[64];
		strcpy_s(name,sizeof(name),res->getString("name").c_str());
		CCommonConvert::Char2WChar(name,sizeof(name),Wname);
		int childnum = res->getInt("ID");
		pchild->SetAttribute(_T("text"),Wname);
		pchild->SetAttribute(_T("folderattr"),_T("normalimage=\"file='Image/treeview_a.png' source='0,0,16,16'\" hotimage=\"file='Image/treeview_a.png' source='16,0,32,16'\" selectedimage=\"file='Image/treeview_b.png' source='0,0,16,16'\" selectedhotimage=\"file='Image/treeview_b.png' source='16,0,32,16'\" "));
		PaintTree(pchild,childnum);
	}
}

void CStaff::UpdateStaffPage()
{
	char name[100];
	int gender;
	char card[100];
	char work[100];
	char depart[64];
	char phone[100];
	char mobile[100];
	char room[50];
	char addr[200];
	char word[200];
	WCHAR wname[200];
	CDuiString wgender;
	WCHAR wcard[200];
	WCHAR wwork[200];
	WCHAR wdepart[128];
	WCHAR wphone[200];
	WCHAR wmobile[200];
	WCHAR wroom[100];
	WCHAR waddr[400];
	WCHAR wword[400];
	m_pList->RemoveAll();
	std::string	strsql = "select * from staff";
	std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	while(res->next())
	{
		CDialogBuilder builder;
		CListContainerElementUI* pListItem = NULL;
		pListItem = static_cast<CListContainerElementUI*>(builder.Create(_T("xmls/Staff_Item.xml"),UINT(0),NULL,m_pManager,NULL));
		pListItem->SetFixedHeight(60);
		CLabelUI* pName = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffName"));
		CLabelUI* pGender = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffGender"));
		CLabelUI* pCard = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffCard"));
		CLabelUI* pWorkID = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffID"));
		CLabelUI* pDepart = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffDepart"));
		CLabelUI* pPhone = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffPhone"));
		CLabelUI* pMobile = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffMobile"));
		CLabelUI* pRoom = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffRoom"));
		CLabelUI* pAddr = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffAddr"));
		CLabelUI* pWord = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffPassword"));
		CButtonUI* pOpt = static_cast<CButtonUI*>(m_pManager->FindSubControlByName(pListItem,_T("StaffOperate")));
		pOpt->SetTag((UINT_PTR)pListItem);
		CButtonUI* pDel = static_cast<CButtonUI*>(m_pManager->FindSubControlByName(pListItem,_T("StaffDelete")));
		pDel->SetTag((UINT_PTR)pListItem);
		strcpy_s(name,sizeof(name),res->getString("name").c_str());
		CCommonConvert::Char2WChar(name,sizeof(name),wname);
		pName->SetText(wname);
		gender = res->getInt("gender");
		if (gender == 2)
		{
			wgender = (_T("女"));
		}
		else
		{
			wgender = (_T("男"));
		}
		pGender->SetText(wgender);
		strcpy_s(card,sizeof(card),res->getString("cardnum").c_str());
		CCommonConvert::Char2WChar(card,sizeof(card),wcard);
		pCard->SetText(wcard);
		strcpy_s(work,sizeof(work),res->getString("worker_id").c_str());
		CCommonConvert::Char2WChar(work,sizeof(work),wwork);
		pWorkID->SetText(wwork);
		int id = res->getInt("depart_id");
		std::string	strID = CDepartmentWnd::DepartID2Name(id);
		strcpy_s(depart,sizeof(depart),strID.c_str());
		CCommonConvert::Char2WChar(depart,sizeof(depart),wdepart);
		pDepart->SetText(wdepart);
		strcpy_s(phone,sizeof(phone),res->getString("callnum").c_str());
		CCommonConvert::Char2WChar(phone,sizeof(phone),wphone);
		pPhone->SetText(wphone);
		strcpy_s(mobile,sizeof(mobile),res->getString("phonenum").c_str());
		CCommonConvert::Char2WChar(mobile,sizeof(mobile),wmobile);
		pMobile->SetText(wmobile);
		strcpy_s(room,sizeof(room),res->getString("room_num").c_str());
		CCommonConvert::Char2WChar(room,sizeof(room),wroom);
		pRoom->SetText(wroom);
		strcpy_s(addr,sizeof(addr),res->getString("address").c_str());
		CCommonConvert::Char2WChar(addr,sizeof(addr),waddr);
		pAddr->SetText(waddr);
		strcpy_s(word,sizeof(word),res->getString("password").c_str());
		CCommonConvert::Char2WChar(word,sizeof(word),wword);
		pWord->SetText(wword);
		int index = m_pList->GetCount();
		m_pList->AddAt(pListItem,index);
	}
	CComboUI* pParent = static_cast<CComboUI*>(m_pManager->FindControl(_T("Staff_SearchCombo")));
	pParent->RemoveAll();
	strsql = "select * from depart";
	WCHAR text[100];
	int i=0;
	std::auto_ptr<sql::ResultSet> result = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	while (result->next())
	{
		std::string strDepart = result->getString("name");
		char name[48];
		strcpy_s(name,sizeof(name),strDepart.c_str());
		CCommonConvert::Char2WChar(name,sizeof(name),text);
		CListLabelElementUI* pLabel = new CListLabelElementUI;
		pLabel->SetText(text);
		pLabel->SetTag(i);
		pParent->Add(pLabel);
		pParent->SetItemIndex(pLabel,i);
		++i;
	}
	ShowStatus();
}	

void CStaff::Search()
{
	// 判断查询条件，执行查询
	CEditUI* pName = static_cast<CEditUI*>(m_pManager->FindControl(_T("Staff_SearchEdit")));
	CComboUI* pDepart = static_cast<CComboUI*>(m_pManager->FindControl(_T("Staff_SearchCombo")));
	CDuiString strName = pName->GetText();
	CDuiString strDepart = pDepart->GetText();
	std::string strsql;
	std::string y = "'";
	std::string d = "%";
	if (_tcscmp(strName.GetData(),_T(""))==0)
	{
		if (_tcscmp(strDepart.GetData(),_T(""))==0 || _tcscmp(strDepart.GetData(),_T("无"))==0)
		{
			strsql = "select * from staff";
		}
		else
		{
			std::string	strDepartID = CDepartmentWnd::DepartName2ID(strDepart.GetStringA());
			strsql = "select * from staff where depart_id="+y+strDepartID+y;
		}
	}
	else
	{
		if (_tcscmp(strDepart.GetData(),_T(""))==0 || _tcscmp(strDepart.GetData(),_T("无"))==0)
		{
			strsql = "select * from staff where name like"+y+d+strName.GetStringA()+d+y;
		}
		else
		{
			std::string	strDepartID = CDepartmentWnd::DepartName2ID(strDepart.GetStringA());
			strsql = "select * from staff where name like"+y+d+strName.GetStringA()+d+y+" and depart_id="+y+strDepartID+y;
		}
	}
	std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	// 将查询结果显示到界面
	char name[100];
	int gender;
	char card[100];
	char work[100];
	char depart[64];
	char phone[100];
	char mobile[100];
	char room[50];
	char addr[200];
	char word[200];
	WCHAR wname[200];
	CDuiString wgender;
	WCHAR wcard[200];
	WCHAR wwork[200];
	WCHAR wdepart[128];
	WCHAR wphone[200];
	WCHAR wmobile[200];
	WCHAR wroom[100];
	WCHAR waddr[400];
	WCHAR wword[400];
	m_pList->RemoveAll();
	while(res->next())
	{
		CDialogBuilder builder;
		CListContainerElementUI* pListItem = NULL;
		pListItem = static_cast<CListContainerElementUI*>(builder.Create(_T("xmls/Staff_Item.xml"),UINT(0),NULL,m_pManager,NULL));
		pListItem->SetFixedHeight(60);
		CLabelUI* pName = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffName"));
		CLabelUI* pGender = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffGender"));
		CLabelUI* pCard = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffCard"));
		CLabelUI* pWorkID = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffID"));
		CLabelUI* pDepart = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffDepart"));
		CLabelUI* pPhone = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffPhone"));
		CLabelUI* pMobile = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffMobile"));
		CLabelUI* pRoom = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffRoom"));
		CLabelUI* pAddr = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffAddr"));
		CLabelUI* pWord = (CLabelUI*)m_pManager->FindSubControlByName(pListItem,_T("StaffPassword"));
		strcpy_s(name,sizeof(name),res->getString("name").c_str());
		CCommonConvert::Char2WChar(name,sizeof(name),wname);
		pName->SetText(wname);
		gender = res->getInt("gender");
		if (gender == 2)
		{
			wgender = (_T("女"));
		}
		else
		{
			wgender = (_T("男"));
		}
		pGender->SetText(wgender);
		strcpy_s(card,sizeof(card),res->getString("cardnum").c_str());
		CCommonConvert::Char2WChar(card,sizeof(card),wcard);
		pCard->SetText(wcard);
		strcpy_s(work,sizeof(work),res->getString("worker_id").c_str());
		CCommonConvert::Char2WChar(work,sizeof(work),wwork);
		pWorkID->SetText(wwork);
		int id = res->getInt("depart_id");
		std::string	strID = CDepartmentWnd::DepartID2Name(id);
		strcpy_s(depart,sizeof(depart),strID.c_str());
		CCommonConvert::Char2WChar(depart,sizeof(depart),wdepart);
		pDepart->SetText(wdepart);
		strcpy_s(phone,sizeof(phone),res->getString("callnum").c_str());
		CCommonConvert::Char2WChar(phone,sizeof(phone),wphone);
		pPhone->SetText(wphone);
		strcpy_s(mobile,sizeof(mobile),res->getString("phonenum").c_str());
		CCommonConvert::Char2WChar(mobile,sizeof(mobile),wmobile);
		pMobile->SetText(wmobile);
		strcpy_s(room,sizeof(room),res->getString("room_num").c_str());
		CCommonConvert::Char2WChar(room,sizeof(room),wroom);
		pRoom->SetText(wroom);
		strcpy_s(addr,sizeof(addr),res->getString("address").c_str());
		CCommonConvert::Char2WChar(addr,sizeof(addr),waddr);
		pAddr->SetText(waddr);
		strcpy_s(word,sizeof(word),res->getString("password").c_str());
		CCommonConvert::Char2WChar(word,sizeof(word),wword);
		pWord->SetText(wword);
		int index = m_pList->GetCount();
		m_pList->AddAt(pListItem,index);
	}
	// 底部状态栏
	ShowStatus();
}

void CStaff::SelectAll(bool flag)
{
	int num = m_pList->GetCount();
	for (int i=0;i<num;++i)
	{
		CListContainerElementUI* pContainer = static_cast<CListContainerElementUI*>(m_pList->GetItemAt(i));
		COptionUI* pOpt = static_cast<COptionUI*>(m_pManager->FindSubControlByName(pContainer,_T("selectall")));
		pOpt->Selected(flag);
	}
}

void CStaff::ShowStatus()
{
	int num = m_pList->GetCount();
	WCHAR buf1[128];
	wsprintf(buf1,L"%s%d%s",L"当前人员信息记录：",num	,L"条");
	CLabelUI* pShow1 = static_cast<CLabelUI*>(m_pManager->FindControl(_T("currentperson")));
	pShow1->SetText(buf1);
	CLabelUI* pSHow2 = static_cast<CLabelUI*>(m_pManager->FindControl(_T("currentdepart")));
	CComboUI* pCombo = static_cast<CComboUI*>(m_pManager->FindControl(_T("Staff_SearchCombo")));
	std::wstring wstr = pCombo->GetText().GetStringW();
	WCHAR buf2[128];
	wsprintf(buf2,L"%s%s",L"当前选择的部门：",wstr.c_str());
	pSHow2->SetText(buf2);
}

void CStaff::DeleteSelected()
{
	int num = m_pList->GetCount();
	for (int i=0;i<num;++i)
	{
		CListContainerElementUI* pContainer = static_cast<CListContainerElementUI*>(m_pList->GetItemAt(i));
		COptionUI* pOpt = static_cast<COptionUI*>(m_pManager->FindSubControlByName(pContainer,_T("selectall")));
		if (pOpt->IsSelected())
		{
			CLabelUI* pLabel = static_cast<CLabelUI*>(m_pManager->FindSubControlByName(pContainer,_T("StaffID")));
			std::string strWorkerID = pLabel->GetText().GetStringA();
			std::string y = "'";
			std::string strsql = "delete from staff where worker_id="+y+strWorkerID+y;
			CLoginWnd::m_mySql.Execute(strsql);
		}
	}
}
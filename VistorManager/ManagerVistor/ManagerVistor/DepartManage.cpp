#include "stdafx.h"
#include "DepartManage.h"
#include "ConectToMySql.h"
#include "LoginWnd.h"
#include "CommonConvert.h"

LRESULT CDepartmentWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 屏蔽双击标题栏最大化
	switch (uMsg)
	{
	case WM_LBUTTONDBLCLK:
		return 0;
		break;
	}
	return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CDepartmentWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		CDuiString str = msg.pSender->GetName();
		if (str == _T("DepartmentAdd"))
		{
			// 往数据库添加部门信息然后刷新页面
			AddDepartment();
			InsertDepartment();
			UpdateDepartmentList();
		}
		else if (str == _T("DepartmentDelete"))
		{
			// 删除数据库中的相应信息然后刷新页面
			DeleteDepartment();
			InsertDepartment();
			UpdateDepartmentList();
		}
	}
	else if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender->GetName()==_T("DepartmentList"))
		{
			CListUI* pList = static_cast<CListUI*>(msg.pSender);	
			int i = pList->GetCurSel();
			CListTextElementUI *pText = static_cast<CListTextElementUI*>(pList->GetItemAt(i));
			CEditUI* pName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentName")));
			pName->SetText(pText->GetText(1));
			CEditUI* pNumber = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentNumber")));
			pNumber->SetText(pText->GetText(0));
			CEditUI* pPhone = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentPhone")));
			pPhone->SetText(pText->GetText(3));
			CEditUI* pLocation = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentLocation")));
			pLocation->SetText(pText->GetText(4));
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentParent")));
			pEdit->SetText(pText->GetText(2));
		}
		else if (msg.pSender->GetName()==_T("DepatmentCombo"))
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentParent")));
			CComboUI* pParent = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("DepatmentCombo")));
			pEdit->SetText(pParent->GetText());
		}
	}
	return WindowImplBase::Notify(msg);
}

void CDepartmentWnd::InsertDepartment()
{
	CComboUI* pParent = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("DepatmentCombo")));
	pParent->RemoveAll();
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentParent")));
	pEdit->SetText(pParent->GetText());
	std::string	strsql = "select * from depart";
	WCHAR wname[100];
	int i=0;
	std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	while (res->next())
	{
		std::string strDepart = res->getString("name");
		//	if (strDepart != "无")
		//	{
		char name[48];
		strcpy_s(name,sizeof(name),strDepart.c_str());
		CCommonConvert::Char2WChar(name,sizeof(name),wname);
		CListLabelElementUI* pLabel = new CListLabelElementUI;
		pLabel->SetText(wname);
		pLabel->SetTag(i);
		pParent->Add(pLabel);
		pParent->SetItemIndex(pLabel,i);
		++i;
		//}
	}
}

void CDepartmentWnd::InitWindow()
{
	// 连接数据库向Combo中插入存在部门
	InsertDepartment();
	// 将数据库的数据展示到表格中
	UpdateDepartmentList();
}

void CDepartmentWnd::UpdateDepartmentList()
{
	CListUI *pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("DepartmentList")));
	TListInfoUI *pInfo = pList->GetListInfo();
	pInfo->uTextStyle = DT_CENTER|DT_VCENTER;
	pList->RemoveAll();
	std::string strSql = "select * from depart";
	std::auto_ptr<sql::ResultSet> result = CLoginWnd::m_mySql.ExecuteQuery(strSql);
	int i = 0;
	CDuiString strID;
	CDuiString strParent;
	WCHAR Wname[128];
	WCHAR Wparent[128];
	WCHAR Wphonenum[128];
	WCHAR Wlocation[256];
	while (result->next())
	{
		strID.Format(_T("%d"),result->getInt("ID"));
		if (strID == _T("1"))
		{
			continue;
		}
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(i);
		pList->Add(pListElement);
		char name[64];
		char parent[64];
		char phonenum[64];
		char location[128];
		strcpy_s(name,sizeof(name),result->getString("name").c_str());
		CCommonConvert::Char2WChar(name,sizeof(name),Wname);
		strParent.Format(_T("%d"),result->getInt("parent"));
		std::string str = "select name from depart where ID='"+strParent.GetStringA()+"'";
		std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(str);
		if (res->next())
		{
			strcpy_s(parent,sizeof(parent),res->getString("name").c_str());
			CCommonConvert::Char2WChar(parent,sizeof(parent),Wparent);
		}
		strcpy_s(phonenum,sizeof(phonenum),result->getString("phonenum").c_str());
		CCommonConvert::Char2WChar(phonenum,sizeof(phonenum),Wphonenum);
		strcpy_s(location,sizeof(location),result->getString("location").c_str());
		CCommonConvert::Char2WChar(location,sizeof(location),Wlocation);
		++i;
		pListElement->SetText(0,strID);
		pListElement->SetText(1,Wname);
		pListElement->SetText(2,Wparent);
		pListElement->SetText(3,Wphonenum);
		pListElement->SetText(4,Wlocation);
	}
}

// 增加部门
void CDepartmentWnd::AddDepartment()
{
	CEditUI* pName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentName")));
	CDuiString strName = pName->GetText();
	CEditUI* pNumber = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentNumber")));
	CDuiString strNumber = pNumber->GetText();
	CEditUI* pPhone = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentPhone")));
	CDuiString strPhone = pPhone->GetText();
	CEditUI* pLocation = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentLocation")));
	CDuiString strLocation = pLocation->GetText();
	CComboUI* pParent = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("DepatmentCombo")));
	CDuiString strParent = pParent->GetText();
	std::string y = "'";
	std::string	d = ",";
	std::string	strsql = "select * from depart where ID ="+y+strNumber.GetStringA()+y; 
	std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	if (res->next())
	{
		MessageBox(NULL,_T("该编号已经存在，部门编号必须唯一！"),_T("提示"),NULL);
		return;
	}
	strsql = "select * from depart where name="+y+strName.GetStringA()+y;
	res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	if (res->next())
	{
		MessageBox(NULL,_T("该部门已经存在，部门名称必须唯一！"),_T("提示"),NULL);
		return;
	}
	// 查找其父部门的ID，如果父部门是'无'，则数据库中parent字段表现为1，即把‘无’部门的ID设置为1，所以‘无’部门要事先添加到数据库中
	strsql = "select * from depart where name='无'";
	res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	if (!res->next())
	{
		// 数据库中没有‘无’部门时，添加‘无’部门
		strsql = "insert into depart(ID,name) values('1','无')";
		CLoginWnd::m_mySql.Execute(strsql);
	}
	// 这样就可以直接根据父部门名查找对应的ID，设置到parent字段中
	strsql = "select ID from depart where name ="+y+strParent.GetStringA()+y;
	res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	if (res->next())
	{
		int id = res->getInt("ID");
		strParent.Format(_T("%d"),id);
	}
	strsql = "insert into depart(ID,name,parent,phonenum,location) values("+y+strNumber.GetStringA()+y+d+y+strName.GetStringA()+y+d+y+strParent.GetStringA()+y+d+y+strPhone.GetStringA()+y+d+y+strLocation.GetStringA()+y+")";
	if (CLoginWnd::m_mySql.Execute(strsql))
	{
		MessageBox(NULL,_T("添加成功"),_T("提示"),NULL);
		pLocation->SetText(_T(""));
		pName->SetText(_T(""));
		pNumber->SetText(_T(""));
		pParent->SetText(_T(""));
		pPhone->SetText(_T(""));
	}
}

void CDepartmentWnd::DeleteDepartment()
{
	CEditUI* pName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentName")));
	CDuiString strName = pName->GetText();
	if (_tcscmp(strName.GetData(),_T(""))==0)
	{
		MessageBox(NULL,_T("先选中您要删除的部门！"),_T("提示"),NULL);
		return;
	}
	std::string strsql = "delete from depart where name='"+strName.GetStringA()+"'";
	if (CLoginWnd::m_mySql.Execute(strsql))
	{
		MessageBox(NULL,_T("成功删除该部门！"),_T("提示"),NULL);
	}
	pName->SetText(_T(""));
	CEditUI* pNumber = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentNumber")));
	pNumber->SetText(_T(""));
	CEditUI* pPhone = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentPhone")));
	pPhone->SetText(_T(""));
	CEditUI* pLocation = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("DepartmentLocation")));
	pLocation->SetText(_T(""));
	//	CComboUI* pParent = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("DepatmentCombo")));
}

std::string	CDepartmentWnd::DepartID2Name(int departID)
{
	char num[64];
	sprintf_s(num,sizeof(num),"%d",departID);
	std::string strID = num;
	std::string	strsql = "select name from depart where ID='"+strID+"'";
	std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	if (res->next())
	{
		return res->getString("name");
	}
	else
	{
		return "";
	}
}

std::string CDepartmentWnd::DepartName2ID(string strName)
{
	std::string	strsql = "select ID from depart where name='"+strName+"'";
	std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(strsql);
	if (res->next())
	{
		int departID = res->getInt("ID");
		char num[64];
		sprintf_s(num,sizeof(num),"%d",departID);
		std::string strID = num;
		return strID;
	}
	else
	{
		return "";
	}
}
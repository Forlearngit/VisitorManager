#include "stdafx.h"
#include "PowerWnd.h"
#include "LoginWnd.h"
#include "CommonConvert.h"

void CPowerWnd::InitWindow()
{
	// 普通用户，隐藏管理界面
	if (CLoginWnd::m_iPower == 1)
	{
		COptionUI* pOpt = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("manage")));
		pOpt->SetAttribute(_T("visible"),_T("false"));
	}
}

LRESULT CPowerWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 屏蔽双击标题栏消息
	switch (uMsg)
	{
	case WM_NCLBUTTONDBLCLK:
		return 0;
		break;
	}
	return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

BOOL CPowerWnd::ChangePassWord()
{
	CEditUI *pOld = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("old")));
	CEditUI *pNew = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("new")));
	CEditUI *pConfirm = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("confirm")));
	CDuiString strOld = pOld->GetText();
	CDuiString strNew = pNew->GetText();
	CDuiString strConfirm = pConfirm->GetText();
	if ( strOld != CLoginWnd::m_strPassWord )
	{
		MessageBox(NULL,_T("密码错误"),_T("提示"),NULL);
		return False;
	}
	if (strNew != strConfirm)
	{
		MessageBox(NULL,_T("输入不一致"),_T("提示"),NULL);
		return False;
	}
	std::string	str = "'";
	std::string	strSql = "update manager set password = "+str+strConfirm.GetStringA()+str+" where name = "+str+CLoginWnd::m_strAccount.GetStringA()+str;
	if (CLoginWnd::m_mySql.Execute(strSql))
	{
		MessageBox(NULL,_T("更改成功"),_T("提示"),NULL);
		CLoginWnd::m_strPassWord = strConfirm;
		pOld->SetText(_T(""));
		pNew->SetText(_T(""));
		pConfirm->SetText(_T(""));
	}
	return True;
}

void CPowerWnd::UpdatePage()
{
	CListUI *pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("stafflist")));
	pList->RemoveAll();
	std::string strSql = "select * from manager";
	std::auto_ptr<sql::ResultSet> result = CLoginWnd::m_mySql.ExecuteQuery(strSql);
	int i = 0;
	CDuiString Number,Power;
	WCHAR Wname[100];
	while(result->next())
	{
		TListInfoUI *pInfo = pList->GetListInfo();
		pInfo->uTextStyle = DT_CENTER|DT_VCENTER;
		CListTextElementUI* pListElement = new CListTextElementUI;
		pListElement->SetTag(i);
		pList->Add(pListElement);
		Number.Format(_T("%d"),i);
		char name[48];
		strcpy_s(name,sizeof(name),result->getString("name").c_str());
		CCommonConvert::Char2WChar(name,sizeof(name),Wname);
		Power = result->getInt("power")?_T("普通用户"):_T("管理员");
		i++;
		pListElement->SetText(0,Number);
		pListElement->SetText(1,Wname);
		pListElement->SetText(2,Power);
	}
}

void CPowerWnd::AddStaff()
{
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("add_edit")));
	CDuiString strName = pEdit->GetText();
	if (strName == _T("")||strName == _T("请输入要添加的账号..."))
	{
		MessageBox(NULL,_T("请输入账号"),_T("提示"),NULL);
		return;
	}
	std::string	str = "'";
	std::string strSql = "select * from manager where name = "+str+strName.GetStringA()+str;
	std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(strSql);
	if (res->next())
	{
		MessageBox(NULL,_T("该员工已经存在"),_T("提示"),NULL);
		return;
	}
	strSql = "insert into manager(name,password,power) values("+str+strName.GetStringA()+str+",'123','1')";
	if (CLoginWnd::m_mySql.Execute(strSql))
	{
		MessageBox(NULL,_T("添加成功"),_T("提示"),NULL);
		pEdit->SetText(_T(""));
		UpdatePage();
	}
}

void CPowerWnd::DeleteStaff()
{
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("selected")));
	CDuiString strName = pEdit->GetText();
	std::string str = "'";
	std::string	strSql = "select * from manager where name = "+str+strName.GetStringA()+str;
	std::auto_ptr<sql::ResultSet> res = CLoginWnd::m_mySql.ExecuteQuery(strSql);
	if (!res->next())
	{
		MessageBox(NULL,_T("该员工不存在"),_T("提示"),NULL);
		return;
	}
	strSql = "delete from manager where name="+str+strName.GetStringA()+str;
	if (CLoginWnd::m_mySql.Execute(strSql))
	{
		MessageBox(NULL,_T("删除成功"),_T("提示"),NULL);
		pEdit->SetText(_T(""));
		UpdatePage();
	}
}

void CPowerWnd::ToUser()
{
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("selected")));
	CDuiString strName = pEdit->GetText();
	std::string str = "'";
	std::string strSql = "update manager set power = '1' where name = "+str+strName.GetStringA()+str;
	if (CLoginWnd::m_mySql.Execute(strSql))
	{
		MessageBox(NULL,_T("更改成功"),_T("提示"),NULL);
		pEdit->SetText(_T(""));
		UpdatePage();
	}
}

void CPowerWnd::ToManager()
{
	CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("selected")));
	CDuiString strName = pEdit->GetText();
	std::string str = "'";
	std::string strSql = "update manager set power = '0' where name = "+str+strName.GetStringA()+str;
	if (CLoginWnd::m_mySql.Execute(strSql))
	{
		MessageBox(NULL,_T("更改成功"),_T("提示"),NULL);
		pEdit->SetText(_T(""));
		UpdatePage();
	}
}

void CPowerWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		CDuiString str = msg.pSender->GetName();
		CTabLayoutUI* pTab = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("setTab")));
		if ( str == _T("modify"))
		{
			pTab->SelectItem(0);
		}
		else if ( str == _T("manage"))
		{
			if (CLoginWnd::m_iPower)
			{
				// 普通权限，不显示管理界面
				MessageBox(NULL,_T("你没有这个权限"),_T("提示"),NULL);
			}
			else
			{
				pTab->SelectItem(1);
				UpdatePage();
			}
		}
		else if ( str == _T("submit"))
		{
			ChangePassWord();			
		}
		else if ( str == _T("add_btn"))
		{
			AddStaff();
		}
		else if ( str == _T("dele_btn"))
		{
			DeleteStaff();
		}
		else if ( str == _T("touser_btn"))
		{
			ToUser();
		}
		else if ( str == _T("tomanager_btn"))
		{
			ToManager();
		}
	}
	else if (msg.sType == _T("itemselect"))
	{
		CEditUI* pEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("selected")));
		CListUI* pList = static_cast<CListUI*>(msg.pSender);	
		int i = pList->GetCurSel();
		CListTextElementUI *pText = static_cast<CListTextElementUI*>(pList->GetItemAt(i));
		pEdit->SetText(pText->GetText(1));
	}
	return WindowImplBase::Notify(msg);
}
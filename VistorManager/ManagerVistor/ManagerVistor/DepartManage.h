#pragma  once	

class CDepartmentWnd:public WindowImplBase
{
public:
	virtual LPCTSTR GetWindowClassName()const{return _T("DepartmentWnd");}
	virtual CDuiString GetSkinFolder(){return _T("");}
	virtual CDuiString GetSkinFile(){return _T("xmls/Department.xml");}
	virtual void Notify(TNotifyUI& msg);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void AddDepartment();
	void DeleteDepartment();
	void UpdateDepartmentList();
	void InsertDepartment();
public:
	static std::string	DepartID2Name(int departID);
	static std::string  DepartName2ID(string strName);
};
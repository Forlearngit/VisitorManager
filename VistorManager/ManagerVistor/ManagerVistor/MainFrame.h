#pragma once

class CCaptureVideo;

class CMainFrame : public WindowImplBase
{
public:
	CMainFrame(void);
	~CMainFrame();
public:
	virtual LPCTSTR GetWindowClassName() const{return _T("XMLWnd");}
	virtual CDuiString GetSkinFile(){return _T("xmls/MainFrame.xml");}
	virtual CDuiString GetSkinFolder(){return _T("");}
	virtual CControlUI* CreateControl( LPCTSTR pstrClassName );
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void InitWindow();
	void Notify(TNotifyUI& msg);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
private:
	void InitSearchCtrl();
	void AddCarNumTypeMsg();
	BOOL StartVdieoCap();
	void AddCertyTypeMsg();
	void AddVistorNum();//访问人数
	void AddVistorBatch();//访问批次
	void AddGoodsNum();//物品数量
private:
	CControlUI* m_pMenuBtn;
private:
	//recordUI控件
	CComboUI* m_pCertiTypeCombo;
	CEditUI* m_pCertiNumEdit;
	CEditUI* m_pVistorNameEdit;
	CComboUI* m_pSexCombo;
	CEditUI* m_pCardNumEdit;
	CEditUI* m_pAddressEdit;
	CComboUI* m_pCarNumCombo;
	CEditUI* m_pCarNumEdit;
	CComboUI* m_pCarTypeCombo;
	CButtonUI* m_pAddCarTypeBtn;
	CEditUI* m_pVistorUnitEdit;
	CEditUI* m_pVistorPhoneNumEdit;
	CComboUI* m_pVistorNumCombo;
	CButtonUI* m_pAddVistorBtn;
	CDateTimeUI* m_pLeaveDate;
	CDateTimeUI* m_pLeaveTime;
	CComboUI* m_pBatchCombo;
	CButtonUI* m_pAddBatchBtn;
	CComboUI* m_pGoodsNumCombo;
	CComboUI* m_pGoodsCombo;
private:
	CEditUI* m_pCarTypeEdit;
	CEditUI* m_pVistorNumEdit;
	CEditUI* m_pBatchEdit;
	CEditUI* m_pGoodsEdit;
	CEditUI* m_pGoodsNumEdit;
private:
	CControlUI* m_pPerviewCtrl;
	CControlUI* m_pVdieoCtrl;
private:
	CDialogBuilder m_dlgBuilder;
	CListContainerElementUI* m_LastElement;
private:
	CCaptureVideo* m_pCaptureVideo;
	RECT m_rShowVdieo;//显示视频图像区域
	BOOL m_fShowCaptureVdieo;//显示隐藏图像切换
};
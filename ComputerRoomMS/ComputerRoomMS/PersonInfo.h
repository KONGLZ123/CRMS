#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "ListCtrlOwner.h"
#include "WinThreadDatabase.h"
#include "ViewReigstRequestDlg.h"
// CPersonInfo 对话框

class CPersonInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CPersonInfo)

public:
	CPersonInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPersonInfo();

	enum { IDD = IDD_DLG_PERSON_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
    afx_msg void OnCbnSelchangeComboRole();
    afx_msg void OnBnClickedBtnSearchStudent();
    afx_msg void OnBnClickedBtnUpload();
    afx_msg void OnBnClickedBtnAddPerson();
    afx_msg void OnBnClickedBtnViewRegister();
    LRESULT OnUpdateListItem(WPARAM wParam, LPARAM lParam);
    LRESULT OnDelListItem(WPARAM wParam, LPARAM lParam);
    LRESULT OnAddListItem(WPARAM wParam, LPARAM lParam);

private:
    void InitCtrls();
    BOOL InitData();

public:
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);
    void SetPersonInfo(DATA_STOCK &personInfo);
    void SetRegisterData(vector<ADD_PERSON_DATA> &vecRegisterData);
    //void UpdatePersonInfo(PERSON_INFO_PART *pPersonInfo);

public:
    CComboBox                   m_comboRole;
    CEdit                       m_editSearch;
    CListCtrlOwner              m_listPersonInfo;
    CWinThreadDatabase *        m_pDbThread;
    DATA_STOCK                  m_personInfo;
    vector<ADD_PERSON_DATA>     m_vecRegisterData;
    CViewReigstRequestDlg       m_registeDlg;
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

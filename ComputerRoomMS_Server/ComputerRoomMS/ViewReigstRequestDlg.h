#pragma once
#include "WinThreadDatabase.h"

// CViewReigstRequestDlg 对话框

class CViewReigstRequestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewReigstRequestDlg)

public:
	CViewReigstRequestDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewReigstRequestDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_VIEW_REGISTER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedBtnPageUp();
    afx_msg void OnBnClickedBtnPageDown();
    afx_msg void OnBnClickedBtnOk();
    afx_msg void OnBnClickedBtnRegiester();

	DECLARE_MESSAGE_MAP()

    void SetEditText(ADD_PERSON_DATA personData);

public:
    void SetRegisterData(vector<ADD_PERSON_DATA> &vecRegisterData);

private:
    vector<ADD_PERSON_DATA>         m_vecRegisterData;
    CWinThreadDatabase *            m_pDbThread;
    ADD_PERSON_DATA                 m_registerData;
    int                             m_pageIndex;


};

#pragma once
#include "afxcmn.h"


// CSalaryCheckDlg 对话框

class CSalaryCheckDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSalaryCheckDlg)

public:
	CSalaryCheckDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSalaryCheckDlg();


	enum { IDD = IDD_DLG_SALARY_CHECK };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_listSalaryCheck;
    virtual BOOL OnInitDialog();
};

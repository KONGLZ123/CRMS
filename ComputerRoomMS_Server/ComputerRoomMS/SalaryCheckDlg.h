#pragma once
#include "afxcmn.h"


// CSalaryCheckDlg �Ի���

class CSalaryCheckDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSalaryCheckDlg)

public:
	CSalaryCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSalaryCheckDlg();


	enum { IDD = IDD_DLG_SALARY_CHECK };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_listSalaryCheck;
    virtual BOOL OnInitDialog();
};

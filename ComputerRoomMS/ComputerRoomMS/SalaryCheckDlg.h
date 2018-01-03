#pragma once
#include "afxcmn.h"
#include "ListCtrlOwner.h"

// CSalaryCheckDlg �Ի���

class CSalaryCheckDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSalaryCheckDlg)

public:
	CSalaryCheckDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSalaryCheckDlg();


	enum { IDD = IDD_DLG_SALARY_CHECK };
    void SetStyle(LIST_STYLE style);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    void SetSalaryInfo(vector<WORKLOAD_REPORT> & vecSalaryInfo);
    CListCtrlOwner m_listSalaryCheck;
    virtual BOOL OnInitDialog();
    afx_msg void OnNMClickListSalaryCheck(NMHDR *pNMHDR, LRESULT *pResult);
    vector<WORKLOAD_REPORT> m_vecSalaryInfo;
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);
    CWinThreadDatabase *    m_pDbThread;

    LIST_STYLE      m_style;
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    LRESULT OnUpdateSalaryListItem(WPARAM wParam, LPARAM lParam);
};

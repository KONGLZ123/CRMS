#pragma once
#include "WinThreadDatabase.h"

// CViewRepairDlg 对话框

class CViewRepairDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewRepairDlg)

public:
	CViewRepairDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewRepairDlg();

	enum { IDD = IDD_DLG_VIEW_REPAIR };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    afx_msg void OnBnClickedBtnPageup();
    afx_msg void OnBnClickedBtnPagedow();
    DECLARE_MESSAGE_MAP()

public:
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);
    void SeRepairData(vector<REPORT_DATA>& vecRepairData);

private:
    CWinThreadDatabase *            m_pDbThread;
    vector<REPORT_DATA>             m_vecRepairData;
    REPORT_DATA                     m_repairData;
    int                             m_pageIndex;
};

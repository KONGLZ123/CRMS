#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "ListCtrlOwner.h"

// CViewInfoDlg 对话框

class CViewInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewInfoDlg)

public:
	CViewInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewInfoDlg();

	enum { IDD = IDD_DLG_VIEW_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
    void SetReportData(vector<REPORT_DATA> &reportData);
    LRESULT OnUpdateList(WPARAM wParam, LPARAM lParam);

private:
    void InitCtrl();

public:
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);
    CWinThreadDatabase *    m_pDbThread;
    CListCtrlOwner          m_listChectReport;
    CComboBox               m_comboReportStyle;
    vector<REPORT_DATA>     m_reportData;
    afx_msg void OnCbnSelchangeComboReportStyle();
    afx_msg void OnNMClickListPersonInfo(NMHDR *pNMHDR, LRESULT *pResult);
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    
};

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


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

private:
    void InitCtrl();

public:
    CListCtrl               m_listChectReport;
    CComboBox               m_comboReportStyle;
    vector<REPORT_DATA>     m_reportData;
    afx_msg void OnCbnSelchangeComboReportStyle();
};

#pragma once
#include "WinThreadDatabase.h"

// CServerSetupDlg 对话框

class CServerSetupDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CServerSetupDlg)

public:
	CServerSetupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CServerSetupDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SERVER_SETUP};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();
    virtual BOOL OnInitDialog();
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);

private:
    CWinThreadDatabase* m_pDbThread;
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

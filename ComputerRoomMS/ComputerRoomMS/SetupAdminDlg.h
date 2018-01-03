#pragma once
#include "WinThreadDatabase.h"

// CSetupAdminDlg 对话框

class CSetupAdminDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupAdminDlg)

public:
	CSetupAdminDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetupAdminDlg();

	enum { IDD = IDD_DLG_SETUP_ADMIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedBtnAsetup();
    void SetAdminInfo(SYSADMIN_INFO & sysAdminInfo);
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);

private:
    SYSADMIN_INFO           m_adminInfo;
    CWinThreadDatabase *    m_pDbThread;
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

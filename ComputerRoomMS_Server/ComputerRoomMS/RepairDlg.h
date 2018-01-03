#pragma once
#include "WinThreadDatabase.h"

// CRepairDlg 对话框

class CRepairDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRepairDlg)

public:
	CRepairDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRepairDlg();

	enum { IDD = IDD_DLG_REPAIR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()


public:
    void SetCurUserName(CString strUserName);
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);

private:
    CWinThreadDatabase *    m_pDbThread;
    CString                 m_curUserName;
};

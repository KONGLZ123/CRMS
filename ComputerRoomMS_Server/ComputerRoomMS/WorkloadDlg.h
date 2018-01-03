#pragma once
#include "WinThreadDatabase.h"

// CWorkloadDlg 对话框

class CWorkloadDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkloadDlg)

public:
	CWorkloadDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWorkloadDlg();


	enum { IDD = IDD_DLG_REQUEST_WORKLOAD };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton4();

public:
    void SetCurUserName(CString strUserName);
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);

private:
    CWinThreadDatabase *    m_pDbThread;
    CString                 m_curUserName;
};

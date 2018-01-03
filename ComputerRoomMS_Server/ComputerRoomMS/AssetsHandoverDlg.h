#pragma once
#include "WinThreadDatabase.h"

// CAssetsHandoverDlg 对话框

class CAssetsHandoverDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAssetsHandoverDlg)

public:
	CAssetsHandoverDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAssetsHandoverDlg();

	enum { IDD = IDD_DLG_ASSETS_HANDOVER };

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

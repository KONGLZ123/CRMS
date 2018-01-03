#pragma once
#include "WinThreadDatabase.h"

// CRoomFaultDlg 对话框

class CRoomFaultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRoomFaultDlg)

public:
	CRoomFaultDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoomFaultDlg();

	enum { IDD = IDD_DLG_UPLOAD_FAULT };

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

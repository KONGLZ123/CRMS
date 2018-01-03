#pragma once
#include "WinThreadDatabase.h"

// CRequestRoomDlg 对话框

class CRequestRoomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRequestRoomDlg)

public:
	CRequestRoomDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRequestRoomDlg();

	enum { IDD = IDD_DLG_REQUEST_ROOM };

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

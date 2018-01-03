#pragma once
#include "WinThreadDatabase.h"

// CRoomRunningReportDlg 对话框

class CRoomRunningReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRoomRunningReportDlg)

public:
	CRoomRunningReportDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRoomRunningReportDlg();

	enum { IDD = IDD_DLG_ROOM_RUNING_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButton1();

public:
    void SetCurUserName(CString strUserName);
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);

private:
    CWinThreadDatabase *    m_pDbThread;
    CString                 m_curUserName;
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

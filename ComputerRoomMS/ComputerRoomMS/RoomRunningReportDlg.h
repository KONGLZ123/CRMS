#pragma once
#include "WinThreadDatabase.h"

// CRoomRunningReportDlg �Ի���

class CRoomRunningReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRoomRunningReportDlg)

public:
	CRoomRunningReportDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRoomRunningReportDlg();

	enum { IDD = IDD_DLG_ROOM_RUNING_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

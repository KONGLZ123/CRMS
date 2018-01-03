#pragma once
#include "WinThreadDatabase.h"

// CRequestRoomDlg �Ի���

class CRequestRoomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRequestRoomDlg)

public:
	CRequestRoomDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRequestRoomDlg();

	enum { IDD = IDD_DLG_REQUEST_ROOM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    afx_msg void OnBnClickedButton1();
	DECLARE_MESSAGE_MAP()


public:
    void SetCurUserName(CString strUserName);
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);

private:
    CWinThreadDatabase *    m_pDbThread;
    CString                 m_curUserName;
};

#pragma once
#include "WinThreadDatabase.h"

// CRoomFaultDlg �Ի���

class CRoomFaultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRoomFaultDlg)

public:
	CRoomFaultDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRoomFaultDlg();

	enum { IDD = IDD_DLG_UPLOAD_FAULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
    void SetCurUserName(CString strUserName);
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);

private:
    CWinThreadDatabase *    m_pDbThread;
    CString                 m_curUserName;
public:
    afx_msg void OnBnClickedBtnUploadGz();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

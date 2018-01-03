#pragma once
#include "WinThreadDatabase.h"

// CRepairDlg �Ի���

class CRepairDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRepairDlg)

public:
	CRepairDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRepairDlg();

	enum { IDD = IDD_DLG_REPAIR };

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

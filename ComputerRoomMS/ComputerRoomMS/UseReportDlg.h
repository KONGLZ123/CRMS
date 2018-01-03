#pragma once
#include "WinThreadDatabase.h"

// CUseReportDlg �Ի���

class CUseReportDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CUseReportDlg)

public:
	CUseReportDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUseReportDlg();

	enum { IDD = IDD_DLG_USE_REPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
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

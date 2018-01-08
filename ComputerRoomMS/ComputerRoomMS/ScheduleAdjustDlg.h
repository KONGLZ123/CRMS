#pragma once
#include "afxwin.h"
#include "WinThreadDatabase.h"

// CScheduleAdjustDlg 对话框

class CScheduleAdjustDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScheduleAdjustDlg)

public:
	CScheduleAdjustDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScheduleAdjustDlg();

	enum { IDD = IDD_DLG_SCHEDULE_ADJUST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);
    void SetClassName(set<CString> setClassName);

    afx_msg void OnBnClickedBtnVerify();
    virtual BOOL OnInitDialog();

    CWinThreadDatabase      *m_pDbThread;
    CComboBox               m_comboWeek;
    CComboBox               m_comboClass;
    CComboBox               m_weekNum;
    set<CString>            m_setClassName;
    CComboBox m_comboGrage;
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    void SetHwnd(HWND hwnd) { m_scheduleHwnd = hwnd; }
    HWND m_scheduleHwnd;
};

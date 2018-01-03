#pragma once
#include "WinThreadDatabase.h"
#include "afxwin.h"

// CRegisterDlg 对话框

class CRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegisterDlg();


	enum { IDD = IDD_DLG_REGISTER };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedBtnRgRegister();
    virtual BOOL OnInitDialog();

public:
    void SetTitleAndBtn(CString strTitle, CString strBtn);
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);
    void SetHwnd(HWND hwnd);

private:
    CWinThreadDatabase *    m_pDbThread;
    CString                 m_strTitle;
    CString                 m_strBtn;
    HWND                    m_parHwnd;

public:
    CComboBox m_comboRole;
    afx_msg void OnCbnSelchangeComboRgRole();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

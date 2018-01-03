#pragma once
#include "WinThreadDatabase.h"
#include "afxwin.h"

// CRegisterDlg �Ի���

class CRegisterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegisterDlg)

public:
	CRegisterDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegisterDlg();


	enum { IDD = IDD_DLG_REGISTER };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
    afx_msg void OnBnClickedBtnRgRegister();
    virtual BOOL OnInitDialog();

public:
    void SetTitleAndBtn(CString strTitle, CString strBtn);
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);

private:
    CWinThreadDatabase *    m_pDbThread;
    CString                 m_strTitle;
    CString                 m_strBtn;

public:
    CComboBox m_comboRole;
};

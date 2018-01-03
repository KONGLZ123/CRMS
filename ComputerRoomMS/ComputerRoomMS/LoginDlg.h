
// ComputerRoomMSDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "data.h"
#include "WinThreadDatabase.h"
#include "AGLinkCtrl.h"

// CComputerRoomMSDlg �Ի���
class CLoginDlg : public CDialogEx
{
// ����
public:
	CLoginDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DLG_LOGIN };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HCURSOR OnQueryDragIcon();
    
    afx_msg LRESULT OnShowLoginDlg(WPARAM wParam, LPARAM lParam); 
    afx_msg void OnClose();
    afx_msg void OnBnClickedLogin();
    afx_msg void OnStnClickedStaticRegister();
	DECLARE_MESSAGE_MAP()

    
    LRESULT OnLoginSuccess(WPARAM wParam, LPARAM lParam);
    LRESULT OnLoginError(WPARAM wParam, LPARAM lParam);
    //LRESULT OnRecv(WPARAM wParam, LPARAM lParam);

    //BOOL SendMsgToServer(Json::Value root);
    //BOOL SendLoginRequestToServer(CString strAccount, CString strPwd, int role);



private:
    void InitCtrls();
    BOOL CheckEmpty(CString strAccount, CString strPwd);
    //BOOL ConnectServer();
    //void InitDatabaseThread();

    //void PraseJson(char * buf);
    void InitDatabaseThread();

public:
    CWinThreadDatabase          *m_pThreadDatabase;
    CEdit       m_editAccount;
    CEdit       m_editPwd;
    CComboBox   m_comboRole;    
    CFont       m_fontLogo;
    CFont       m_fontRegister;
    CStatic     m_staticLogo;
    CAGLinkCtrl m_linkCtrlRegister;
    CString     m_curStrAccount;

    CSocketClient               m_sock;
    CString                     m_strIP;
    CString                     m_strPort;

    afx_msg void OnStnClickedStaticServerSetup();
    CAGLinkCtrl m_linkCtrlSetup;
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

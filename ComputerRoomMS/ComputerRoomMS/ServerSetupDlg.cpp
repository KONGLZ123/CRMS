// ServerSetupDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ServerSetupDlg.h"
#include "afxdialogex.h"
#include "data.h"

// CServerSetupDlg 对话框

IMPLEMENT_DYNAMIC(CServerSetupDlg, CDialogEx)

CServerSetupDlg::CServerSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SERVER_SETUP, pParent)
{

}

CServerSetupDlg::~CServerSetupDlg()
{
}

void CServerSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CServerSetupDlg, CDialogEx)
    ON_BN_CLICKED(IDOK, &CServerSetupDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CServerSetupDlg 消息处理程序


BOOL CServerSetupDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetBackgroundColor(RGB(255, 255, 255));

    GetDlgItem(IDC_EDIT_IP)->SetWindowText(_T("127.0.0.1"));
    GetDlgItem(IDC_EDIT_PORT)->SetWindowText(_T("6666"));

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
}

void CServerSetupDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}


void CServerSetupDlg::OnBnClickedOk()
{
    SERVER_SETUP *pServerSetup = new SERVER_SETUP;

    GetDlgItem(IDC_EDIT_IP)->GetWindowText(pServerSetup->strIP);
    GetDlgItem(IDC_EDIT_PORT)->GetWindowText(pServerSetup->strPort);

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_SETUP_SERVER, (WPARAM)pServerSetup, 0);
    OnOK();
}


BOOL CServerSetupDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_ESCAPE)    //屏蔽ESC  
            return TRUE;

        if (pMsg->wParam == VK_RETURN)    //屏蔽回车 
            return TRUE;

        if (pMsg->wParam == VK_F1) 	//屏蔽F1帮助键
            return TRUE;
    }

    if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) 	//屏蔽ALT+F4
        return TRUE;

    return CDialogEx::PreTranslateMessage(pMsg);
}

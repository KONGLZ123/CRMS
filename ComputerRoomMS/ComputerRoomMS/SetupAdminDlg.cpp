// SetupAdminDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "SetupAdminDlg.h"
#include "afxdialogex.h"


// CSetupAdminDlg 对话框

IMPLEMENT_DYNAMIC(CSetupAdminDlg, CDialogEx)

CSetupAdminDlg::CSetupAdminDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SETUP_ADMIN, pParent)
{

}

CSetupAdminDlg::~CSetupAdminDlg()
{
}

void CSetupAdminDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSetupAdminDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_ASETUP, &CSetupAdminDlg::OnBnClickedBtnAsetup)
END_MESSAGE_MAP()


// CSetupAdminDlg 消息处理程序


void CSetupAdminDlg::OnBnClickedBtnAsetup()
{
    CString strAccount;
    CString strOldPwd;
    CString strNewPwd1;
    CString strNewPwd2;

    //GetDlgItem(IDC_EDIT_ASETUP_ACCOUNT)->SetWindowText(_T(""));
    //GetDlgItem(IDC_EDIT_ASETUP_OLD_PWD)->SetWindowText(_T(""));
    //GetDlgItem(IDC_EDIT_ASETUP_NEW_PWD1)->SetWindowText(_T(""));
    //GetDlgItem(IDC_EDIT_ASETUP_NEW_PWD2)->SetWindowText(_T(""));
    GetDlgItem(IDC_STATIC_ASETUP)->SetWindowText(_T(""));

    //GetDlgItem(IDC_EDIT_ASETUP_ACCOUNT)->GetWindowText(strAccount);
    GetDlgItem(IDC_EDIT_ASETUP_OLD_PWD)->GetWindowText(strOldPwd);
    GetDlgItem(IDC_EDIT_ASETUP_NEW_PWD1)->GetWindowText(strNewPwd1);
    GetDlgItem(IDC_EDIT_ASETUP_NEW_PWD2)->GetWindowText(strNewPwd2);

    if (strOldPwd.IsEmpty() || strNewPwd1.IsEmpty() || strNewPwd2.IsEmpty())
    {
        ::MessageBox(NULL, _T("编辑框不能为空"), _T("提示"), MB_OK);
        //GetDlgItem(IDC_STATIC_ASETUP)->SetWindowText(_T("编辑框不能为空"));
        return;
    }

    if (strOldPwd != m_adminInfo.pwd)
    {
        GetDlgItem(IDC_STATIC_ASETUP)->SetWindowText(_T("旧密码输入错误"));
        return;
    }

    if (strNewPwd1 != strNewPwd2)
    {
        GetDlgItem(IDC_STATIC_ASETUP)->SetWindowText(_T("两次修改密码输入不一致"));
        return;
    }

    CString *pStrPwd = new CString(strNewPwd1);
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_ADMIN_SETUP_PWD, (WPARAM)pStrPwd, 0);

    ::MessageBox(NULL, _T("修改成功"), _T("提示"), MB_OK);
    //GetDlgItem(IDC_STATIC_ASETUP)->SetWindowText(_T("修改成功！"));
}

void CSetupAdminDlg::SetAdminInfo(SYSADMIN_INFO & sysAdminInfo)
{
    m_adminInfo = sysAdminInfo;
    GetDlgItem(IDC_EDIT_ASETUP_ACCOUNT)->SetWindowText(m_adminInfo.account);
}

void CSetupAdminDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}


BOOL CSetupAdminDlg::PreTranslateMessage(MSG* pMsg)
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

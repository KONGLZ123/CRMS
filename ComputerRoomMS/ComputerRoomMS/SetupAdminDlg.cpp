// SetupAdminDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "SetupAdminDlg.h"
#include "afxdialogex.h"


// CSetupAdminDlg �Ի���

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


// CSetupAdminDlg ��Ϣ�������


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
        ::MessageBox(NULL, _T("�༭����Ϊ��"), _T("��ʾ"), MB_OK);
        //GetDlgItem(IDC_STATIC_ASETUP)->SetWindowText(_T("�༭����Ϊ��"));
        return;
    }

    if (strOldPwd != m_adminInfo.pwd)
    {
        GetDlgItem(IDC_STATIC_ASETUP)->SetWindowText(_T("�������������"));
        return;
    }

    if (strNewPwd1 != strNewPwd2)
    {
        GetDlgItem(IDC_STATIC_ASETUP)->SetWindowText(_T("�����޸��������벻һ��"));
        return;
    }

    CString *pStrPwd = new CString(strNewPwd1);
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_ADMIN_SETUP_PWD, (WPARAM)pStrPwd, 0);

    ::MessageBox(NULL, _T("�޸ĳɹ�"), _T("��ʾ"), MB_OK);
    //GetDlgItem(IDC_STATIC_ASETUP)->SetWindowText(_T("�޸ĳɹ���"));
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
        if (pMsg->wParam == VK_ESCAPE)    //����ESC  
            return TRUE;

        if (pMsg->wParam == VK_RETURN)    //���λس� 
            return TRUE;

        if (pMsg->wParam == VK_F1) 	//����F1������
            return TRUE;
    }

    if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) 	//����ALT+F4
        return TRUE;

    return CDialogEx::PreTranslateMessage(pMsg);
}

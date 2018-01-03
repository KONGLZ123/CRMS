// SetupPwdDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "SetupPwdDlg.h"
#include "afxdialogex.h"


// CSetupPwdDlg �Ի���

IMPLEMENT_DYNAMIC(CSetupPwdDlg, CDialogEx)

CSetupPwdDlg::CSetupPwdDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SETUP, pParent)
{

}

CSetupPwdDlg::~CSetupPwdDlg()
{
}

void CSetupPwdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CSetupPwdDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}

void CSetupPwdDlg::SetRoomManagerInfo(ROOM_MANAGER_INFO & personInfo)
{
    m_managerInfo = personInfo;
    m_role = MANAGER;

    GetDlgItem(IDC_EDIT_PERSON_ACCOUNT)->SetWindowText(m_managerInfo.account);
    GetDlgItem(IDC_EDIT_NAME)->SetWindowText(m_managerInfo.name);
    GetDlgItem(IDC_EDIT_TEL)->SetWindowText(m_managerInfo.tel);
    GetDlgItem(IDC_EDIT_AHTURITY)->SetWindowText(_T("��������Ա"));
}

void CSetupPwdDlg::SetRepairInfo(ROOM_REPAIR_INFO & personInfo)
{
    m_repairInfo = personInfo;
    m_role = REPAIR;

    GetDlgItem(IDC_EDIT_PERSON_ACCOUNT)->SetWindowText(m_repairInfo.account);
    GetDlgItem(IDC_EDIT_NAME)->SetWindowText(m_repairInfo.name);
    GetDlgItem(IDC_EDIT_TEL)->SetWindowText(m_repairInfo.tel);
    GetDlgItem(IDC_EDIT_AHTURITY)->SetWindowText(_T("����ά����Ա"));
}

void CSetupPwdDlg::SetTeacherInfo(TEACHER_INFO & personInfo)
{
    m_teacherInfo = personInfo;
    m_role = TEACHER;

    GetDlgItem(IDC_EDIT_PERSON_ACCOUNT)->SetWindowText(m_teacherInfo.account);
    GetDlgItem(IDC_EDIT_NAME)->SetWindowText(m_teacherInfo.name);
    GetDlgItem(IDC_EDIT_TEL)->SetWindowText(m_teacherInfo.tel);
    GetDlgItem(IDC_EDIT_AHTURITY)->SetWindowText(_T("��ʦ"));
}


BEGIN_MESSAGE_MAP(CSetupPwdDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_PERSON_SETUP, &CSetupPwdDlg::OnBnClickedBtnPersonSetup)
END_MESSAGE_MAP()


// CSetupPwdDlg ��Ϣ�������


void CSetupPwdDlg::OnBnClickedBtnPersonSetup()
{
    CString strAccount;
    CString strOldPwd;
    CString strNewPwd1;
    CString strNewPwd2;

    //GetDlgItem(IDC_EDIT_ASETUP_ACCOUNT)->SetWindowText(_T(""));
    //GetDlgItem(IDC_EDIT_ASETUP_OLD_PWD)->SetWindowText(_T(""));
    //GetDlgItem(IDC_EDIT_ASETUP_NEW_PWD1)->SetWindowText(_T(""));
    //GetDlgItem(IDC_EDIT_ASETUP_NEW_PWD2)->SetWindowText(_T(""));
    GetDlgItem(IDC_STATIC_PERSON_SETUP)->SetWindowText(_T(""));

    //GetDlgItem(IDC_EDIT_ASETUP_ACCOUNT)->GetWindowText(strAccount);
    GetDlgItem(IDC_EDIT_PERSON_OLD_PWD)->GetWindowText(strOldPwd);
    GetDlgItem(IDC_EDIT_PERSON_NEW_PWD1)->GetWindowText(strNewPwd1);
    GetDlgItem(IDC_EDIT_PERSON_NEW_PWD2)->GetWindowText(strNewPwd2);

    if (strOldPwd.IsEmpty() || strNewPwd1.IsEmpty() || strNewPwd2.IsEmpty())
    {
        GetDlgItem(IDC_STATIC_PERSON_SETUP)->SetWindowText(_T("�༭����Ϊ��"));
        return;
    }

    UPDATE_PERSON_PWD *pPersonPwd = new UPDATE_PERSON_PWD;

    switch (m_role)
    {
    case MANAGER:
        pPersonPwd->newPwd = m_managerInfo.pwd;
        pPersonPwd->account = m_managerInfo.account;
        pPersonPwd->authrity = MANAGER;
        break;
    case REPAIR:
        pPersonPwd->newPwd = m_repairInfo.pwd;
        pPersonPwd->account = m_repairInfo.account;
        pPersonPwd->authrity = REPAIR;
        break;
    case TEACHER:
        pPersonPwd->newPwd = m_teacherInfo.pwd;
        pPersonPwd->account = m_teacherInfo.account;
        pPersonPwd->authrity = TEACHER;
        break;
    default:
        break;
    }

    if (strOldPwd != pPersonPwd->newPwd)
    {
        GetDlgItem(IDC_STATIC_PERSON_SETUP)->SetWindowText(_T("�������������"));
        return;
    }

    if (strNewPwd1 != strNewPwd2)
    {
        GetDlgItem(IDC_STATIC_PERSON_SETUP)->SetWindowText(_T("�����޸��������벻һ��"));
        return;
    }

    pPersonPwd->newPwd = strNewPwd1;
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_SETUP_PWD, (WPARAM)pPersonPwd, 0);

    GetDlgItem(IDC_STATIC_PERSON_SETUP)->SetWindowText(_T("�޸ĳɹ���"));
}

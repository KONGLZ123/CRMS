// RegisterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "RegisterDlg.h"
#include "afxdialogex.h"


// CRegisterDlg �Ի���

IMPLEMENT_DYNAMIC(CRegisterDlg, CDialogEx)

CRegisterDlg::CRegisterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_REGISTER, pParent)
{

}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_RG_ROLE, m_comboRole);
}


BEGIN_MESSAGE_MAP(CRegisterDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_RG_REGISTER, &CRegisterDlg::OnBnClickedBtnRgRegister)
END_MESSAGE_MAP()


// CRegisterDlg ��Ϣ�������


BOOL CRegisterDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetBackgroundColor(RGB(255, 255, 255));

    SetWindowText(m_strTitle);
    GetDlgItem(IDC_BTN_RG_REGISTER)->SetWindowText(m_strBtn);

    m_comboRole.InsertString(0, _T("��������Ա"));
    m_comboRole.InsertString(1, _T("����ά����Ա"));
    m_comboRole.InsertString(2, _T("��ʦ"));
    m_comboRole.InsertString(3, _T("ѧ��"));
    m_comboRole.SetCurSel(0);

    return TRUE;
}

void CRegisterDlg::SetTitleAndBtn(CString strTitle, CString strBtn)
{
    m_strTitle = strTitle;
    m_strBtn = strBtn;
}

void CRegisterDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}


void CRegisterDlg::OnBnClickedBtnRgRegister()
{
    CString strAccount;
    CString strPwd1;
    CString strPwd2;
    CString strName;
    CString strTel;

    GetDlgItem(IDC_STATIC_RG)->SetWindowText(_T(""));

    GetDlgItem(IDC_EDIT_RG_ACCOUNT)->GetWindowText(strAccount);
    GetDlgItem(IDC_EDIT_RG_PWD1)->GetWindowText(strPwd1);
    GetDlgItem(IDC_EDIT_RG_PWD2)->GetWindowText(strPwd2);
    GetDlgItem(IDC_EDIT_RG_NAME)->GetWindowText(strName);
    GetDlgItem(IDC_EDIT_RG_TEL)->GetWindowText(strTel);

    if (strAccount.IsEmpty() || strPwd1.IsEmpty() || strPwd2.IsEmpty()
        || strName.IsEmpty() || strTel.IsEmpty())
    {
        GetDlgItem(IDC_STATIC_RG)->SetWindowText(_T("�༭����Ϊ��"));
        return;
    }

    if (strPwd1 != strPwd2)
    {
        GetDlgItem(IDC_STATIC_RG)->SetWindowText(_T("������������벻��ͬ"));
        return;
    }

    ADD_PERSON_DATA *ptagAddPerson = new ADD_PERSON_DATA;
    ptagAddPerson->account = strAccount;
    ptagAddPerson->pwd = strPwd1;
    ptagAddPerson->name = strName;
    ptagAddPerson->tel = strTel;
    ptagAddPerson->authority = m_comboRole.GetCurSel();
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_ADD_PERSON, (WPARAM)ptagAddPerson, (LPARAM)m_hWnd);

    if (_T("����") == m_strBtn)
        GetDlgItem(IDC_STATIC_RG)->SetWindowText(_T("����ɹ�"));
    else
        GetDlgItem(IDC_STATIC_RG)->SetWindowText(_T("��ӳɹ�"));


}

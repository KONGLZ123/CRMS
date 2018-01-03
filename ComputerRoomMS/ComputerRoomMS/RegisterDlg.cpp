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
    ON_CBN_SELCHANGE(IDC_COMBO_RG_ROLE, &CRegisterDlg::OnCbnSelchangeComboRgRole)
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

    //GetDlgItem(IDC_EDIT_RG_ID)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_RG_GRADE)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_RG_MAJOR)->EnableWindow(FALSE);
    GetDlgItem(IDC_EDIT_COURSE)->EnableWindow(FALSE);

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

void CRegisterDlg::SetHwnd(HWND hwnd)
{
    m_parHwnd = hwnd;
}


void CRegisterDlg::OnBnClickedBtnRgRegister()
{
    CString strAccount;
    CString strPwd1;
    CString strPwd2;
    CString strName;
    CString strTel;
    CString strGrade;
    CString strMajor;
    CString strCourse;

    GetDlgItem(IDC_EDIT_RG_ACCOUNT)->GetWindowText(strAccount);
    GetDlgItem(IDC_EDIT_RG_PWD1)->GetWindowText(strPwd1);
    GetDlgItem(IDC_EDIT_RG_PWD2)->GetWindowText(strPwd2);
    GetDlgItem(IDC_EDIT_RG_NAME)->GetWindowText(strName);
    GetDlgItem(IDC_EDIT_RG_TEL)->GetWindowText(strTel);
    
    GetDlgItem(IDC_STATIC_RG)->SetWindowText(_T(""));
    int role = m_comboRole.GetCurSel();
    int authority = 0;
    if (0 == role)
        authority = MANAGER;
    else if (1 == role)
        authority = REPAIR;
    else if (2 == role)
        authority = TEACHER;
    else if (3 == role)
        authority = STUDENT;

    if (STUDENT == authority) {
        GetDlgItem(IDC_EDIT_RG_MAJOR)->GetWindowText(strMajor);
        GetDlgItem(IDC_EDIT_RG_GRADE)->GetWindowText(strGrade);
        GetDlgItem(IDC_EDIT_COURSE)->GetWindowText(strCourse);

        if (strGrade.IsEmpty() ||
            strMajor.IsEmpty() ||
            strPwd1.IsEmpty() ||
            strPwd2.IsEmpty() ||
            strName.IsEmpty() ||
            strTel.IsEmpty() ||
            strCourse.IsEmpty())
        {
            ::MessageBox(NULL, _T("�༭����Ϊ��"), _T("��ʾ"), MB_OK);
            return;
        }
    }
    else if (TEACHER == authority) {
        GetDlgItem(IDC_EDIT_RG_GRADE)->GetWindowText(strGrade);
        GetDlgItem(IDC_EDIT_COURSE)->GetWindowText(strCourse);

        if (strGrade.IsEmpty() ||
            strPwd1.IsEmpty() ||
            strPwd2.IsEmpty() ||
            strName.IsEmpty() ||
            strTel.IsEmpty() ||
            strCourse.IsEmpty())
        {
            ::MessageBox(NULL, _T("�༭����Ϊ��"), _T("��ʾ"), MB_OK);
            return;
        }
    }
    else {
        if (strAccount.IsEmpty() || strPwd1.IsEmpty() || 
            strPwd2.IsEmpty() || 
            strName.IsEmpty() || 
            strTel.IsEmpty())
        {
            ::MessageBox(NULL, _T("�༭����Ϊ��"), _T("��ʾ"), MB_OK);
            return;
        }
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
    ptagAddPerson->authority = authority;

    if (_T("����") == m_strBtn) {
        ptagAddPerson->bIsRegister = TRUE;
    }
    else {
        ptagAddPerson->bIsRegister = FALSE;
    }

    if (STUDENT == authority || TEACHER == authority) {
        ptagAddPerson->grade = strGrade;
        ptagAddPerson->major = strMajor;
        ptagAddPerson->course = strCourse;
    }

    ::SendMessage(m_parHwnd, WM_ADD_LIST_ITEM, (WPARAM)ptagAddPerson, 0);
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_ADD_PERSON, (WPARAM)ptagAddPerson, (LPARAM)m_hWnd);

    if (_T("����") == m_strBtn)
        GetDlgItem(IDC_STATIC_RG)->SetWindowText(_T("����ɹ�"));
    else
        GetDlgItem(IDC_STATIC_RG)->SetWindowText(_T("��ӳɹ�"));
}


void CRegisterDlg::OnCbnSelchangeComboRgRole()
{
    GetDlgItem(IDC_EDIT_RG_ACCOUNT)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_RG_PWD1)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_RG_PWD2)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_RG_NAME)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_RG_TEL)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_RG_MAJOR)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_RG_GRADE)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_COURSE)->SetWindowText(_T(""));

    if (STUDENT == m_comboRole.GetCurSel() + 1 || TEACHER == m_comboRole.GetCurSel() + 1) {
        GetDlgItem(IDC_EDIT_RG_GRADE)->EnableWindow(TRUE);
        GetDlgItem(IDC_EDIT_COURSE)->EnableWindow(TRUE);

        if (STUDENT == m_comboRole.GetCurSel() + 1) {
            GetDlgItem(IDC_STATIC_GRADE)->SetWindowText(_T("�༶��"));
            GetDlgItem(IDC_STATIC_COURSE)->SetWindowText(_T("��ѧ�γ̣�"));
            GetDlgItem(IDC_EDIT_RG_MAJOR)->EnableWindow(TRUE);
        }
        else {
            GetDlgItem(IDC_STATIC_GRADE)->SetWindowText(_T("���ڰ༶��"));
            GetDlgItem(IDC_STATIC_COURSE)->SetWindowText(_T("���ڿγ̣�"));
            GetDlgItem(IDC_EDIT_RG_MAJOR)->EnableWindow(FALSE);
        }
    } else {
        GetDlgItem(IDC_EDIT_COURSE)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_RG_GRADE)->EnableWindow(FALSE);
        GetDlgItem(IDC_EDIT_RG_MAJOR)->EnableWindow(FALSE);
    }
}


BOOL CRegisterDlg::PreTranslateMessage(MSG* pMsg)
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

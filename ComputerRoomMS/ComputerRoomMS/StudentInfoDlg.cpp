// StudentInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "StudentInfoDlg.h"
#include "afxdialogex.h"


// CStudentInfoDlg 对话框

IMPLEMENT_DYNAMIC(CStudentInfoDlg, CDialogEx)

CStudentInfoDlg::CStudentInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_VIEW_STU_INFO, pParent)
{

}

CStudentInfoDlg::~CStudentInfoDlg()
{
}

void CStudentInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CStudentInfoDlg::SetStudentInfo(STUDETN_INFO &studentInfo)
{
    m_studentInfo = studentInfo;

    GetDlgItem(IDC_EDIT_SACCOUNT)->SetWindowText(m_studentInfo.student_id);
    GetDlgItem(IDC_EDIT_SNAME)->SetWindowText(m_studentInfo.name);
    GetDlgItem(IDC_EDIT_SSEX)->SetWindowText(m_studentInfo.tel);
    GetDlgItem(IDC_EDIT_PERSON_ACCOUNT)->SetWindowText(m_studentInfo.student_id);
    GetDlgItem(IDC_EDIT_SCLASS)->SetWindowText(m_studentInfo.classes);
    GetDlgItem(IDC_EDIT_SMAJOR)->SetWindowText(m_studentInfo.major);
}


BEGIN_MESSAGE_MAP(CStudentInfoDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_PERSON_SETUP, &CStudentInfoDlg::OnBnClickedBtnPersonSetup)
END_MESSAGE_MAP()


// CStudentInfoDlg 消息处理程序

void CStudentInfoDlg::OnBnClickedBtnPersonSetup()
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
        ::MessageBox(NULL, _T("编辑框不能为空"), _T("提示"), MB_OK);
        //GetDlgItem(IDC_STATIC_PERSON_SETUP)->SetWindowText(_T("编辑框不能为空"));
        return;
    }

    UPDATE_PERSON_PWD *pPersonPwd = new UPDATE_PERSON_PWD;

    pPersonPwd->newPwd = m_studentInfo.pwd;
    pPersonPwd->account = m_studentInfo.student_id;
    pPersonPwd->authrity = STUDENT;

    if (strOldPwd != pPersonPwd->newPwd)
    {
        GetDlgItem(IDC_STATIC_PERSON_SETUP)->SetWindowText(_T("旧密码输入错误"));
        return;
    }

    if (strNewPwd1 != strNewPwd2)
    {
        GetDlgItem(IDC_STATIC_PERSON_SETUP)->SetWindowText(_T("两次修改密码输入不一致"));
        return;
    }

    pPersonPwd->newPwd = strNewPwd1;
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_SETUP_PWD, (WPARAM)pPersonPwd, 0);

    GetDlgItem(IDC_STATIC_PERSON_SETUP)->SetWindowText(_T("修改成功！"));
}

void CStudentInfoDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}



BOOL CStudentInfoDlg::PreTranslateMessage(MSG* pMsg)
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

// SearchResultDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "SearchResultDlg.h"
#include "afxdialogex.h"


// CSearchResultDlg �Ի���

IMPLEMENT_DYNAMIC(CSearchResultDlg, CDialogEx)

CSearchResultDlg::CSearchResultDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SEARCH_RESULT, pParent)
{

}

CSearchResultDlg::~CSearchResultDlg()
{
}

void CSearchResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSearchResultDlg, CDialogEx)
END_MESSAGE_MAP()


// CSearchResultDlg ��Ϣ�������

void CSearchResultDlg::SetStudentInfo(STUDETN_INFO &studentInfo)
{
    m_stuInfo = studentInfo;
}

BOOL CSearchResultDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    GetDlgItem(IDC_EDIT_PERSON_ACCOUNT1)->SetWindowText(m_stuInfo.student_id);
    GetDlgItem(IDC_EDIT_SACCOUNT1)->SetWindowText(m_stuInfo.student_id);
    GetDlgItem(IDC_EDIT_SNAME1)->SetWindowText(m_stuInfo.name);
    GetDlgItem(IDC_EDIT_SSEX1)->SetWindowText(m_stuInfo.tel);
    GetDlgItem(IDC_EDIT_SCLASS1)->SetWindowText(m_stuInfo.classes);
    GetDlgItem(IDC_EDIT_SMAJOR1)->SetWindowText(m_stuInfo.major);

    return TRUE;
}


BOOL CSearchResultDlg::PreTranslateMessage(MSG* pMsg)
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

// ViewExamResultDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ViewExamResultDlg.h"
#include "afxdialogex.h"


// CViewExamResultDlg �Ի���

IMPLEMENT_DYNAMIC(CViewExamResultDlg, CDialogEx)

CViewExamResultDlg::CViewExamResultDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_VIEW_RESULT, pParent)
{

}

CViewExamResultDlg::~CViewExamResultDlg()
{
}

void CViewExamResultDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_NORMAL_RESULT, m_listInfo);
}


BEGIN_MESSAGE_MAP(CViewExamResultDlg, CDialogEx)
END_MESSAGE_MAP()


// CViewExamResultDlg ��Ϣ�������


BOOL CViewExamResultDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    DWORD dwStyleEx = m_listInfo.GetExtendedStyle();
    m_listInfo.SetExtendedStyle(dwStyleEx | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    CRect rcClient;
    m_listInfo.GetClientRect(&rcClient);

    int width = (rcClient.Width() - 60) / 3;
    m_listInfo.InsertColumn(0, _T("���"), LVCFMT_LEFT, 60);
    m_listInfo.InsertColumn(1, _T("�γ�����"), LVCFMT_LEFT, width);
    m_listInfo.InsertColumn(2, _T("ȱ�ڴ���"), LVCFMT_LEFT, width);
    m_listInfo.InsertColumn(3, _T("ƽ����"), LVCFMT_LEFT, width);

    return TRUE;
}




BOOL CViewExamResultDlg::PreTranslateMessage(MSG* pMsg)
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

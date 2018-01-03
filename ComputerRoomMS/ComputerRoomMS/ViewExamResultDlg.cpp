// ViewExamResultDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ViewExamResultDlg.h"
#include "afxdialogex.h"


// CViewExamResultDlg 对话框

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


// CViewExamResultDlg 消息处理程序


BOOL CViewExamResultDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    DWORD dwStyleEx = m_listInfo.GetExtendedStyle();
    m_listInfo.SetExtendedStyle(dwStyleEx | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    CRect rcClient;
    m_listInfo.GetClientRect(&rcClient);

    int width = (rcClient.Width() - 60) / 3;
    m_listInfo.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 60);
    m_listInfo.InsertColumn(1, _T("课程名称"), LVCFMT_LEFT, width);
    m_listInfo.InsertColumn(2, _T("缺勤次数"), LVCFMT_LEFT, width);
    m_listInfo.InsertColumn(3, _T("平常分"), LVCFMT_LEFT, width);

    return TRUE;
}




BOOL CViewExamResultDlg::PreTranslateMessage(MSG* pMsg)
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

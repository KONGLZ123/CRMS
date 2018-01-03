// WorkloadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "WorkloadDlg.h"
#include "afxdialogex.h"


// CWorkloadDlg 对话框

IMPLEMENT_DYNAMIC(CWorkloadDlg, CDialogEx)

CWorkloadDlg::CWorkloadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_REQUEST_WORKLOAD, pParent)
{

}

CWorkloadDlg::~CWorkloadDlg()
{
}

void CWorkloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWorkloadDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_DECLARATION, &CWorkloadDlg::OnBnClickedBtnDeclaration)
END_MESSAGE_MAP()


// CWorkloadDlg 消息处理程序

void CWorkloadDlg::SetCurUserName(CString strUserName)
{
    m_curUserName = strUserName;
}

void CWorkloadDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}


// 请求津贴
void CWorkloadDlg::OnBnClickedBtnDeclaration()
{
    //CString strRoomNum;
    CString strReason;
    CString strName;
    CString strDate;
    CString strFixSalary;
    CString strWorkloadSalary;

    //GetDlgItem(IDC_EDIT_ROOM_NUM)->GetWindowText(strRoomNum);
    GetDlgItem(IDC_EDIT_REASON)->GetWindowText(strReason);
    GetDlgItem(IDC_EDIT_DNAME)->GetWindowText(strName);
    GetDlgItem(IDC_EDIT_DDATE)->GetWindowText(strDate);
    GetDlgItem(IDC_EDIT_FIX_SALARY)->GetWindowText(strFixSalary);
    GetDlgItem(IDC_EDIT_WORKLOAD_SALARY)->GetWindowText(strWorkloadSalary);

    if ( strReason.IsEmpty() ||
        strName.IsEmpty() ||
        strDate.IsEmpty() ||
        strFixSalary.IsEmpty() ||
        strWorkloadSalary.IsEmpty())
    {
        ::MessageBox(NULL, _T("编辑框不能为空"), _T("提示"), MB_OK);
        return;
    }

    //CTime tm = CTime::GetCurrentTime();
    //CString strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));

    WORKLOAD_REPORT *pReportData = new WORKLOAD_REPORT;
    pReportData->strName = strName;
    pReportData->strReason = strReason;
    pReportData->strDate = strDate;
    pReportData->strFixSalary = strFixSalary;
    pReportData->strWorkloadSalary = strWorkloadSalary;

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_INSERT_WORKLOAD_REPORT, (WPARAM)pReportData, 0);
}


BOOL CWorkloadDlg::PreTranslateMessage(MSG* pMsg)
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

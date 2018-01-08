// RepairDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "RepairDlg.h"
#include "afxdialogex.h"


// CRepairDlg 对话框

IMPLEMENT_DYNAMIC(CRepairDlg, CDialogEx)

CRepairDlg::CRepairDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_REPAIR, pParent)
{

}

CRepairDlg::~CRepairDlg()
{
}

void CRepairDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRepairDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CRepairDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRepairDlg 消息处理程序


void CRepairDlg::OnBnClickedButton1()
{
    CString strRoomNum;
    CString strReason;

    GetDlgItem(IDC_EDIT_ROOM_NUM)->GetWindowText(strRoomNum);
    GetDlgItem(IDC_EDIT_REASON)->GetWindowText(strReason);

    if (strRoomNum.IsEmpty() || strReason.IsEmpty())
    {
        ::MessageBox(NULL, _T("编辑框不能为空"), _T("提示"), MB_OK);
        return;
    }

    CTime tm = CTime::GetCurrentTime();
    CString strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));

    REPORT_DATA *pReportData = new REPORT_DATA;
    pReportData->submitPerson = m_curUserName;
    pReportData->roomId = strRoomNum;
    pReportData->reason = strReason;
    pReportData->uploadDate = strTime;
    pReportData->notes = _T("");
    pReportData->reportType = REPAIR_REPORT;

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_INSERT_REPORT, (WPARAM)pReportData, 0);

    ::MessageBox(NULL, _T("提交成功"), _T("提示"), MB_OK);
}

void CRepairDlg::SetCurUserName(CString strUserName)
{
    m_curUserName = strUserName;
}

void CRepairDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}


BOOL CRepairDlg::PreTranslateMessage(MSG* pMsg)
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

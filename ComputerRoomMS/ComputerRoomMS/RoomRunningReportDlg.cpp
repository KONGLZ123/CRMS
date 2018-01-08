// RoomRunningReportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "RoomRunningReportDlg.h"
#include "afxdialogex.h"


// CRoomRunningReportDlg 对话框

IMPLEMENT_DYNAMIC(CRoomRunningReportDlg, CDialogEx)

CRoomRunningReportDlg::CRoomRunningReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_ROOM_RUNING_REPORT, pParent)
{

}

CRoomRunningReportDlg::~CRoomRunningReportDlg()
{
}

void CRoomRunningReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRoomRunningReportDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CRoomRunningReportDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CRoomRunningReportDlg 消息处理程序


void CRoomRunningReportDlg::OnBnClickedButton1()
{
    CString strRoomNum;
    CString strReason;
    CString strName;
    CString strDate;

    GetDlgItem(IDC_EDIT_ROOM_NUM)->GetWindowText(strRoomNum);
    GetDlgItem(IDC_EDIT_REASON)->GetWindowText(strReason);
    GetDlgItem(IDC_EDIT_NAME_GZ)->GetWindowText(strName);
    GetDlgItem(IDC_EDIT_DATE_GZ)->GetWindowText(strDate);

    if (strRoomNum.IsEmpty() ||
        strReason.IsEmpty() ||
        strName.IsEmpty() ||
        strDate.IsEmpty())
    {
        ::MessageBox(NULL, _T("编辑框不能为空"), _T("提示"), MB_OK);
        return;
    }

    //CTime tm = CTime::GetCurrentTime();
    //CString strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));

    REPORT_DATA *pReportData = new REPORT_DATA;
    pReportData->submitPerson = strName;
    pReportData->roomId = strRoomNum;
    pReportData->reason = strReason;
    pReportData->uploadDate = strDate;
    pReportData->notes = _T("");
    pReportData->reportType = RUNING_REPORT;

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_INSERT_REPORT, (WPARAM)pReportData, 0);

    ::MessageBox(NULL, _T("提交成功"), _T("提示"), MB_OK);
}


void CRoomRunningReportDlg::SetCurUserName(CString strUserName)
{
    m_curUserName = strUserName;
}


void CRoomRunningReportDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}




BOOL CRoomRunningReportDlg::PreTranslateMessage(MSG* pMsg)
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

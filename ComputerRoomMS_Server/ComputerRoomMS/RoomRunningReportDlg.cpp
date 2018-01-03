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
    pReportData->reportType = REQUEST_REPAIR;

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_REQUEST_REPAIR, (WPARAM)pReportData, 0);
}


void CRoomRunningReportDlg::SetCurUserName(CString strUserName)
{
    m_curUserName = strUserName;
}


void CRoomRunningReportDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}



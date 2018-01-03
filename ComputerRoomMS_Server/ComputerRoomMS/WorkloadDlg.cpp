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
    ON_BN_CLICKED(IDC_BUTTON4, &CWorkloadDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CWorkloadDlg 消息处理程序


void CWorkloadDlg::OnBnClickedButton4()
{
    CString strReason;

    GetDlgItem(IDC_EDIT_REASON)->GetWindowText(strReason);

    if (strReason.IsEmpty())
    {
        ::MessageBox(NULL, _T("编辑框不能为空"), _T("提示"), MB_OK);
        return;
    }

    CTime tm = CTime::GetCurrentTime();
    CString strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));

    REPORT_DATA *pReportData = new REPORT_DATA;
    pReportData->submitPerson = m_curUserName;
    pReportData->roomId = _T("");
    pReportData->reason = strReason;
    pReportData->uploadDate = strTime;
    pReportData->notes = _T("");
    pReportData->reportType = REQUEST_REPAIR;

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_REQUEST_REPAIR, (WPARAM)pReportData, 0);
}


void CWorkloadDlg::SetCurUserName(CString strUserName)
{
    m_curUserName = strUserName;
}

void CWorkloadDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}


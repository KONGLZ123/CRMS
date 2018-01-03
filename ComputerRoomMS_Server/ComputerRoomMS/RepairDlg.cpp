// RepairDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "RepairDlg.h"
#include "afxdialogex.h"


// CRepairDlg �Ի���

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


// CRepairDlg ��Ϣ�������


void CRepairDlg::OnBnClickedButton1()
{
    CString strRoomNum;
    CString strReason;

    GetDlgItem(IDC_EDIT_ROOM_NUM)->GetWindowText(strRoomNum);
    GetDlgItem(IDC_EDIT_REASON)->GetWindowText(strReason);

    if (strRoomNum.IsEmpty() || strReason.IsEmpty())
    {
        ::MessageBox(NULL, _T("�༭����Ϊ��"), _T("��ʾ"), MB_OK);
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

void CRepairDlg::SetCurUserName(CString strUserName)
{
    m_curUserName = strUserName;
}

void CRepairDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}

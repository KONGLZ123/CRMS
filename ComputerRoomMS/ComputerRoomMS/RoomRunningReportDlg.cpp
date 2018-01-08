// RoomRunningReportDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "RoomRunningReportDlg.h"
#include "afxdialogex.h"


// CRoomRunningReportDlg �Ի���

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


// CRoomRunningReportDlg ��Ϣ�������


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
        ::MessageBox(NULL, _T("�༭����Ϊ��"), _T("��ʾ"), MB_OK);
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

    ::MessageBox(NULL, _T("�ύ�ɹ�"), _T("��ʾ"), MB_OK);
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

// RoomFaultDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "RoomFaultDlg.h"
#include "afxdialogex.h"


// CRoomFaultDlg �Ի���

IMPLEMENT_DYNAMIC(CRoomFaultDlg, CDialogEx)

CRoomFaultDlg::CRoomFaultDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_UPLOAD_FAULT, pParent)
{

}

CRoomFaultDlg::~CRoomFaultDlg()
{
}

void CRoomFaultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRoomFaultDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_UPLOAD_GZ, &CRoomFaultDlg::OnBnClickedBtnUploadGz)
END_MESSAGE_MAP()


// CRoomFaultDlg ��Ϣ�������

void CRoomFaultDlg::SetCurUserName(CString strUserName)
{
    m_curUserName = strUserName;
}

void CRoomFaultDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}


void CRoomFaultDlg::OnBnClickedBtnUploadGz()
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
    pReportData->reportType = REQUEST_REPAIR;

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_INSERT_REPORT, (WPARAM)pReportData, 0);

    ::MessageBox(NULL, _T("�ύ�ɹ�"), _T("��ʾ"), MB_OK);
}


BOOL CRoomFaultDlg::PreTranslateMessage(MSG* pMsg)
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

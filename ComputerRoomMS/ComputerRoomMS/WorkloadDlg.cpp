// WorkloadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "WorkloadDlg.h"
#include "afxdialogex.h"


// CWorkloadDlg �Ի���

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


// CWorkloadDlg ��Ϣ�������

void CWorkloadDlg::SetCurUserName(CString strUserName)
{
    m_curUserName = strUserName;
}

void CWorkloadDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}


// �������
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
        ::MessageBox(NULL, _T("�༭����Ϊ��"), _T("��ʾ"), MB_OK);
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

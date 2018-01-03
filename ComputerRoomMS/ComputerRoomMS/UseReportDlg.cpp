// UseReportDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "UseReportDlg.h"
#include "afxdialogex.h"


// CUseReportDlg 对话框

IMPLEMENT_DYNAMIC(CUseReportDlg, CDialogEx)

CUseReportDlg::CUseReportDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_USE_REPORT, pParent)
{

}

CUseReportDlg::~CUseReportDlg()
{
}

void CUseReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUseReportDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CUseReportDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CUseReportDlg 消息处理程序


BOOL CUseReportDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    

    return TRUE;
}


void CUseReportDlg::OnBnClickedButton1()
{
    CString strRoomNum;
    CString strReason;
    CString strName;
    CString strDate;

    GetDlgItem(IDC_EDIT_ROOM_NUM)->GetWindowText(strRoomNum);
    GetDlgItem(IDC_EDIT_REASON)->GetWindowText(strReason);
    GetDlgItem(IDC_EDIT_NAME_GZ1)->GetWindowText(strName);
    GetDlgItem(IDC_EDIT_DATE_GZ1)->GetWindowText(strDate);

    if (strRoomNum.IsEmpty() ||
        strReason.IsEmpty() ||
        strName.IsEmpty() ||
        strDate.IsEmpty())
    {
        ::MessageBox(NULL, _T("编辑框不能为空"), _T("提示"), MB_OK);
        return;
    }

    REPORT_DATA *pReportData = new REPORT_DATA;
    pReportData->submitPerson = strName;
    pReportData->roomId = strRoomNum;
    pReportData->reason = strReason;
    pReportData->uploadDate = strDate;
    pReportData->notes = _T("");
    pReportData->reportType = USE_REPORT;

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_INSERT_REPORT, (WPARAM)pReportData, 0);
}


void CUseReportDlg::SetCurUserName(CString strUserName)
{
    m_curUserName = strUserName;
}

void CUseReportDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}



BOOL CUseReportDlg::PreTranslateMessage(MSG* pMsg)
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

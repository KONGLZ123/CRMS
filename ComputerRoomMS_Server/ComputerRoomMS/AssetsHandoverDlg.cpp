// AssetsHandoverDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "AssetsHandoverDlg.h"
#include "afxdialogex.h"


// CAssetsHandoverDlg �Ի���

IMPLEMENT_DYNAMIC(CAssetsHandoverDlg, CDialogEx)

CAssetsHandoverDlg::CAssetsHandoverDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_ASSETS_HANDOVER, pParent)
{

}

CAssetsHandoverDlg::~CAssetsHandoverDlg()
{
}

void CAssetsHandoverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAssetsHandoverDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON4, &CAssetsHandoverDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CAssetsHandoverDlg ��Ϣ�������


void CAssetsHandoverDlg::OnBnClickedButton4()
{
    CString strReason;

    GetDlgItem(IDC_EDIT_REASON)->GetWindowText(strReason);

    if (strReason.IsEmpty())
    {
        ::MessageBox(NULL, _T("�༭����Ϊ��"), _T("��ʾ"), MB_OK);
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


void CAssetsHandoverDlg::SetCurUserName(CString strUserName)
{
    m_curUserName = strUserName;
}


void CAssetsHandoverDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}


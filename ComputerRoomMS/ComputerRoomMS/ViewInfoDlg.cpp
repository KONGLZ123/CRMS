// ViewInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ViewInfoDlg.h"
#include "afxdialogex.h"


// CViewInfoDlg 对话框

IMPLEMENT_DYNAMIC(CViewInfoDlg, CDialogEx)

CViewInfoDlg::CViewInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_VIEW_INFO, pParent)
{

}

CViewInfoDlg::~CViewInfoDlg()
{
}

void CViewInfoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_PERSON_INFO, m_listChectReport);
    DDX_Control(pDX, IDC_COMBO_REPORT_STYLE, m_comboReportStyle);
}

void CViewInfoDlg::SetReportData(vector<REPORT_DATA> & reportData)
{
    m_reportData = reportData;

    int size = m_reportData.size();
    int j = 0;
    for (int i = 0; i < size; i++)
    {
        REPORT_DATA reportData = m_reportData.at(i);
        if (0 == reportData.reportType)
        {
            CString strId;
            strId.Format(_T("%d"), j + 1);
            m_listChectReport.InsertItem(j, strId);
            m_listChectReport.SetItemText(j, 1, reportData.submitPerson);
            m_listChectReport.SetItemText(j, 2, reportData.uploadDate);
            m_listChectReport.SetItemText(j, 3, reportData.reason);
            m_listChectReport.SetItemText(j, 4, reportData.notes);
            j++;
        }
    }
}

void CViewInfoDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
    m_listChectReport.SetThreadDatabase(m_pDbThread);
}

LRESULT CViewInfoDlg::OnUpdateList(WPARAM wParam, LPARAM lParam)
{
    REPORT_DATA *pReportData = reinterpret_cast<REPORT_DATA *>(wParam);

    vector<REPORT_DATA>::iterator it;
    for (it = m_reportData.begin(); it != m_reportData.end(); ++it) {
        if (pReportData->submitPerson == it->submitPerson &&
            pReportData->uploadDate == it->uploadDate) {
            it->isView = pReportData->isView;
            break;
        }
    }

    //delete pReportData;
    //pReportData = NULL;

    return 1;
}

void CViewInfoDlg::InitCtrl()
{
    m_comboReportStyle.InsertString(RUNING_REPORT, _T("机房运行报告"));
    m_comboReportStyle.InsertString(USE_REPORT, _T("机房使用报告"));
    m_comboReportStyle.InsertString(REPAIR_REPORT, _T("机房维护报告"));
    m_comboReportStyle.InsertString(REQUEST_REPORT, _T("机房申请报告"));
    m_comboReportStyle.SetCurSel(0);

    DWORD dwStyleEx = m_listChectReport.GetExtendedStyle();
    m_listChectReport.SetExtendedStyle(dwStyleEx | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    CRect rcClient;
    m_listChectReport.GetClientRect(&rcClient);

    m_listChectReport.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 45);
    m_listChectReport.InsertColumn(1, _T("提交者"), LVCFMT_LEFT, 60);
    m_listChectReport.InsertColumn(2, _T("日期"), LVCFMT_LEFT, 120); // 225
    m_listChectReport.InsertColumn(3, _T("状况"), LVCFMT_LEFT, rcClient.Width() - 225);
    //m_listChectReport.InsertColumn(4, _T("特殊情况说明"), LVCFMT_LEFT, rcClient.Width() - 375);
    m_listChectReport.InsertColumn(4, _T("同意"), LVCFMT_LEFT, 0);
}


BEGIN_MESSAGE_MAP(CViewInfoDlg, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_REPORT_STYLE, &CViewInfoDlg::OnCbnSelchangeComboReportStyle)
    ON_NOTIFY(NM_CLICK, IDC_LIST_PERSON_INFO, &CViewInfoDlg::OnNMClickListPersonInfo)
    ON_MESSAGE(WM_UPDATE_ROOM_LIST, OnUpdateList)
END_MESSAGE_MAP()


// CViewInfoDlg 消息处理程序


BOOL CViewInfoDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    InitCtrl();

    return TRUE;
}


void CViewInfoDlg::OnCbnSelchangeComboReportStyle()
{
    CRect rcClient;
    m_listChectReport.GetClientRect(&rcClient);
    m_listChectReport.DeleteAllItems();

    int selType = m_comboReportStyle.GetCurSel();

    if (3 == selType) {
        m_listChectReport.SetStyle(LIST_STYLE_ROOM_REQUEST);

        //m_listChectReport.SetColumnWidth(4, rcClient.Width() - 425);
        m_listChectReport.SetColumnWidth(3, rcClient.Width() - 275);
        m_listChectReport.SetColumnWidth(4, 50);
    }
    else {
        m_listChectReport.SetStyle(LIST_STYLE_NORMAL);
        //m_listChectReport.SetColumnWidth(4, rcClient.Width() - 375);
        m_listChectReport.SetColumnWidth(3, rcClient.Width() - 225);
        m_listChectReport.SetColumnWidth(4, 0);
    }

    int size = m_reportData.size();
    int j = 0;
    for (int i = 0; i < size; i++) {
        REPORT_DATA reportData = m_reportData.at(i);
        if (selType == reportData.reportType) {
            CString strId;
            strId.Format(_T("%d"), j + 1);
            m_listChectReport.InsertItem(j, strId);
            m_listChectReport.SetItemText(j, 1, reportData.submitPerson);
            m_listChectReport.SetItemText(j, 2, reportData.uploadDate);
            m_listChectReport.SetItemText(j, 3, reportData.reason);
            //m_listChectReport.SetItemText(j, 4, reportData.notes);
            if (reportData.isView)
                m_listChectReport.SetItemText(j, 4, _T("是"));
            else
                m_listChectReport.SetItemText(j, 4, _T("否"));
            j++;
        }
    }
}


void CViewInfoDlg::OnNMClickListPersonInfo(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}


BOOL CViewInfoDlg::PreTranslateMessage(MSG* pMsg)
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

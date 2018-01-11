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
    //DDX_Control(pDX, IDC_LIST_PERSON_INFO2, m_listRepairReport);
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
            //m_listChectReport.SetItemText(j, 4, reportData.notes);
            j++;
        }
    }
}

void CViewInfoDlg::InitCtrl()
{
    m_comboReportStyle.InsertString(0, _T("机房运行报告"));
    m_comboReportStyle.InsertString(1, _T("机房使用报告"));
    m_comboReportStyle.InsertString(2, _T("机房维护报告"));
    m_comboReportStyle.InsertString(3, _T("机房申请报告"));
    m_comboReportStyle.SetCurSel(0);

    DWORD dwStyleEx = m_listChectReport.GetExtendedStyle();
    m_listChectReport.SetExtendedStyle(dwStyleEx | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    CRect rcClient;
    m_listChectReport.GetClientRect(&rcClient);

    m_listChectReport.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 45);
    m_listChectReport.InsertColumn(1, _T("提交者"), LVCFMT_LEFT, 60);
    m_listChectReport.InsertColumn(2, _T("日期"), LVCFMT_LEFT, 60);
    m_listChectReport.InsertColumn(3, _T("说明"), LVCFMT_LEFT, rcClient.Width() - 165);
    //m_listChectReport.InsertColumn(4, _T("特殊情况说明"), LVCFMT_LEFT, rcClient.Width() - 315);
}


BEGIN_MESSAGE_MAP(CViewInfoDlg, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_REPORT_STYLE, &CViewInfoDlg::OnCbnSelchangeComboReportStyle)
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
    m_listChectReport.DeleteAllItems();

    int selType = m_comboReportStyle.GetCurSel();

    int size = m_reportData.size();
    int j = 0;
    for (int i = 0; i < size; i++)
    {
        REPORT_DATA reportData = m_reportData.at(i);
        if (selType == reportData.reportType)
        {
            CString strId;
            strId.Format(_T("%d"), j + 1);
            m_listChectReport.InsertItem(j, strId);
            m_listChectReport.SetItemText(j, 1, reportData.submitPerson);
            m_listChectReport.SetItemText(j, 2, reportData.uploadDate);
            m_listChectReport.SetItemText(j, 3, reportData.reason);
            //m_listChectReport.SetItemText(j, 4, reportData.notes);
            j++;
        }
    }
}

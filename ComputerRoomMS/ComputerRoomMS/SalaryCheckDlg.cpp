// SalaryCheckDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "SalaryCheckDlg.h"
#include "afxdialogex.h"
#include "data.h"

// CSalaryCheckDlg 对话框

IMPLEMENT_DYNAMIC(CSalaryCheckDlg, CDialogEx)

CSalaryCheckDlg::CSalaryCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SALARY_CHECK, pParent)
{

}

CSalaryCheckDlg::~CSalaryCheckDlg()
{
}

void CSalaryCheckDlg::SetStyle(LIST_STYLE style)
{
    m_style = style;
}

void CSalaryCheckDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_SALARY_CHECK, m_listSalaryCheck);
}


BEGIN_MESSAGE_MAP(CSalaryCheckDlg, CDialogEx)
    ON_NOTIFY(NM_CLICK, IDC_LIST_SALARY_CHECK, &CSalaryCheckDlg::OnNMClickListSalaryCheck)
    ON_MESSAGE(WM_UPDATE_SALARY_LIST_ITEM, OnUpdateSalaryListItem)
END_MESSAGE_MAP()


// CSalaryCheckDlg 消息处理程序


void CSalaryCheckDlg::SetSalaryInfo(vector<WORKLOAD_REPORT>& vecSalaryInfo)
{
    m_vecSalaryInfo = vecSalaryInfo;

    int size = m_vecSalaryInfo.size();
    for (int i = 0; i < size; i++)
    {
        WORKLOAD_REPORT salaryInfo = m_vecSalaryInfo.at(i);
        CString strId;
        strId.Format(_T("%d"), i + 1);
        m_listSalaryCheck.InsertItem(i, strId);
        m_listSalaryCheck.SetItemText(i, 1, salaryInfo.strName);
        m_listSalaryCheck.SetItemText(i, 2, salaryInfo.strDate);
        m_listSalaryCheck.SetItemText(i, 3, salaryInfo.strFixSalary);
        m_listSalaryCheck.SetItemText(i, 4, salaryInfo.strWorkloadSalary);
        m_listSalaryCheck.SetItemText(i, 5, salaryInfo.strReason);
    }
}

BOOL CSalaryCheckDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_listSalaryCheck.SetStyle(LIST_STYLE_SALARY);

    DWORD ExStyle = m_listSalaryCheck.GetExtendedStyle();
    m_listSalaryCheck.SetExtendedStyle(ExStyle | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);

    CRect rcClient;
    m_listSalaryCheck.GetClientRect(&rcClient);
    int columnWidth = (rcClient.Width() - 100 - 80 - 120 - 100);
    m_listSalaryCheck.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);
    m_listSalaryCheck.InsertColumn(1, _T("姓名"), LVCFMT_LEFT, 60);
    m_listSalaryCheck.InsertColumn(2, _T("申报时间"), LVCFMT_LEFT, 80);
    m_listSalaryCheck.InsertColumn(3, _T("固定津贴"), LVCFMT_LEFT, 60);
    m_listSalaryCheck.InsertColumn(4, _T("申报津贴"), LVCFMT_LEFT, 60);
    m_listSalaryCheck.InsertColumn(5, _T("申报原由"), LVCFMT_LEFT, columnWidth);
    m_listSalaryCheck.InsertColumn(6, _T("津贴核算"), LVCFMT_LEFT, 60);
    m_listSalaryCheck.InsertColumn(7, _T("批准"), LVCFMT_LEFT, 40);

    return TRUE;
}


void CSalaryCheckDlg::OnNMClickListSalaryCheck(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}

void CSalaryCheckDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
    m_listSalaryCheck.SetThreadDatabase(pDbThread);
}


BOOL CSalaryCheckDlg::PreTranslateMessage(MSG* pMsg)
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

LRESULT CSalaryCheckDlg::OnUpdateSalaryListItem(WPARAM wParam, LPARAM lParam)
{
    WORKLOAD_REPORT *pWorkload = reinterpret_cast<WORKLOAD_REPORT *>(wParam);

    vector<WORKLOAD_REPORT>::iterator it;
    for (it = m_vecSalaryInfo.begin(); it != m_vecSalaryInfo.end(); ++it) {
        if (it->strName == pWorkload->strName &&
            it->strDate == pWorkload->strDate) {
            it->strWorkloadSalary = pWorkload->strWorkloadSalary;
            it->isView = pWorkload->isView;
            break;
        }
    }

    return 0;
}

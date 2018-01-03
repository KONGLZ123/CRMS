// ViewRepairDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ViewRepairDlg.h"
#include "afxdialogex.h"


// CViewRepairDlg 对话框

IMPLEMENT_DYNAMIC(CViewRepairDlg, CDialogEx)

CViewRepairDlg::CViewRepairDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_VIEW_REPAIR, pParent)
{

}

CViewRepairDlg::~CViewRepairDlg()
{
}

void CViewRepairDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewRepairDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_PAGEUP, &CViewRepairDlg::OnBnClickedBtnPageup)
    ON_BN_CLICKED(IDC_BTN_PAGEDOW, &CViewRepairDlg::OnBnClickedBtnPagedow)
END_MESSAGE_MAP()


// CViewRepairDlg 消息处理程序


void CViewRepairDlg::OnBnClickedBtnPageup()
{
    if (0 == m_pageIndex)
    {
        ::MessageBox(this->m_hWnd, _T("已是最前页"), _T("提示"), MB_OK);
        return;
    }

    m_pageIndex--;
    m_repairData = m_vecRepairData.at(m_pageIndex);
    GetDlgItem(IDC_EDIT_OLD_ANNOUNCE)->SetWindowText(m_repairData.reason);
}


void CViewRepairDlg::OnBnClickedBtnPagedow()
{
    int size = m_vecRepairData.size();
    if (m_pageIndex >= size - 1)
    {
        ::MessageBox(this->m_hWnd, _T("已是最末页"), _T("提示"), MB_OK);
        return;
    }
    m_pageIndex++;
    m_repairData = m_vecRepairData.at(m_pageIndex);
    GetDlgItem(IDC_EDIT_OLD_ANNOUNCE)->SetWindowText(m_repairData.reason);
}

void CViewRepairDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}

void CViewRepairDlg::SeRepairData(vector<REPORT_DATA>& vecRepairData)
{
    m_vecRepairData = vecRepairData;

    int size = vecRepairData.size();
    if (0 == size)
    {
        m_pageIndex = 0;
        return;
    }

    m_repairData = vecRepairData.at(size - 1);
    m_pageIndex = size - 1;

    GetDlgItem(IDC_EDIT_OLD_ANNOUNCE)->SetWindowText(m_repairData.reason);
}


BOOL CViewRepairDlg::PreTranslateMessage(MSG* pMsg)
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

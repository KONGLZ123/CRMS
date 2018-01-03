// ViewRepairDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ViewRepairDlg.h"
#include "afxdialogex.h"


// CViewRepairDlg �Ի���

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


// CViewRepairDlg ��Ϣ�������


void CViewRepairDlg::OnBnClickedBtnPageup()
{
    if (0 == m_pageIndex)
    {
        ::MessageBox(this->m_hWnd, _T("������ǰҳ"), _T("��ʾ"), MB_OK);
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
        ::MessageBox(this->m_hWnd, _T("������ĩҳ"), _T("��ʾ"), MB_OK);
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

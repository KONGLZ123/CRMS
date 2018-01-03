// ViewAnnounceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ViewAnnounceDlg.h"
#include "afxdialogex.h"


// CViewAnnounceDlg �Ի���

IMPLEMENT_DYNAMIC(CViewAnnounceDlg, CDialogEx)

CViewAnnounceDlg::CViewAnnounceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_VIEW_ANNOUNCE, pParent)
{

}

CViewAnnounceDlg::~CViewAnnounceDlg()
{
}

void CViewAnnounceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CViewAnnounceDlg::SetAnnounceData(vector<ANNOUNCE_DATA>& vecAnnounceData)
{
    m_vecAnnounceData = vecAnnounceData;

    int size = vecAnnounceData.size();
    if (0 == size)
    {
        m_pageIndex = 0;
        return;
    }

    m_announceData = vecAnnounceData.at(size - 1);
    m_pageIndex = size - 1;

    GetDlgItem(IDC_EDIT_PERSON_ANNOUNCE)->SetWindowText(m_announceData.strText);
}


BEGIN_MESSAGE_MAP(CViewAnnounceDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_PAGE1, &CViewAnnounceDlg::OnBnClickedBtnPage1)
    ON_BN_CLICKED(IDC_BTN_PAGE2, &CViewAnnounceDlg::OnBnClickedBtnPage2)
END_MESSAGE_MAP()


// CViewAnnounceDlg ��Ϣ�������


void CViewAnnounceDlg::OnBnClickedBtnPage1()
{
    if (0 == m_pageIndex)
    {
        ::MessageBox(this->m_hWnd, _T("������ǰҳ"), _T("��ʾ"), MB_OK);
        return;
    }

    m_pageIndex--;
    m_announceData = m_vecAnnounceData.at(m_pageIndex);
    GetDlgItem(IDC_EDIT_PERSON_ANNOUNCE)->SetWindowText(m_announceData.strText);

}


void CViewAnnounceDlg::OnBnClickedBtnPage2()
{
    int size = m_vecAnnounceData.size();
    if (m_pageIndex >= size - 1)
    {
        ::MessageBox(this->m_hWnd, _T("������ĩҳ"), _T("��ʾ"), MB_OK);
        return;
    }
    m_pageIndex++;
    m_announceData = m_vecAnnounceData.at(m_pageIndex);
    GetDlgItem(IDC_EDIT_PERSON_ANNOUNCE)->SetWindowText(m_announceData.strText);
}


BOOL CViewAnnounceDlg::PreTranslateMessage(MSG* pMsg)
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

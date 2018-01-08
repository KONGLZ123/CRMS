// AnnounceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "AnnounceDlg.h"
#include "afxdialogex.h"


// CAnnounceDlg 对话框

IMPLEMENT_DYNAMIC(CAnnounceDlg, CDialogEx)

CAnnounceDlg::CAnnounceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_ANNOUNCE, pParent)
{

}

CAnnounceDlg::~CAnnounceDlg()
{
}

void CAnnounceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void CAnnounceDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}

void CAnnounceDlg::SetAnnounceData(vector<ANNOUNCE_DATA>& vecAnnounceData)
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

    GetDlgItem(IDC_EDIT_OLD_ANNOUNCE)->SetWindowText(m_announceData.strText);
}


BEGIN_MESSAGE_MAP(CAnnounceDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_ANNOUNCE, &CAnnounceDlg::OnBnClickedBtnAnnounce)
    ON_BN_CLICKED(IDC_BTN_DEL_ANNOUNCE, &CAnnounceDlg::OnBnClickedBtnDelAnnounce)
    ON_BN_CLICKED(IDC_BTN_PAGEUP, &CAnnounceDlg::OnBnClickedBtnPageup)
    ON_BN_CLICKED(IDC_BTN_PAGEDOW, &CAnnounceDlg::OnBnClickedBtnPagedow)
END_MESSAGE_MAP()


// CAnnounceDlg 消息处理程序


void CAnnounceDlg::OnBnClickedBtnAnnounce()
{
    CString strText;
    GetDlgItem(IDC_EDIT_NEW_ANNOUNCE)->GetWindowText(strText);

    if (strText.IsEmpty())
    {
        ::MessageBox(this->m_hWnd, _T("发布内容不能为空"), _T("提示"), MB_OK);
        return;
    }

    int type = 0;

    if (IsDlgButtonChecked(IDC_CHECK_MANAGER))
        type |= 0x0001;

    if (IsDlgButtonChecked(IDC_CHECK_TEACHER))
        type |= 0x0010;

    if (IsDlgButtonChecked(IDC_CHECK_STUDENT))
        type |= 0x0100;

    //if (IsDlgButtonChecked(IDC_CHECK_ALL))
    //    type |= 0x0111;

    CTime time = CTime::GetCurrentTime();
    CString strTime = time.Format(_T("%Y-%m-%d %H:%M:%S"));
    ANNOUNCE_DATA *pAnnouce = new ANNOUNCE_DATA;
    pAnnouce->strText = _T("发布日期") + strTime + _T("\r\n\r\n") + strText;
    pAnnouce->type = type;
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_DECLARE_ANNOUNCE, (WPARAM)pAnnouce, (LPARAM)m_hWnd);
}


void CAnnounceDlg::OnBnClickedBtnDelAnnounce()
{
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_DEL_ANNOUNCE, (WPARAM)m_pageIndex + 1, (LPARAM)m_hWnd);

    vector<ANNOUNCE_DATA>::iterator ite;
    for (ite = m_vecAnnounceData.begin(); ite != m_vecAnnounceData.end(); ++ite) {
        if (ite->announce_id == m_pageIndex + 1)
        {
            //m_vecAnnounceData.erase(ite);
            ite->strText = _T("公告已删除");
            GetDlgItem(IDC_EDIT_OLD_ANNOUNCE)->SetWindowText(_T("公告已删除"));
        }
    }
}


void CAnnounceDlg::OnBnClickedBtnPageup ()
{
    if (0 == m_pageIndex)
    {
        ::MessageBox(this->m_hWnd, _T("已是最前页"), _T("提示"), MB_OK);
        return;
    }

    m_pageIndex--;
    m_announceData = m_vecAnnounceData.at(m_pageIndex);
    GetDlgItem(IDC_EDIT_OLD_ANNOUNCE)->SetWindowText(m_announceData.strText);
    
}


void CAnnounceDlg::OnBnClickedBtnPagedow()
{
    int size = m_vecAnnounceData.size();
    if (m_pageIndex >= size - 1)
    {
        ::MessageBox(this->m_hWnd, _T("已是最末页"), _T("提示"), MB_OK);
        return;
    }
    m_pageIndex++;
    m_announceData = m_vecAnnounceData.at(m_pageIndex);
    GetDlgItem(IDC_EDIT_OLD_ANNOUNCE)->SetWindowText(m_announceData.strText);
}


BOOL CAnnounceDlg::PreTranslateMessage(MSG* pMsg)
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

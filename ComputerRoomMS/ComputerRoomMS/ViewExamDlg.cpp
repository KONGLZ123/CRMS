// ViewExamDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ViewExamDlg.h"
#include "afxdialogex.h"


// CViewExamDlg �Ի���

IMPLEMENT_DYNAMIC(CViewExamDlg, CDialogEx)

CViewExamDlg::CViewExamDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_VIEW_EXAM, pParent)
{
    m_vecExamInfo.clear();
}

CViewExamDlg::~CViewExamDlg()
{
}

void CViewExamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


void CViewExamDlg::SetExamInfo(vector<EXAM_INFO>& vecExamInfo)
{
    vector<EXAM_INFO>::iterator it;
    //int i = 0;
    EXAM_INFO tmpExamInfo;
    for (it = vecExamInfo.begin(); it != vecExamInfo.end(); ++it)
    {
        if (it->strClass == m_curStuClass)
        {
            tmpExamInfo.strClass = it->strClass;
            tmpExamInfo.strDate = it->strDate;
            tmpExamInfo.strPerson = it->strPerson;
            tmpExamInfo.strText = it->strText;
            m_vecExamInfo.push_back(tmpExamInfo);
        }
    }

    int size = m_vecExamInfo.size();
    if (0 == size)
    {
        m_pageIndex = 0;
        return;
    }

    m_examInfo = m_vecExamInfo.at(size - 1);
    m_pageIndex = size - 1;

    GetDlgItem(IDC_EDIT_EXAM_INFO)->SetWindowText(m_examInfo.strText);
}

void CViewExamDlg::SetCurStuClass(CString strClass)
{
    m_curStuClass = strClass;
}


BEGIN_MESSAGE_MAP(CViewExamDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON1, &CViewExamDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CViewExamDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CViewExamDlg ��Ϣ�������


void CViewExamDlg::OnBnClickedButton1()
{
    if (0 == m_pageIndex)
    {
        ::MessageBox(this->m_hWnd, _T("������ǰҳ"), _T("��ʾ"), MB_OK);
        return;
    }

    m_pageIndex--;
    m_examInfo = m_vecExamInfo.at(m_pageIndex);
    GetDlgItem(IDC_EDIT_PERSON_ANNOUNCE)->SetWindowText(m_examInfo.strText);
}


void CViewExamDlg::OnBnClickedButton2()
{
    int size = m_vecExamInfo.size();
    if (m_pageIndex >= size - 1)
    {
        ::MessageBox(this->m_hWnd, _T("������ĩҳ"), _T("��ʾ"), MB_OK);
        return;
    }
    m_pageIndex++;
    m_examInfo = m_vecExamInfo.at(m_pageIndex);
    GetDlgItem(IDC_EDIT_PERSON_ANNOUNCE)->SetWindowText(m_examInfo.strText);
}


BOOL CViewExamDlg::PreTranslateMessage(MSG* pMsg)
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

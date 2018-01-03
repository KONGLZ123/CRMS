// AddPersonDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "AddPersonDlg.h"
#include "afxdialogex.h"


// CAddPersonDlg �Ի���

IMPLEMENT_DYNAMIC(CAddPersonDlg, CDialogEx)

CAddPersonDlg::CAddPersonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_ADD_PERSON, pParent)
{

}

CAddPersonDlg::~CAddPersonDlg()
{
}

void CAddPersonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAddPersonDlg, CDialogEx)
END_MESSAGE_MAP()


// CAddPersonDlg ��Ϣ�������


BOOL CAddPersonDlg::PreTranslateMessage(MSG* pMsg)
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

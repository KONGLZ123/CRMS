// AddPersonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "AddPersonDlg.h"
#include "afxdialogex.h"


// CAddPersonDlg 对话框

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


// CAddPersonDlg 消息处理程序


BOOL CAddPersonDlg::PreTranslateMessage(MSG* pMsg)
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

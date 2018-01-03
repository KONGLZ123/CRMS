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

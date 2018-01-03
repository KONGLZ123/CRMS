// StudentManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "StudentManageDlg.h"
#include "afxdialogex.h"


// CStudentManageDlg 对话框

IMPLEMENT_DYNAMIC(CStudentManageDlg, CDialogEx)

CStudentManageDlg::CStudentManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_STUDENT_MANAGE, pParent)
{

}

CStudentManageDlg::~CStudentManageDlg()
{
}

void CStudentManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStudentManageDlg, CDialogEx)
END_MESSAGE_MAP()


// CStudentManageDlg 消息处理程序

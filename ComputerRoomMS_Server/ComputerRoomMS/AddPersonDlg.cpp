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

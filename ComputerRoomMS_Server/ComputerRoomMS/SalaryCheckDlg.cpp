// SalaryCheckDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "SalaryCheckDlg.h"
#include "afxdialogex.h"


// CSalaryCheckDlg �Ի���

IMPLEMENT_DYNAMIC(CSalaryCheckDlg, CDialogEx)

CSalaryCheckDlg::CSalaryCheckDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SALARY_CHECK, pParent)
{

}

CSalaryCheckDlg::~CSalaryCheckDlg()
{
}

void CSalaryCheckDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_SALARY_CHECK, m_listSalaryCheck);
}


BEGIN_MESSAGE_MAP(CSalaryCheckDlg, CDialogEx)
END_MESSAGE_MAP()


// CSalaryCheckDlg ��Ϣ�������


BOOL CSalaryCheckDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    DWORD ExStyle = m_listSalaryCheck.GetExtendedStyle();
    m_listSalaryCheck.SetExtendedStyle(ExStyle | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);

    CRect rcClient;
    m_listSalaryCheck.GetClientRect(&rcClient);
    int columnWidth = rcClient.Width() / 5;
    m_listSalaryCheck.InsertColumn(0, _T("���"), LVCFMT_LEFT, columnWidth);
    m_listSalaryCheck.InsertColumn(1, _T("����"), LVCFMT_LEFT, columnWidth);
    m_listSalaryCheck.InsertColumn(2, _T("�̶�����"), LVCFMT_LEFT, columnWidth);
    m_listSalaryCheck.InsertColumn(3, _T("�걨����"), LVCFMT_LEFT, columnWidth);
    m_listSalaryCheck.InsertColumn(4, _T("��������"), LVCFMT_LEFT, columnWidth);

    return TRUE;
}

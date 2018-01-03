// RoomManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "RoomManageDlg.h"
#include "afxdialogex.h"


// CAssetsListDlg 对话框

IMPLEMENT_DYNAMIC(CAssetsListDlg, CDialogEx)

CAssetsListDlg::CAssetsListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_ASSETS_LIST, pParent)
{

}

CAssetsListDlg::~CAssetsListDlg()
{
}

void CAssetsListDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_ASSERT_LIST, m_listRoomAssets);
}


BEGIN_MESSAGE_MAP(CAssetsListDlg, CDialogEx)
END_MESSAGE_MAP()


// CAssetsListDlg 消息处理程序


BOOL CAssetsListDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();


    DWORD ExStyle = m_listRoomAssets.GetExtendedStyle();
    m_listRoomAssets.SetExtendedStyle(ExStyle | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);

    CRect rcClient;
    m_listRoomAssets.GetClientRect(&rcClient);
    int columnWidth = rcClient.Width() / 7;
    m_listRoomAssets.InsertColumn(0, _T("序号"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(1, _T("设备名称"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(2, _T("持有者"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(3, _T("所属机房"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(4, _T("入库时间"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(5, _T("出库时间"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(6, _T("现状"), LVCFMT_LEFT, columnWidth);

    return TRUE;
}

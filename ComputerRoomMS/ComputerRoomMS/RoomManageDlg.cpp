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
    ON_BN_CLICKED(IDC_BUTTON4, &CAssetsListDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CAssetsListDlg 消息处理程序


BOOL CAssetsListDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_listRoomAssets.SetStyle(LIST_STYLE_ASSERT_LIST);

    DWORD ExStyle = m_listRoomAssets.GetExtendedStyle();
    m_listRoomAssets.SetExtendedStyle(ExStyle | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);

    CRect rcClient;
    m_listRoomAssets.GetClientRect(&rcClient);
    int columnWidth = rcClient.Width() / 6;
    int i = 0;
    m_listRoomAssets.InsertColumn(i++, _T("序号"), LVCFMT_LEFT, columnWidth);
    //m_listRoomAssets.InsertColumn(1, _T("设备名称"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(i++, _T("机房号"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(i++, _T("说明"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(i++, _T("时间"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(i++, _T("管理员"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(i++, _T("接收者"), LVCFMT_LEFT, columnWidth);
    return TRUE;
}

void CAssetsListDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
    m_listRoomAssets.SetThreadDatabase(m_pDbThread);
}

void CAssetsListDlg::SetAssertDate(vector<ASSERT_DATA>& vecAssertData)
{
    m_vecAssertData = vecAssertData;

    int size = vecAssertData.size();
    for (int i = 0; i < size; i++)
    {
        ASSERT_DATA assertData = m_vecAssertData.at(i);
        CString strId;
        strId.Format(_T("%d"), i + 1);
        m_listRoomAssets.InsertItem(i, strId);
        int index = 1;
        //m_listRoomAssets.SetItemText(i, 1, assertData.strDeviceName);
        m_listRoomAssets.SetItemText(i, index++, assertData.strRoomNum);
        m_listRoomAssets.SetItemText(i, index++, assertData.strReason);
        m_listRoomAssets.SetItemText(i, index++, assertData.strOutDate);
        m_listRoomAssets.SetItemText(i, index++, assertData.strOwner);
        m_listRoomAssets.SetItemText(i, index++, assertData.strInDate);
    }
}


BOOL CAssetsListDlg::PreTranslateMessage(MSG* pMsg)
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


void CAssetsListDlg::OnBnClickedButton4()
{
    //CString strDeviceName;
    CString strManager;
    CString strRoomNum;
    CString strRecv;
    CString strOutDate;
    CString strReason;
    //CString strStatus;

    //GetDlgItem(IDC_EDIT_DEVICE_NAME)->GetWindowText(strDeviceName);
    GetDlgItem(IDC_EDIT_OWNER)->GetWindowText(strManager);
    GetDlgItem(IDC_EDIT_ROOM_NUM)->GetWindowText(strRoomNum);
    GetDlgItem(IDC_EDIT_IN_DATE)->GetWindowText(strRecv);
    GetDlgItem(IDC_EDIT_OUT_DATE)->GetWindowText(strOutDate);
    //GetDlgItem(IDC_EDIT_STATUS)->GetWindowText(strStatus);
    GetDlgItem(IDC_EDIT_REASON11)->GetWindowText(strReason);

    if (//strDeviceName.IsEmpty() ||
        strManager.IsEmpty() ||
        strRoomNum.IsEmpty() ||
        strRecv.IsEmpty() ||
        strOutDate.IsEmpty() ||
        strReason.IsEmpty())
    {
        ::MessageBox(NULL, _T("编辑框不能为空"), _T("提示"), MB_OK);
        return;
    }

    ASSERT_DATA *pReportData = new ASSERT_DATA;
    //pReportData->strDeviceName = strDeviceName;
    pReportData->strOwner = strManager;
    pReportData->strRoomNum = strRoomNum;
    pReportData->strInDate = strRecv;
    pReportData->strOutDate = strOutDate;
    //pReportData->strStatus = strStatus;
    pReportData->strReason = strReason;

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_INSERT_ASSERT_DATA, (WPARAM)pReportData, 0);

    int i = m_listRoomAssets.GetItemCount();
    CString strId;
    strId.Format(_T("%d"), i + 1);
    m_listRoomAssets.InsertItem(i, strId);
    int index = 1;
    //m_listRoomAssets.SetItemText(i, 1, assertData.strDeviceName);
    m_listRoomAssets.SetItemText(i, index++, strRoomNum);
    m_listRoomAssets.SetItemText(i, index++, strReason);
    m_listRoomAssets.SetItemText(i, index++, strOutDate);
    m_listRoomAssets.SetItemText(i, index++, strManager);
    m_listRoomAssets.SetItemText(i, index++, strRecv);

    //::MessageBox(NULL, _T("提交成功"), _T("提示"), MB_OK);
}



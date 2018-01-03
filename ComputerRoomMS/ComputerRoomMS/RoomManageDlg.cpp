// RoomManageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "RoomManageDlg.h"
#include "afxdialogex.h"


// CAssetsListDlg �Ի���

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


// CAssetsListDlg ��Ϣ�������


BOOL CAssetsListDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();


    DWORD ExStyle = m_listRoomAssets.GetExtendedStyle();
    m_listRoomAssets.SetExtendedStyle(ExStyle | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);

    CRect rcClient;
    m_listRoomAssets.GetClientRect(&rcClient);
    int columnWidth = rcClient.Width() / 7;
    m_listRoomAssets.InsertColumn(0, _T("���"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(1, _T("�豸����"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(2, _T("������"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(3, _T("��������"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(4, _T("���ʱ��"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(5, _T("����ʱ��"), LVCFMT_LEFT, columnWidth);
    m_listRoomAssets.InsertColumn(6, _T("��״"), LVCFMT_LEFT, columnWidth);

    return TRUE;
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
        m_listRoomAssets.SetItemText(i, 1, assertData.strDeviceName);
        m_listRoomAssets.SetItemText(i, 2, assertData.strOwner);
        m_listRoomAssets.SetItemText(i, 3, assertData.strRoomNum);
        m_listRoomAssets.SetItemText(i, 4, assertData.strInDate);
        m_listRoomAssets.SetItemText(i, 5, assertData.strOutDate);
        m_listRoomAssets.SetItemText(i, 6, assertData.strStatus);
    }
}


BOOL CAssetsListDlg::PreTranslateMessage(MSG* pMsg)
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

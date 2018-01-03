// ScheduleDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ScheduleDlg.h"
#include "afxdialogex.h"


// CScheduleDlg �Ի���

IMPLEMENT_DYNAMIC(CScheduleDlg, CDialogEx)

CScheduleDlg::CScheduleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_VIEW_SCHEDULE, pParent)
{

}

CScheduleDlg::~CScheduleDlg()
{
}

void CScheduleDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_listSchedule);
    DDX_Control(pDX, IDC_COMBO1, m_comboWeek);
}


BEGIN_MESSAGE_MAP(CScheduleDlg, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO1, &CScheduleDlg::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CScheduleDlg ��Ϣ�������


BOOL CScheduleDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    DWORD ExStyle = m_listSchedule.GetExtendedStyle();
    m_listSchedule.SetExtendedStyle(ExStyle | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);

    m_listSchedule.SetRowHeigt(50);

    CRect rcClient;
    m_listSchedule.GetClientRect(&rcClient);
    int columnWidth = rcClient.Width() / 6;
    m_listSchedule.InsertColumn(0, _T(""), LVCFMT_LEFT, columnWidth);
    m_listSchedule.InsertColumn(1, _T("����һ"), LVCFMT_LEFT, columnWidth);
    m_listSchedule.InsertColumn(2, _T("���ڶ�"), LVCFMT_LEFT, columnWidth);
    m_listSchedule.InsertColumn(3, _T("������"), LVCFMT_LEFT, columnWidth);
    m_listSchedule.InsertColumn(4, _T("������"), LVCFMT_LEFT, columnWidth);
    m_listSchedule.InsertColumn(5, _T("������"), LVCFMT_LEFT, columnWidth);

    m_listSchedule.InsertItem(0, _T("��1��"));
    //m_listSchedule.SetItemText(0, 1, _T("�����ԭ��\n�Ա���\n��ʦA301"));
    m_listSchedule.InsertItem(1, _T("��2��"));
    m_listSchedule.InsertItem(2, _T("��3��"));
    m_listSchedule.InsertItem(3, _T("��4��"));
    m_listSchedule.InsertItem(4, _T("��5��"));
    m_listSchedule.InsertItem(5, _T("��6��"));
    m_listSchedule.InsertItem(6, _T("��7��"));
    m_listSchedule.InsertItem(7, _T("��8��"));

    return TRUE;
}

void CScheduleDlg::SetScheduleInfo(vector<SCHEDULE_DATA>& vecScheduleData)
{
    m_vecScheduleData = vecScheduleData;

    for (int i = 0; i < vecScheduleData.size(); i++)
    {
        CString str;
        str.Format(_T("��%d��"), i + 1);
        m_comboWeek.InsertString(i, str);
    }
    m_comboWeek.SetCurSel(0);

    SCHEDULE_DATA scheduleData = m_vecScheduleData.at(0);

    int index = 1;
    m_listSchedule.SetItemText(0, index, scheduleData.strMon12);
    m_listSchedule.SetItemText(1, index, scheduleData.strMon12);
    m_listSchedule.SetItemText(2, index, scheduleData.strMon34);
    m_listSchedule.SetItemText(3, index, scheduleData.strMon34);
    m_listSchedule.SetItemText(4, index, scheduleData.strMon56);
    m_listSchedule.SetItemText(5, index, scheduleData.strMon56);
    m_listSchedule.SetItemText(6, index, scheduleData.strMon78);
    m_listSchedule.SetItemText(7, index, scheduleData.strMon78);

    m_listSchedule.SetItemText(0, index + 1, scheduleData.strTues12);
    m_listSchedule.SetItemText(1, index + 1, scheduleData.strTues12);
    m_listSchedule.SetItemText(2, index + 1, scheduleData.strTues34);
    m_listSchedule.SetItemText(3, index + 1, scheduleData.strTues34);
    m_listSchedule.SetItemText(4, index + 1, scheduleData.strTues56);
    m_listSchedule.SetItemText(5, index + 1, scheduleData.strTues56);
    m_listSchedule.SetItemText(6, index + 1, scheduleData.strTues78);
    m_listSchedule.SetItemText(7, index + 1, scheduleData.strTues78);

    m_listSchedule.SetItemText(0, index + 2, scheduleData.strWed12);
    m_listSchedule.SetItemText(1, index + 2, scheduleData.strWed12);
    m_listSchedule.SetItemText(2, index + 2, scheduleData.strWed34);
    m_listSchedule.SetItemText(3, index + 2, scheduleData.strWed34);
    m_listSchedule.SetItemText(4, index + 2, scheduleData.strWed56);
    m_listSchedule.SetItemText(5, index + 2, scheduleData.strWed56);
    m_listSchedule.SetItemText(6, index + 2, scheduleData.strWed78);
    m_listSchedule.SetItemText(7, index + 2, scheduleData.strWed78);

    m_listSchedule.SetItemText(0, index + 3, scheduleData.strThu12);
    m_listSchedule.SetItemText(1, index + 3, scheduleData.strThu12);
    m_listSchedule.SetItemText(2, index + 3, scheduleData.strThu34);
    m_listSchedule.SetItemText(3, index + 3, scheduleData.strThu34);
    m_listSchedule.SetItemText(4, index + 3, scheduleData.strThu56);
    m_listSchedule.SetItemText(5, index + 3, scheduleData.strThu56);
    m_listSchedule.SetItemText(6, index + 3, scheduleData.strThu78);
    m_listSchedule.SetItemText(7, index + 3, scheduleData.strThu78);

    m_listSchedule.SetItemText(0, index + 4, scheduleData.strFri12);
    m_listSchedule.SetItemText(1, index + 4, scheduleData.strFri12);
    m_listSchedule.SetItemText(2, index + 4, scheduleData.strFri34);
    m_listSchedule.SetItemText(3, index + 4, scheduleData.strFri34);
    m_listSchedule.SetItemText(4, index + 4, scheduleData.strFri56);
    m_listSchedule.SetItemText(5, index + 4, scheduleData.strFri56);
    m_listSchedule.SetItemText(6, index + 4, scheduleData.strFri78);
    m_listSchedule.SetItemText(7, index + 4, scheduleData.strFri78);
}


void CScheduleDlg::OnCbnSelchangeCombo1()
{
    m_listSchedule.DeleteAllItems();

    m_listSchedule.InsertItem(0, _T("��1��"));
    m_listSchedule.InsertItem(1, _T("��2��"));
    m_listSchedule.InsertItem(2, _T("��3��"));
    m_listSchedule.InsertItem(3, _T("��4��"));
    m_listSchedule.InsertItem(4, _T("��5��"));
    m_listSchedule.InsertItem(5, _T("��6��"));
    m_listSchedule.InsertItem(6, _T("��7��"));
    m_listSchedule.InsertItem(7, _T("��8��"));

    int selType = m_comboWeek.GetCurSel();

    SCHEDULE_DATA scheduleData = m_vecScheduleData.at(selType);

    int index = 1;
    m_listSchedule.SetItemText(0, index, scheduleData.strMon12);
    m_listSchedule.SetItemText(1, index, scheduleData.strMon12);
    m_listSchedule.SetItemText(2, index, scheduleData.strMon34);
    m_listSchedule.SetItemText(3, index, scheduleData.strMon34);
    m_listSchedule.SetItemText(4, index, scheduleData.strMon56);
    m_listSchedule.SetItemText(5, index, scheduleData.strMon56);
    m_listSchedule.SetItemText(6, index, scheduleData.strMon78);
    m_listSchedule.SetItemText(7, index, scheduleData.strMon78);

    m_listSchedule.SetItemText(0, index + 1, scheduleData.strTues12);
    m_listSchedule.SetItemText(1, index + 1, scheduleData.strTues12);
    m_listSchedule.SetItemText(2, index + 1, scheduleData.strTues34);
    m_listSchedule.SetItemText(3, index + 1, scheduleData.strTues34);
    m_listSchedule.SetItemText(4, index + 1, scheduleData.strTues56);
    m_listSchedule.SetItemText(5, index + 1, scheduleData.strTues56);
    m_listSchedule.SetItemText(6, index + 1, scheduleData.strTues78);
    m_listSchedule.SetItemText(7, index + 1, scheduleData.strTues78);

    m_listSchedule.SetItemText(0, index + 2, scheduleData.strWed12);
    m_listSchedule.SetItemText(1, index + 2, scheduleData.strWed12);
    m_listSchedule.SetItemText(2, index + 2, scheduleData.strWed34);
    m_listSchedule.SetItemText(3, index + 2, scheduleData.strWed34);
    m_listSchedule.SetItemText(4, index + 2, scheduleData.strWed56);
    m_listSchedule.SetItemText(5, index + 2, scheduleData.strWed56);
    m_listSchedule.SetItemText(6, index + 2, scheduleData.strWed78);
    m_listSchedule.SetItemText(7, index + 2, scheduleData.strWed78);

    m_listSchedule.SetItemText(0, index + 3, scheduleData.strThu12);
    m_listSchedule.SetItemText(1, index + 3, scheduleData.strThu12);
    m_listSchedule.SetItemText(2, index + 3, scheduleData.strThu34);
    m_listSchedule.SetItemText(3, index + 3, scheduleData.strThu34);
    m_listSchedule.SetItemText(4, index + 3, scheduleData.strThu56);
    m_listSchedule.SetItemText(5, index + 3, scheduleData.strThu56);
    m_listSchedule.SetItemText(6, index + 3, scheduleData.strThu78);
    m_listSchedule.SetItemText(7, index + 3, scheduleData.strThu78);

    m_listSchedule.SetItemText(0, index + 4, scheduleData.strFri12);
    m_listSchedule.SetItemText(1, index + 4, scheduleData.strFri12);
    m_listSchedule.SetItemText(2, index + 4, scheduleData.strFri34);
    m_listSchedule.SetItemText(3, index + 4, scheduleData.strFri34);
    m_listSchedule.SetItemText(4, index + 4, scheduleData.strFri56);
    m_listSchedule.SetItemText(5, index + 4, scheduleData.strFri56);
    m_listSchedule.SetItemText(6, index + 4, scheduleData.strFri78);
    m_listSchedule.SetItemText(7, index + 4, scheduleData.strFri78);
}

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
    ON_BN_CLICKED(IDC_BTN_SEARCH, &CScheduleDlg::OnBnClickedBtnSearch)
    ON_MESSAGE(WM_UPDATE_SCHDUEL, OnUpdateSchdule)
END_MESSAGE_MAP()


// CScheduleDlg ��Ϣ�������


BOOL CScheduleDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    DWORD ExStyle = m_listSchedule.GetExtendedStyle();
    m_listSchedule.SetExtendedStyle(ExStyle | LVS_EX_GRIDLINES | LVS_EX_TRACKSELECT);

    m_listSchedule.SetRowHeigt(50 * 2);

    CRect rcClient;
    m_listSchedule.GetClientRect(&rcClient);
    int columnWidth = rcClient.Width() / 6;
    m_listSchedule.InsertColumn(0, _T(""), LVCFMT_LEFT, columnWidth);
    m_listSchedule.InsertColumn(1, _T("����һ"), LVCFMT_LEFT, columnWidth);
    m_listSchedule.InsertColumn(2, _T("���ڶ�"), LVCFMT_LEFT, columnWidth);
    m_listSchedule.InsertColumn(3, _T("������"), LVCFMT_LEFT, columnWidth);
    m_listSchedule.InsertColumn(4, _T("������"), LVCFMT_LEFT, columnWidth);
    m_listSchedule.InsertColumn(5, _T("������"), LVCFMT_LEFT, columnWidth);

    m_listSchedule.InsertItem(0, _T("��1 / 2��"));
    m_listSchedule.InsertItem(1, _T("��3 / 4��"));
    m_listSchedule.InsertItem(2, _T("��4 / 5��"));
    m_listSchedule.InsertItem(3, _T("��7 / 8��"));

    return TRUE;
}

void CScheduleDlg::SetScheduleInfo(vector<CLASS_DATA>& vecScheduleData)
{
    m_vecScheduleData = vecScheduleData;

    m_mapWeek2Class.clear();
    for (unsigned int i = 0; i < vecScheduleData.size(); i++)
    {
        // ��ͬһ�ܵķŵ�ͬһ��map��Ӧ��vector
        m_mapWeek2Class[vecScheduleData.at(i).week].push_back(vecScheduleData.at(i));
    }

    for (UINT i = 0; i < m_mapWeek2Class.size(); ++i)
    {
        CString str;
        str.Format(_T("��%d��"), i + 1);
        m_comboWeek.InsertString(i, str);
    }
    m_comboWeek.SetCurSel(0);

    // �ѵ�һ�ܵĳ�ʼ�����б�
    vector<CLASS_DATA> vec = m_mapWeek2Class[1];
    for (UINT i = 0; i < vec.size(); ++i)
    {
        m_listSchedule.SetItemText(0, vec.at(i).day, vec.at(i).class_12);
        m_listSchedule.SetItemText(1, vec.at(i).day, vec.at(i).class_34);
        m_listSchedule.SetItemText(2, vec.at(i).day, vec.at(i).class_56);
        m_listSchedule.SetItemText(3, vec.at(i).day, vec.at(i).class_78);
    }
}

LRESULT CScheduleDlg::OnUpdateSchdule(WPARAM wParam, LPARAM lParam)
{
    CLASS_DATA* pClassData = (CLASS_DATA *)wParam;

    //m_listSchedule.SetItemText(0, pClassData->day, pClassData->class_text);

    map<int, vector<CLASS_DATA>>::iterator it1;
    vector<CLASS_DATA>::iterator it2;
    for (it1 = m_mapWeek2Class.begin(); it1 != m_mapWeek2Class.end(); ++it1) {
        if (it1->first == pClassData->week) {
            for (it2 = it1->second.begin(); it2 != it1->second.end(); ++it2) {
                if (it2->day == pClassData->day) {
                    if (1 == pClassData->class_num) {
                        it2->class_12 = pClassData->class_text;
                    }
                    else if (2 == pClassData->class_num) {
                        it2->class_34 = pClassData->class_text;
                    }
                    else if (3 == pClassData->class_num) {
                        it2->class_56 = pClassData->class_text;
                    }
                    else if (4 == pClassData->class_num) {
                        it2->class_78 = pClassData->class_text;
                    }
                    return 1;
                }
            }            
        }
    }

    return 1;
}


void CScheduleDlg::OnCbnSelchangeCombo1()
{
    m_listSchedule.DeleteAllItems();

    m_listSchedule.InsertItem(0, _T("��1 / 2��"));
    m_listSchedule.InsertItem(1, _T("��3 / 4��"));
    m_listSchedule.InsertItem(2, _T("��4 / 5��"));
    m_listSchedule.InsertItem(3, _T("��7 / 8��"));

    int selType = m_comboWeek.GetCurSel();

    vector<CLASS_DATA> vec = m_mapWeek2Class[selType + 1];
    for (UINT i = 0; i < vec.size(); ++i)
    {
        m_listSchedule.SetItemText(0, vec.at(i).day, vec.at(i).class_12);
        m_listSchedule.SetItemText(1, vec.at(i).day, vec.at(i).class_34);
        m_listSchedule.SetItemText(2, vec.at(i).day, vec.at(i).class_56);
        m_listSchedule.SetItemText(3, vec.at(i).day, vec.at(i).class_78);
    }
}


BOOL CScheduleDlg::PreTranslateMessage(MSG* pMsg)
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


void CScheduleDlg::OnBnClickedBtnSearch()
{
    CString strSearch;
    GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strSearch);

    if (strSearch.IsEmpty())
        MessageBox(_T("�༭����Ϊ�գ�"));

    for (UINT i = 1; i < m_mapWeek2Class.size(); ++i)
    {
        CString strWeek;
        strWeek.Format(_T("��%d��"), i);

        vector<CLASS_DATA> vec = m_mapWeek2Class[i];
        for (UINT m = 0; m < vec.size(); ++m)
        {
            CString str12 = vec.at(m).class_12;
            CString str34 = vec.at(m).class_34;
            CString str56 = vec.at(m).class_56;
            CString str78 = vec.at(m).class_78;

            if (-1 != str12.Find(strSearch)) {
                CString str;
                str = strWeek + _T(" | ") + _T("��1/2��") + str12;
                MessageBox(str);
                //return;
            }
            else if (-1 != str34.Find(strSearch)) {
                CString str;
                str = strWeek + _T(" | ") + _T("��3/4��:") + str34;
                MessageBox(str);
                //return;
            }
            else if (-1 != str56.Find(strSearch)) {
                CString str;
                str = strWeek + _T(" | ") + _T("��5/6��") + str56;
                MessageBox(str);
                //return;
            }
            else if (-1 != str78.Find(strSearch)) {
                CString str;
                str = strWeek + _T(" | ") + _T("��7/8��") + str78;
                MessageBox(str);
                //return;
            }
        }
    }

}

// ScheduleAdjustDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ScheduleAdjustDlg.h"
#include "afxdialogex.h"


// CScheduleAdjustDlg �Ի���

IMPLEMENT_DYNAMIC(CScheduleAdjustDlg, CDialogEx)

CScheduleAdjustDlg::CScheduleAdjustDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_SCHEDULE_ADJUST, pParent)
{

}

CScheduleAdjustDlg::~CScheduleAdjustDlg()
{
}

void CScheduleAdjustDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_DAY, m_comboWeek);
    DDX_Control(pDX, IDC_COMBO_CLASS, m_comboClass);
    DDX_Control(pDX, IDC_COMBO_WEEK_NUM1, m_weekNum);
    DDX_Control(pDX, IDC_COMBO_CLASS_NAME, m_comboGrage);
}


BEGIN_MESSAGE_MAP(CScheduleAdjustDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_VERIFY, &CScheduleAdjustDlg::OnBnClickedBtnVerify)
END_MESSAGE_MAP()


// CScheduleAdjustDlg ��Ϣ�������
BOOL CScheduleAdjustDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    for (int i = 0; i < 10; ++i)
    {
        CString strTmp;
        strTmp.Format(_T("��%d��"), i + 1);
        m_weekNum.InsertString(i, strTmp);
    }
    m_weekNum.SetCurSel(0);

    m_comboWeek.InsertString(0, _T("����һ"));
    m_comboWeek.InsertString(1, _T("���ڶ�"));
    m_comboWeek.InsertString(2, _T("������"));
    m_comboWeek.InsertString(3, _T("������"));
    m_comboWeek.InsertString(4, _T("������"));
    //m_comboWeek.InsertString(5, _T("������"));
    //m_comboWeek.InsertString(6, _T("������"));
    m_comboWeek.SetCurSel(0);

    m_comboClass.InsertString(0, _T("��1/2��"));
    m_comboClass.InsertString(1, _T("��3/4��"));
    m_comboClass.InsertString(2, _T("��5/6��"));
    m_comboClass.InsertString(3, _T("��7/8��"));
    m_comboClass.SetCurSel(0);

    return TRUE;
}

void CScheduleAdjustDlg::OnBnClickedBtnVerify()
{
    CString strWeek;
    CString strClassName;
    CString strRoomNum;
    CString strTeacherName;

    //GetDlgItem(IDC_EDIT_WEEK)->GetWindowText(strWeek);
    GetDlgItem(IDC_EDIT_CLASS_NAME)->GetWindowText(strClassName);
    GetDlgItem(IDC_EDIT_ROOM_NUM)->GetWindowText(strRoomNum);
    GetDlgItem(IDC_EDIT_TEAC_NAME)->GetWindowText(strTeacherName);

    if (//strWeek.IsEmpty() ||
        strClassName.IsEmpty() ||
        strRoomNum.IsEmpty() ||
        strTeacherName.IsEmpty())
    {
        MessageBox(_T("�༭����Ϊ�գ�"));
        return;
    }

    CLASS_DATA* pClassData = new CLASS_DATA;
    pClassData->week = m_weekNum.GetCurSel() + 1;
    pClassData->day = m_comboWeek.GetCurSel() + 1;
    pClassData->class_num = m_comboClass.GetCurSel() + 1;
    CString strGrage;
    m_comboGrage.GetLBText(m_comboGrage.GetCurSel(), strGrage);
    pClassData->class_text = strClassName + _T("|��ʦ��") + strTeacherName + _T("|�����ţ�") + strRoomNum  + _T("|�༶��") + strGrage;

    ::SendMessage(m_scheduleHwnd, WM_UPDATE_SCHDUEL, (WPARAM)pClassData, 0);
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_UPDATE_CLASS_DATA, (WPARAM)pClassData, 0);

    MessageBox(_T("�޸ĳɹ���"));
}

void CScheduleAdjustDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}

void CScheduleAdjustDlg::SetClassName(set<CString> setClassName)
{
    m_setClassName = setClassName;

    set<CString>::iterator it;
    int i = 0;
    for (it = m_setClassName.begin(); it != m_setClassName.end(); ++it)
    {
        m_comboGrage.InsertString(i++, *it);
    }
    if (0 == m_comboGrage.GetCount())
    {
        MessageBox(_T("ѧ��Ϊ�գ��뵼��ѧ����Ϣ���ٽ��пγ̵���"));
        m_comboGrage.InsertString(0, _T("�༶��ϢΪ��"));
    }
    else
    {
        m_comboGrage.SetCurSel(0);
    }
}


BOOL CScheduleAdjustDlg::PreTranslateMessage(MSG* pMsg)
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

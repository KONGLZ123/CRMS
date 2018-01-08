// ScheduleAdjustDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ScheduleAdjustDlg.h"
#include "afxdialogex.h"


// CScheduleAdjustDlg 对话框

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


// CScheduleAdjustDlg 消息处理程序
BOOL CScheduleAdjustDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    for (int i = 0; i < 10; ++i)
    {
        CString strTmp;
        strTmp.Format(_T("第%d周"), i + 1);
        m_weekNum.InsertString(i, strTmp);
    }
    m_weekNum.SetCurSel(0);

    m_comboWeek.InsertString(0, _T("星期一"));
    m_comboWeek.InsertString(1, _T("星期二"));
    m_comboWeek.InsertString(2, _T("星期三"));
    m_comboWeek.InsertString(3, _T("星期四"));
    m_comboWeek.InsertString(4, _T("星期五"));
    //m_comboWeek.InsertString(5, _T("星期六"));
    //m_comboWeek.InsertString(6, _T("星期七"));
    m_comboWeek.SetCurSel(0);

    m_comboClass.InsertString(0, _T("第1/2节"));
    m_comboClass.InsertString(1, _T("第3/4节"));
    m_comboClass.InsertString(2, _T("第5/6节"));
    m_comboClass.InsertString(3, _T("第7/8节"));
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
        MessageBox(_T("编辑框不能为空！"));
        return;
    }

    CLASS_DATA* pClassData = new CLASS_DATA;
    pClassData->week = m_weekNum.GetCurSel() + 1;
    pClassData->day = m_comboWeek.GetCurSel() + 1;
    pClassData->class_num = m_comboClass.GetCurSel() + 1;
    CString strGrage;
    m_comboGrage.GetLBText(m_comboGrage.GetCurSel(), strGrage);
    pClassData->class_text = strClassName + _T("|教师：") + strTeacherName + _T("|机房号：") + strRoomNum  + _T("|班级：") + strGrage;

    ::SendMessage(m_scheduleHwnd, WM_UPDATE_SCHDUEL, (WPARAM)pClassData, 0);
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_UPDATE_CLASS_DATA, (WPARAM)pClassData, 0);

    MessageBox(_T("修改成功！"));
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
        MessageBox(_T("学生为空，请导入学生信息后再进行课程调整"));
        m_comboGrage.InsertString(0, _T("班级信息为空"));
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

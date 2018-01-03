// PersonInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "PersonInfo.h"
#include "afxdialogex.h"
#include "DlgMain.h"
#include "AddPersonDlg.h"
#include "ViewReigstRequestDlg.h"
#include "RegisterDlg.h"

// CPersonInfo 对话框

IMPLEMENT_DYNAMIC(CPersonInfo, CDialogEx)

CPersonInfo::CPersonInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_PERSON_INFO, pParent)
{

}

CPersonInfo::~CPersonInfo()
{
}

void CPersonInfo::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_COMBO_ROLE, m_comboRole);
    DDX_Control(pDX, IDC_EDIT_SEARCH, m_editSearch);
    DDX_Control(pDX, IDC_LIST_PERSON_INFO, m_listPersonInfo);
}


BEGIN_MESSAGE_MAP(CPersonInfo, CDialogEx)
    ON_CBN_SELCHANGE(IDC_COMBO_ROLE, &CPersonInfo::OnCbnSelchangeComboRole)
    ON_BN_CLICKED(IDC_BTN_SEARCH_STUDENT, &CPersonInfo::OnBnClickedBtnSearchStudent)
    ON_BN_CLICKED(IDC_BTN_UPLOAD, &CPersonInfo::OnBnClickedBtnUpload)
    ON_BN_CLICKED(IDC_BTN_ADD_PERSON, &CPersonInfo::OnBnClickedBtnAddPerson)
    ON_BN_CLICKED(IDC_BTN_REGISTER, &CPersonInfo::OnBnClickedBtnViewRegister)
END_MESSAGE_MAP()


// CPersonInfo 消息处理程序


BOOL CPersonInfo::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    InitCtrls();

    return TRUE;  
}

void CPersonInfo::InitCtrls()
{
    //m_comboRole.InsertString(0, _T("系统后台管理员"));
    m_comboRole.InsertString(0, _T("机房管理员"));
    m_comboRole.InsertString(1, _T("机房维护人员"));
    m_comboRole.InsertString(2, _T("教师"));
    //m_comboRole.InsertString(4, _T("学生"));
    m_comboRole.SetCurSel(0);

    DWORD ExStyle = m_listPersonInfo.GetExtendedStyle();
    m_listPersonInfo.SetExtendedStyle(ExStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    CRect rcClient;
    m_listPersonInfo.GetClientRect(&rcClient);
    int columnWidth = rcClient.Width() / 6;
    m_listPersonInfo.InsertColumn(0, _T("序号"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(1, _T("账号"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(2, _T("密码"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(3, _T("姓名"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(4, _T("电话"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(5, _T("权限"), LVCFMT_LEFT, columnWidth + 3);

    //m_listPersonInfo.InsertItem(0, _T("1"));
    //m_listPersonInfo.SetItemText(0, 1, _T("张学友"));
    //m_listPersonInfo.SetItemText(0, 2, _T("男"));
    //m_listPersonInfo.SetItemText(0, 3, _T("20170101"));
    //m_listPersonInfo.SetItemText(0, 4, _T("正常"));

    //m_listPersonInfo.InsertItem(1, _T("2"));
    //m_listPersonInfo.SetItemText(1, 1, _T("刘德华"));
    //m_listPersonInfo.SetItemText(1, 2, _T("男"));
    //m_listPersonInfo.SetItemText(1, 3, _T("20170102"));
    //m_listPersonInfo.SetItemText(1, 4, _T("锁定"));
}

BOOL CPersonInfo::InitData()
{
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_GET_PERSON_INFO, (WPARAM)m_hWnd, 0);
    return 0;
}

void CPersonInfo::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}

void CPersonInfo::SetPersonInfo(DATA_STOCK &personInfo)
{
    m_personInfo = personInfo;

    int size = m_personInfo.vecRoomManagerInfo.size();
    for (int i = 0; i < size; i++)
    {
        ROOM_MANAGER_INFO roomManagerInfo = m_personInfo.vecRoomManagerInfo.at(i);
        CString strId;
        strId.Format(_T("%d"), i + 1);
        m_listPersonInfo.InsertItem(i, strId);
        m_listPersonInfo.SetItemText(i, 1, roomManagerInfo.account);
        m_listPersonInfo.SetItemText(i, 2, roomManagerInfo.pwd);
        m_listPersonInfo.SetItemText(i, 3, roomManagerInfo.name);
        m_listPersonInfo.SetItemText(i, 4, roomManagerInfo.tel);
        CString strTemp;
        strTemp.Format(_T("%d"), roomManagerInfo.authority);
        m_listPersonInfo.SetItemText(i, 5, strTemp);
    }
}

void CPersonInfo::SetRegisterData(vector<ADD_PERSON_DATA>& vecRegisterData)
{
    m_vecRegisterData = vecRegisterData;
}


void CPersonInfo::OnCbnSelchangeComboRole()
{
    int sel = m_comboRole.GetCurSel();
    m_listPersonInfo.DeleteAllItems();

    switch (sel)
    {
    case 0: // 机房管理员
    {
        int size = m_personInfo.vecRoomManagerInfo.size();
        for (int i = 0; i < size; i++)
        {
            ROOM_MANAGER_INFO roomManagerInfo = m_personInfo.vecRoomManagerInfo.at(i);
            CString strId;
            strId.Format(_T("%d"), i + 1);
            m_listPersonInfo.InsertItem(i, strId);
            m_listPersonInfo.SetItemText(i, 1, roomManagerInfo.account);
            m_listPersonInfo.SetItemText(i, 2, roomManagerInfo.pwd);
            m_listPersonInfo.SetItemText(i, 3, roomManagerInfo.name);
            m_listPersonInfo.SetItemText(i, 4, roomManagerInfo.tel);
            CString strTemp;
            strTemp.Format(_T("%d"), roomManagerInfo.authority);
            m_listPersonInfo.SetItemText(i, 5, strTemp);
        }
    }
    break;
    case 1: // 机房维护人员
    { 
        int size = m_personInfo.vecRoomRepairInfo.size();
        for (int i = 0; i < size; i++)
        {
            ROOM_REPAIR_INFO roomRepairInfo = m_personInfo.vecRoomRepairInfo.at(i);
            CString strId;
            strId.Format(_T("%d"), i + 1);
            m_listPersonInfo.InsertItem(i, strId);
            m_listPersonInfo.SetItemText(i, 1, roomRepairInfo.account);
            m_listPersonInfo.SetItemText(i, 2, roomRepairInfo.pwd);
            m_listPersonInfo.SetItemText(i, 3, roomRepairInfo.name);
            m_listPersonInfo.SetItemText(i, 4, roomRepairInfo.tel);
            CString strTemp;
            strTemp.Format(_T("%d"), roomRepairInfo.authority);
            m_listPersonInfo.SetItemText(i, 5, strTemp);
        }
    }
    break;
    case 2: // 教师
    {
        m_listPersonInfo.DeleteAllItems();
        int size = m_personInfo.vecTeacherInfo.size();
        for (int i = 0; i < size; i++)
        {
            TEACHER_INFO teacherInfo = m_personInfo.vecTeacherInfo.at(i);
            CString strId;
            strId.Format(_T("%d"), i + 1);
            m_listPersonInfo.InsertItem(i, strId);
            m_listPersonInfo.SetItemText(i, 1, teacherInfo.account);
            m_listPersonInfo.SetItemText(i, 2, teacherInfo.pwd);
            m_listPersonInfo.SetItemText(i, 3, teacherInfo.name);
            m_listPersonInfo.SetItemText(i, 4, teacherInfo.tel);
            CString strTemp;
            strTemp.Format(_T("%d"), teacherInfo.authority);
            m_listPersonInfo.SetItemText(i, 5, strTemp);
        }
    }
    break;
    default:
        break;
    }
}


void CPersonInfo::OnBnClickedBtnSearchStudent()
{
    CString strStuId;
    GetDlgItem(IDC_EDIT_SEARCH)->GetWindowText(strStuId);

    if (strStuId.IsEmpty())
    {
        ::MessageBox(this->m_hWnd, _T("请输入学号"), _T("提示"), MB_OK);
        return;
    }
        
    int size = m_personInfo.vecStudentInfo.size();
    for (int i = 0; i < size; i++)
    {
        STUDETN_INFO studetnInfo = m_personInfo.vecStudentInfo.at(i);
        if (strStuId == studetnInfo.student_id)
        {
            //studetnInfo.student_id
            //studetnInfo.pwd
            //studetnInfo.name
            //studetnInfo.tel
            //studetnInfo.authority
            return;
        }
    }

    ::MessageBox(this->m_hWnd, _T("未找到此学生"), _T("提示"), MB_OK);
}


void CPersonInfo::OnBnClickedBtnUpload()
{
    CString FilePathName;
    CFileDialog dlg(TRUE, //TRUE为OPEN对话框，FALSE为SAVE AS对话框
        NULL,
        NULL,
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
        (LPCTSTR)_TEXT("文本文件 (*.txt) | 所有文件 (*.*)|*.*||"),
        NULL);
    if (dlg.DoModal() == IDOK)
    {
        FilePathName = dlg.GetPathName(); //文件名保存在了FilePathName里
    }
    else
    {
        return;
    }
}


void CPersonInfo::OnBnClickedBtnAddPerson()
{
    //CAddPersonDlg dlg;
    //dlg.DoModal();

    CRegisterDlg dlg;

    dlg.SetTitleAndBtn(_T("添加账号"), _T("添加"));
    dlg.SetThreadDatabase(m_pDbThread);
    dlg.DoModal();
}


void CPersonInfo::OnBnClickedBtnViewRegister()
{
    CViewReigstRequestDlg dlg;

    dlg.SetRegisterData(m_vecRegisterData);
    dlg.DoModal();
}

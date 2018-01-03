// StudentManageDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "StudentManageDlg.h"
#include "afxdialogex.h"


// CStudentManageDlg �Ի���

IMPLEMENT_DYNAMIC(CStudentManageDlg, CDialogEx)

CStudentManageDlg::CStudentManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_STUDENT_MANAGE, pParent)
{

}

CStudentManageDlg::~CStudentManageDlg()
{
}

void CStudentManageDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_STU_MANAGER, m_listPersonInfo);
    //DDX_Control(pDX, IDC_COMBO_EXAM_CLASS, m_comboExamClass);
    DDX_Control(pDX, IDC_COMBO_CLASS_NAME1, m_comboClass1);
    DDX_Control(pDX, IDC_COMBO_CLASS_NAME2, m_comboClass2);
}


BEGIN_MESSAGE_MAP(CStudentManageDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_DECLARE_EXAM, &CStudentManageDlg::OnBnClickedBtnDeclareExam)
    ON_CBN_SELCHANGE(IDC_COMBO_CLASS_NAME1, &CStudentManageDlg::OnCbnSelchangeComboClassName1)
    ON_MESSAGE(WM_UPDATE_STUDENT_LIST_ITEM, OnUpdateStudentListItem)
END_MESSAGE_MAP()


// CStudentManageDlg ��Ϣ�������

BOOL CStudentManageDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    m_listPersonInfo.SetStyle(LIST_STYLE_STUDENT_MANAGER);

    DWORD ExStyle = m_listPersonInfo.GetExtendedStyle();
    m_listPersonInfo.SetExtendedStyle(ExStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    CRect rcClient;
    m_listPersonInfo.GetClientRect(&rcClient);
    int columnWidth = rcClient.Width() / 5;
    m_listPersonInfo.InsertColumn(0, _T("���"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(1, _T("ѧ��"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(2, _T("����"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(3, _T("ȱ�ڴ���"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(4, _T("ƽ����"), LVCFMT_LEFT, columnWidth);

    //m_comboExamClass.InsertString(0, _T("test"));

    return TRUE;
}

void CStudentManageDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
    m_listPersonInfo.SetThreadDatabase(pDbThread);
}

void CStudentManageDlg::SetStudentInfo(vector<STUDETN_INFO>& vecStudentInfo)
{
    m_vecStudentInfo = vecStudentInfo;

    if (0 == vecStudentInfo.size())
        return;

    m_mapClass2StuInfo.clear();
    for (unsigned int i = 0; i < m_vecStudentInfo.size(); i++)
    {
        m_mapClass2StuInfo[m_vecStudentInfo.at(i).classes].push_back(m_vecStudentInfo.at(i));
    }

    m_comboClass1.SetCurSel(0);

    // �ѵ�һ�ܵĳ�ʼ�����б�
    vector<STUDETN_INFO> vec = m_mapClass2StuInfo[m_vecStudentInfo.at(0).classes];
    STUDETN_INFO stuInfo;
    CString strTmp;
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        stuInfo = vec.at(i);

        strTmp.Format(_T("%d"), i + 1);
        m_listPersonInfo.InsertItem(i, strTmp);
        m_listPersonInfo.SetItemText(i, 1, stuInfo.student_id);
        m_listPersonInfo.SetItemText(i, 2, stuInfo.name);
        //strTmp.Format(_T("%d"), stuInfo.attendece_cnt);
        m_listPersonInfo.SetItemText(i, 3, strTmp);
        //strTmp.Format(_T("%d"), stuInfo.attendece_score);
        m_listPersonInfo.SetItemText(i, 4, strTmp);
    }
}

void CStudentManageDlg::SetClassName(set<CString> setClassName)
{
    m_setClassName = setClassName;

    set<CString>::iterator it;
    int i = 0;
    for (it = m_setClassName.begin(); it != m_setClassName.end(); ++it, i++)
    {
        m_comboClass1.InsertString(i, *it);
        m_comboClass2.InsertString(i, *it);
    }
    if (0 == m_comboClass1.GetCount())
    {
        MessageBox(_T("ѧ��Ϊ�գ��뵼��ѧ����Ϣ���ٽ��пγ̵���"));
        m_comboClass1.InsertString(0, _T("�༶��ϢΪ��"));
        m_comboClass2.InsertString(0, _T("�༶��ϢΪ��"));
    }
    else
    {
        m_comboClass1.SetCurSel(0);
        m_comboClass2.SetCurSel(0);
    }
}

void CStudentManageDlg::OnBnClickedBtnDeclareExam()
{
    CString strExam;
    CString strPerson;
    CString strDate;
    CString strClass;

    GetDlgItem(IDC_EDIT_DELCALER_EDIT)->GetWindowText(strExam);
    GetDlgItem(IDC_EDIT_EXAM_DELCLARE)->GetWindowText(strPerson);
    GetDlgItem(IDC_EDIT_EXAM_DATE)->GetWindowText(strDate);
    m_comboClass2.GetLBText(m_comboClass2.GetCurSel(), strClass);

    if (strExam.IsEmpty() ||
        strPerson.IsEmpty() ||
        strDate.IsEmpty() ||
        strClass.IsEmpty())
    {
        MessageBox(_T("�༭����Ϊ�գ�"));
    }

    EXAM_INFO* pExamInfo = new EXAM_INFO;
    pExamInfo->strText = strExam;
    pExamInfo->strPerson = strPerson;
    pExamInfo->strDate = strDate;
    pExamInfo->strClass = strClass;

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_DECALRE_EXAM_INFO, (WPARAM)pExamInfo, 0);
}


void CStudentManageDlg::OnCbnSelchangeComboClassName1()
{
    m_listPersonInfo.DeleteAllItems();

    CString strText;
    m_comboClass1.GetLBText(m_comboClass1.GetCurSel(), strText);

    // �ѵ�һ�ܵĳ�ʼ�����б�
    vector<STUDETN_INFO> vec = m_mapClass2StuInfo[strText];
    STUDETN_INFO stuInfo;
    CString strTmp;
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        stuInfo = vec.at(i);

        strTmp.Format(_T("%d"), i + 1);
        m_listPersonInfo.InsertItem(i, strTmp);
        m_listPersonInfo.SetItemText(i, 1, stuInfo.student_id);
        m_listPersonInfo.SetItemText(i, 2, stuInfo.name);
        //strTmp.Format(_T("%d"), stuInfo.attendece_cnt);
        m_listPersonInfo.SetItemText(i, 3, strTmp);
        //strTmp.Format(_T("%d"), stuInfo.attendece_score);
        m_listPersonInfo.SetItemText(i, 4, strTmp);
    }
}



BOOL CStudentManageDlg::PreTranslateMessage(MSG* pMsg)
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

LRESULT CStudentManageDlg::OnUpdateStudentListItem(WPARAM wParam, LPARAM lParam)
{
    return 0;
}

// PersonInfo.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "PersonInfo.h"
#include "afxdialogex.h"
#include "DlgMain.h"
#include "ViewReigstRequestDlg.h"
#include "RegisterDlg.h"
#include "CApplication.h"
#include "CFont0.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "SearchResultDlg.h"

// CPersonInfo �Ի���

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
    ON_MESSAGE(WM_UPDATE_LIST_ITEM, OnUpdateListItem)
    ON_MESSAGE(WM_DEL_LIST_ITEM, OnDelListItem)
    ON_MESSAGE(WM_ADD_LIST_ITEM, OnAddListItem)
END_MESSAGE_MAP()


// CPersonInfo ��Ϣ�������


BOOL CPersonInfo::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    InitCtrls();

    return TRUE;  
}

void CPersonInfo::InitCtrls()
{
    //m_comboRole.InsertString(0, _T("ϵͳ��̨����Ա"));
    m_comboRole.InsertString(0, _T("��������Ա"));
    m_comboRole.InsertString(1, _T("����ά����Ա"));
    m_comboRole.InsertString(2, _T("��ʦ"));
    //m_comboRole.InsertString(4, _T("ѧ��"));
    m_comboRole.SetCurSel(0);

    m_listPersonInfo.SetStyle(LIST_STYLE_PERSON_INFO);
    
    DWORD ExStyle = m_listPersonInfo.GetExtendedStyle();
    m_listPersonInfo.SetExtendedStyle(ExStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    CRect rcClient;
    m_listPersonInfo.GetClientRect(&rcClient);
    int columnWidth = rcClient.Width() / 5;
    m_listPersonInfo.InsertColumn(0, _T("���"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(1, _T("�˺�"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(2, _T("����"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(3, _T("����"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(4, _T("�绰"), LVCFMT_LEFT, columnWidth);
    m_listPersonInfo.InsertColumn(5, _T("Ȩ��"), LVCFMT_LEFT, 0);
}

BOOL CPersonInfo::InitData()
{
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_GET_PERSON_INFO, (WPARAM)m_hWnd, 0);
    return 0;
}

void CPersonInfo::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
    m_listPersonInfo.SetThreadDatabase(m_pDbThread);
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

    m_registeDlg.SetThreadDatabase(m_pDbThread);
    m_registeDlg.SetRegisterData(m_vecRegisterData);
}

//void CPersonInfo::UpdatePersonInfo(PERSON_INFO_PART * pPersonInfo)
//{
//    PostThreadMessage(m_pDbThread->m_nThreadID, WM_UPDATE_PERSON_INFO, (WPARAM)pPersonInfo, 0);
//}


void CPersonInfo::OnCbnSelchangeComboRole()
{
    int sel = m_comboRole.GetCurSel();
    m_listPersonInfo.DeleteAllItems();

    switch (sel)
    {
    case 0: // ��������Ա
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
    case 1: // ����ά����Ա
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
    case 2: // ��ʦ
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
        ::MessageBox(this->m_hWnd, _T("������ѧ��"), _T("��ʾ"), MB_OK);
        return;
    }
        
    int size = m_personInfo.vecStudentInfo.size();
    for (int i = 0; i < size; i++)
    {
        STUDETN_INFO studetnInfo = m_personInfo.vecStudentInfo.at(i);
        if (strStuId == studetnInfo.student_id)
        {
            CSearchResultDlg dlg;
            dlg.SetStudentInfo(studetnInfo);
            dlg.DoModal();
            GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText(_T(""));
            return;
        }
    }

    GetDlgItem(IDC_EDIT_SEARCH)->SetWindowText(_T(""));
    ::MessageBox(this->m_hWnd, _T("δ�ҵ���ѧ��"), _T("��ʾ"), MB_OK);
}


void CPersonInfo::OnBnClickedBtnUpload()
{
    CFileDialog  dlg(TRUE, _T("*.xlsx"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("Xls�ļ� (*.xlsx)|*.xlsx"));
    dlg.m_ofn.lpstrTitle = L"���ļ�";
    CString FilePathName;

    if (dlg.DoModal() == IDOK)
    {
        FilePathName = dlg.GetPathName(); //�ļ�����������FilePathName��
        CApplication app;
        CWorkbook book;
        CWorkbooks books;
        CWorksheet sheet;
        CWorksheets sheets;
        CRange range;
        LPDISPATCH lpDisp;

        COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
        if (!app.CreateDispatch(_T("Excel.Application"), NULL)) {
            this->MessageBox(_T("�޷�����ExcelӦ��"));
            return;
        }
        books = app.get_Workbooks();
        //��Excel������pathnameΪExcel���·����
        lpDisp = books.Open(FilePathName, covOptional
            , covOptional, covOptional, covOptional, covOptional, covOptional, covOptional
            , covOptional, covOptional, covOptional
            , covOptional, covOptional, covOptional
            , covOptional);
        book.AttachDispatch(lpDisp);
        sheets = book.get_Worksheets();
        sheet = sheets.get_Item(COleVariant((short)1));

        //app.put_Visible(TRUE); //���ɼ�
        //app.put_UserControl(FALSE); 

        ////�������Ϊ��A��1���ĵ�Ԫ��
        /*range = sheet.get_Range(COleVariant(_T("A1"))
        ,COleVariant(_T("A1")));*/
        ////��õ�Ԫ�������
        //COleVariant rValue;rValue = 
        // COleVariant(range.get_Value2());
        ////ת���ɿ��ַ�
        //rValue.ChangeType(VT_BSTR);
        ////ת����ʽ�������
        //this->MessageBox(CString(rValue.bstrVal));

        /*��ȡExcel���еĶ����Ԫ���ֵ����listctrl����ʾ*/
        COleVariant vResult;

        //��ȡ�Ѿ�ʹ���������Ϣ�������Ѿ�ʹ�õ���������������ʼ�С���ʼ��
        range.AttachDispatch(sheet.get_UsedRange());
        range.AttachDispatch(range.get_Rows());     //ȡ���Ѿ�ʹ�õ�����
        long iRowNum = range.get_Count();
        range.AttachDispatch(range.get_Columns());  //ȡ���Ѿ�ʹ�õ�����
        long iColNum = range.get_Count();   //ȡ����ʹ���������ʼ�У���1��ʼ
        long iStartRow = range.get_Row();   //ȡ����ʹ���������ʼ�У���1��ʼ
        long iStartCol = range.get_Column();

        for (int i = iStartRow; i <= iRowNum; i++)
        {
            if (1 == i)
                continue;

            ADD_PERSON_DATA *ptagAddPerson = new ADD_PERSON_DATA;
            for (int j = iStartCol; j <= iColNum; j++)
            {
                //��ȡ��Ԫ���ֵ
                range.AttachDispatch(sheet.get_Cells());
                range.AttachDispatch(range.get_Item(COleVariant((long)i),
                    COleVariant((long)j)).pdispVal);
                vResult = range.get_Value2();
                CString str, stry, strm, strd;
                //SYSTEMTIME st;
                if (vResult.vt == VT_BSTR)     //�����ַ���
                {
                    str = vResult.bstrVal;
                }
                else if (vResult.vt == VT_R8) //8�ֽڵ�����
                {
                    str.Format(L"%d", static_cast<int>(vResult.dblVal));
                }
                //else if (vResult.vt == VT_DATE) //ʱ���ʽ
                //{
                //    VariantTimeToSystemTime(vResult.date, &st);
                //    stry.Format(L"%d", st.wYear);
                //    strm.Format(L"%d", st.wMonth);
                //    strd.Format(L"%d", st.wDay);
                //    str = stry + L"-" + strm + L"-" + strd;
                //}
                else if (vResult.vt == VT_EMPTY) //��ԪΪ��
                {
                    str = L"";
                }
                else if (vResult.vt == VT_I4)
                {
                    str.Format(_T("%ld"), (int)vResult.lVal);
                }

                switch (j)
                {
                case 1:
                {
                    ptagAddPerson->account = str;
                    break;
                }
                case 2:
                {
                    ptagAddPerson->name = str;
                    break;
                }
                case 3:
                {
                    ptagAddPerson->tel = str;
                    break;
                }
                break;
                case 4:
                {
                    ptagAddPerson->grade = str; // �༶
                }
                break;
                case 5: // רҵ
                {
                    ptagAddPerson->major = str;
                }
                break;
                case 6: { // �γ�
                    ptagAddPerson->course = str;
                    break;
                }
                default:
                    break;
                }
            } // end for j

            //ptagAddPerson->account = ptagAddPerson->id;
            ptagAddPerson->pwd = ptagAddPerson->account;
            ptagAddPerson->authority = STUDENT;
            ptagAddPerson->bIsRegister = FALSE;

            STUDETN_INFO stuInfo;
            stuInfo.authority = ptagAddPerson->authority;
            stuInfo.student_id = ptagAddPerson->account;
            stuInfo.pwd = ptagAddPerson->account;
            stuInfo.name = ptagAddPerson->name;
            stuInfo.classes = ptagAddPerson->grade;
            stuInfo.major = ptagAddPerson->major;
            stuInfo.tel = ptagAddPerson->tel;
            stuInfo.course = ptagAddPerson->course;
            m_personInfo.vecStudentInfo.push_back(stuInfo);

            PostThreadMessage(m_pDbThread->m_nThreadID, WM_ADD_PERSON, (WPARAM)ptagAddPerson, NULL);
        } // end for i

        /*�ͷ���Դ*/
        range.ReleaseDispatch();
        sheet.ReleaseDispatch();
        sheets.ReleaseDispatch();
        book.ReleaseDispatch();
        books.ReleaseDispatch();
        app.Quit();
        app.ReleaseDispatch();

        MessageBox(_T("����ɹ�"));
    }
    else
    {
        //MessageBox(_T("����ʧ�ܣ������ʽ"));
        return;
    }
}


void CPersonInfo::OnBnClickedBtnAddPerson()
{
    //CAddPersonDlg dlg;
    //dlg.DoModal();

    CRegisterDlg dlg(this);

    dlg.SetHwnd(this->m_hWnd);
    dlg.SetTitleAndBtn(_T("����˺�"), _T("���"));
    dlg.SetThreadDatabase(m_pDbThread);
    dlg.DoModal();
}


void CPersonInfo::OnBnClickedBtnViewRegister()
{
    m_registeDlg.DoModal();
}

LRESULT CPersonInfo::OnUpdateListItem(WPARAM wParam, LPARAM lParam)
{
    PERSON_INFO_PART *pPersonInfo = (PERSON_INFO_PART *)wParam;

    int sel = m_comboRole.GetCurSel();

    switch (sel)
    {
    case 0: // ��������Ա
    {
        vector<ROOM_MANAGER_INFO>::iterator it;
            
        for (it = m_personInfo.vecRoomManagerInfo.begin();
            it != m_personInfo.vecRoomManagerInfo.end(); ++it)
        {
            if (it->account == pPersonInfo->account)
            {
                it->name = pPersonInfo->name;
                it->authority = pPersonInfo->authority;
                it->name = pPersonInfo->name;
                it->pwd = pPersonInfo->pwd;
                it->tel = pPersonInfo->tel;
                return TRUE;
            }
        }
    }
    break;
    case 1: // ����ά����Ա
    {
        vector<ROOM_REPAIR_INFO>::iterator it;

        for (it = m_personInfo.vecRoomRepairInfo.begin();
            it != m_personInfo.vecRoomRepairInfo.end(); ++it)
        {
            if (it->account == pPersonInfo->account)
            {
                it->name = pPersonInfo->name;
                it->authority = pPersonInfo->authority;
                it->name = pPersonInfo->name;
                it->pwd = pPersonInfo->pwd;
                it->tel = pPersonInfo->tel;
                return TRUE;
            }
        }
    }
    break;
    case 2: // ��ʦ
    {
        vector<TEACHER_INFO>::iterator it;

        for (it = m_personInfo.vecTeacherInfo.begin();
            it != m_personInfo.vecTeacherInfo.end(); ++it)
        {
            if (it->account == pPersonInfo->account)
            {
                it->name = pPersonInfo->name;
                it->authority = pPersonInfo->authority;
                it->name = pPersonInfo->name;
                it->pwd = pPersonInfo->pwd;
                it->tel = pPersonInfo->tel;
                return TRUE;
            }
        }
    }
    break;
    default:
        break;
    }

    return FALSE;
}

LRESULT CPersonInfo::OnDelListItem(WPARAM wParam, LPARAM lParam)
{
    PERSON_INFO_PART *pPersonInfo = (PERSON_INFO_PART *)wParam;

    int sel = m_comboRole.GetCurSel();

    switch (sel)
    {
    case 0: // ��������Ա
    {
        vector<ROOM_MANAGER_INFO>::iterator it;

        int i = 1;
        for (it = m_personInfo.vecRoomManagerInfo.begin();
            it != m_personInfo.vecRoomManagerInfo.end(); )
        {
            
            if (it->account == pPersonInfo->account)
                it = m_personInfo.vecRoomManagerInfo.erase(it);    //ɾ��Ԫ�أ�����ֵָ����ɾ��Ԫ�ص���һ��λ��    
            else
                ++it;    //ָ����һ��λ�� 
            if (it != m_personInfo.vecRoomManagerInfo.end())
                it->id = i++;
        }
    }
    break;
    case 1: // ����ά����Ա
    {
        vector<ROOM_REPAIR_INFO>::iterator it;

        int i = 1;
        for (it = m_personInfo.vecRoomRepairInfo.begin();
            it != m_personInfo.vecRoomRepairInfo.end();)
        {

            if (it->account == pPersonInfo->account)
                it = m_personInfo.vecRoomRepairInfo.erase(it);    //ɾ��Ԫ�أ�����ֵָ����ɾ��Ԫ�ص���һ��λ��    
            else
                ++it;    //ָ����һ��λ�� 
            if (it != m_personInfo.vecRoomRepairInfo.end())
                it->id = i++;
        }
    }
    break;
    case 2: // ��ʦ
    {
        vector<TEACHER_INFO>::iterator it;

        int i = 1;
        for (it = m_personInfo.vecTeacherInfo.begin();
            it != m_personInfo.vecTeacherInfo.end();)
        {

            if (it->account == pPersonInfo->account)
                it = m_personInfo.vecTeacherInfo.erase(it);    //ɾ��Ԫ�أ�����ֵָ����ɾ��Ԫ�ص���һ��λ��    
            else
                ++it;    //ָ����һ��λ�� 
            //if (it != m_personInfo.vecTeacherInfo.end())
            //    it->id = i++;
        }
    }
    break;
    default:
        break;
    }

    return 0;
}

LRESULT CPersonInfo::OnAddListItem(WPARAM wParam, LPARAM lParam)
{
    ADD_PERSON_DATA* ptagAddPerson = (ADD_PERSON_DATA*)wParam;

    switch (ptagAddPerson->authority)
    {
    case MANAGER: // ��������Ա
    {
        ROOM_MANAGER_INFO roomManagerInfo;
        roomManagerInfo.account = ptagAddPerson->account;
        roomManagerInfo.pwd = ptagAddPerson->pwd;
        roomManagerInfo.name = ptagAddPerson->name;
        roomManagerInfo.tel = ptagAddPerson->tel;
        roomManagerInfo.id = m_personInfo.vecRoomManagerInfo.at(m_personInfo.vecRoomManagerInfo.size() - 1).id + 1;
        roomManagerInfo.authority = ptagAddPerson->authority;
        m_personInfo.vecRoomManagerInfo.push_back(roomManagerInfo);
    }
    break;
    case REPAIR: // ����ά����Ա
    {
        ROOM_REPAIR_INFO repairInfo;
        repairInfo.account = ptagAddPerson->account;
        repairInfo.pwd = ptagAddPerson->pwd;
        repairInfo.name = ptagAddPerson->name;
        repairInfo.tel = ptagAddPerson->tel;
        repairInfo.id = m_personInfo.vecRoomRepairInfo.at(m_personInfo.vecRoomRepairInfo.size() - 1).id + 1;
        repairInfo.authority = ptagAddPerson->authority;
        m_personInfo.vecRoomRepairInfo.push_back(repairInfo);
    }
    break;
    case TEACHER: // ��ʦ
    {
        TEACHER_INFO teacherInfo;
        teacherInfo.account = ptagAddPerson->account;
        teacherInfo.pwd = ptagAddPerson->pwd;
        teacherInfo.name = ptagAddPerson->name;
        teacherInfo.tel = ptagAddPerson->tel;
        //teacherInfo.id = m_personInfo.vecTeacherInfo.at(m_personInfo.vecTeacherInfo.size() - 1).id + 1;
        teacherInfo.authority = ptagAddPerson->authority;
        m_personInfo.vecTeacherInfo.push_back(teacherInfo);
    }
    break;
    //case STUDENT:
    //{
    //    STUDETN_INFO studentInfo;
    //    studentInfo.student_id = ptagAddPerson->account;
    //    studentInfo.pwd = ptagAddPerson->pwd;
    //    studentInfo.name = ptagAddPerson->name;
    //    studentInfo.tel = ptagAddPerson->tel;
    //    studentInfo.classes = ptagAddPerson->grade;
    //    studentInfo.major = ptagAddPerson->major;
    //    studentInfo.authority = ptagAddPerson->authority;
    //    //studentInfo.id = m_personInfo.vecStudentInfo.at(m_personInfo.vecStudentInfo.size() - 1).id + 1;
    //    studentInfo.authority = ptagAddPerson->authority;
    //    m_personInfo.vecStudentInfo.push_back(studentInfo);
    //}
    //break;
    default:
        break;
    }
    m_listPersonInfo.Update(m_listPersonInfo.GetItemCount());

    return 0;
}


BOOL CPersonInfo::PreTranslateMessage(MSG* pMsg)
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

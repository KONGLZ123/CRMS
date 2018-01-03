// ViewReigstRequestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ViewReigstRequestDlg.h"
#include "afxdialogex.h"


// CViewReigstRequestDlg �Ի���

IMPLEMENT_DYNAMIC(CViewReigstRequestDlg, CDialogEx)

CViewReigstRequestDlg::CViewReigstRequestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_VIEW_REGISTER, pParent)
{
    m_pageIndex = 0;
}

CViewReigstRequestDlg::~CViewReigstRequestDlg()
{
}

void CViewReigstRequestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CViewReigstRequestDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BTN_PAGE_UP, &CViewReigstRequestDlg::OnBnClickedBtnPageUp)
    ON_BN_CLICKED(IDC_BTN_PAGE_DOWN, &CViewReigstRequestDlg::OnBnClickedBtnPageDown)
    ON_BN_CLICKED(IDC_BTN_OK, &CViewReigstRequestDlg::OnBnClickedBtnOk)
    ON_BN_CLICKED(IDC_BTN_REGIESTER, &CViewReigstRequestDlg::OnBnClickedRefuse)
END_MESSAGE_MAP()


// CViewReigstRequestDlg ��Ϣ�������
BOOL CViewReigstRequestDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    if (0 != m_vecRegisterData.size()) {
        SetEditText(m_vecRegisterData.at(0));
        m_registerData = m_vecRegisterData.at(0);
    }
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // �쳣: OCX ����ҳӦ���� FALSE
}

void CViewReigstRequestDlg::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}

void CViewReigstRequestDlg::SetRegisterData(vector<ADD_PERSON_DATA>& vecRegisterData)
{
    m_vecRegisterData = vecRegisterData;
}

void CViewReigstRequestDlg::OnBnClickedBtnPageUp()
{
    if (0 == m_pageIndex)
    {
        ::MessageBox(this->m_hWnd, _T("������ǰҳ"), _T("��ʾ"), MB_OK);
        return;
    }

    m_pageIndex--;
    m_registerData = m_vecRegisterData.at(m_pageIndex);

    SetEditText(m_vecRegisterData.at(m_pageIndex));
}


void CViewReigstRequestDlg::OnBnClickedBtnPageDown()
{
    int size = m_vecRegisterData.size();
    if (m_pageIndex >= size - 1)
    {
        ::MessageBox(this->m_hWnd, _T("������ĩҳ"), _T("��ʾ"), MB_OK);
        return;
    }
    m_pageIndex++;
    m_registerData = m_vecRegisterData.at(m_pageIndex);

    SetEditText(m_vecRegisterData.at(m_pageIndex));
}

void CViewReigstRequestDlg::SetEditText(ADD_PERSON_DATA personData)
{
    CString strText;

    strText = _T("�˺ţ�") + personData.account + _T("\r\n\r\n");
    strText += _T("���룺") + personData.pwd + _T("\r\n\r\n");
    strText += _T("������") + personData.name + _T("\r\n\r\n");
    strText += _T("�绰��") + personData.tel + _T("\r\n\r\n");
    switch (personData.authority)
    {
    case MANAGER:
        strText += _T("��ɫ����������Ա\r\n \r\n");
        break;
    case REPAIR:
        strText += _T("��ɫ������ά����Ա\r\n \r\n");
        break;
    case TEACHER:
        strText += _T("��ɫ����ʦ\r\n \r\n");
        break;
    case STUDENT:
        strText += _T("��ɫ��ѧ��\r\n \r\n");
        break;
    default:
        strText += _T("��ɫ��δ֪��ɫ\r\n \r\n");
        break;
    }

    GetDlgItem(IDC_EDIT_REGISTER)->SetWindowText(strText);
}


void CViewReigstRequestDlg::OnBnClickedBtnOk()
{
    // ����Ӧ���������
    ADD_PERSON_DATA *ptagAddPerson = new ADD_PERSON_DATA;
    ptagAddPerson->account = m_registerData.account;
    ptagAddPerson->pwd = m_registerData.pwd;
    ptagAddPerson->name = m_registerData.name;
    ptagAddPerson->tel = m_registerData.tel;
    ptagAddPerson->authority = m_registerData.authority;
    ptagAddPerson->bIsRegister = FALSE;

    if (STUDENT == ptagAddPerson->authority) {
        ptagAddPerson->id = m_registerData.id;
        ptagAddPerson->grade = m_registerData.grade;
        ptagAddPerson->major = m_registerData.major;
    }

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_ADD_PERSON, (WPARAM)ptagAddPerson, (LPARAM)m_hWnd);

    // ɾ��ע������������
    OnBnClickedRefuse();
}


void CViewReigstRequestDlg::OnBnClickedRefuse()
{
    CString* pStr = new CString();
    *pStr = m_registerData.account;
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_DEL_REGISTER, (WPARAM)pStr, 0);

    vector<ADD_PERSON_DATA>::iterator ite;
    for (ite = m_vecRegisterData.begin(); ite != m_vecRegisterData.end(); )
    {
        if (ite->account == m_registerData.account)
        {
            ite = m_vecRegisterData.erase(ite);
            GetDlgItem(IDC_EDIT_REGISTER)->SetWindowText(_T("�������Ѵ���"));
            //m_pageIndex--;
            break;
        }
        else
        {
            ++ite;
        }
    }
}




BOOL CViewReigstRequestDlg::PreTranslateMessage(MSG* pMsg)
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

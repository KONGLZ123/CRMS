// ViewReigstRequestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "ViewReigstRequestDlg.h"
#include "afxdialogex.h"


// CViewReigstRequestDlg 对话框

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


// CViewReigstRequestDlg 消息处理程序
BOOL CViewReigstRequestDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    if (0 != m_vecRegisterData.size()) {
        SetEditText(m_vecRegisterData.at(0));
        m_registerData = m_vecRegisterData.at(0);
    }
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
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
        ::MessageBox(this->m_hWnd, _T("已是最前页"), _T("提示"), MB_OK);
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
        ::MessageBox(this->m_hWnd, _T("已是最末页"), _T("提示"), MB_OK);
        return;
    }
    m_pageIndex++;
    m_registerData = m_vecRegisterData.at(m_pageIndex);

    SetEditText(m_vecRegisterData.at(m_pageIndex));
}

void CViewReigstRequestDlg::SetEditText(ADD_PERSON_DATA personData)
{
    CString strText;

    strText = _T("账号：") + personData.account + _T("\r\n\r\n");
    strText += _T("密码：") + personData.pwd + _T("\r\n\r\n");
    strText += _T("姓名：") + personData.name + _T("\r\n\r\n");
    strText += _T("电话：") + personData.tel + _T("\r\n\r\n");
    switch (personData.authority)
    {
    case MANAGER:
        strText += _T("角色：机房管理员\r\n \r\n");
        break;
    case REPAIR:
        strText += _T("角色：机房维护人员\r\n \r\n");
        break;
    case TEACHER:
        strText += _T("角色：教师\r\n \r\n");
        break;
    case STUDENT:
        strText += _T("角色：学生\r\n \r\n");
        break;
    default:
        strText += _T("角色：未知角色\r\n \r\n");
        break;
    }

    GetDlgItem(IDC_EDIT_REGISTER)->SetWindowText(strText);
}


void CViewReigstRequestDlg::OnBnClickedBtnOk()
{
    // 向相应表插入数据
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

    // 删除注册表里面的数据
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
            GetDlgItem(IDC_EDIT_REGISTER)->SetWindowText(_T("此请求已处理"));
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

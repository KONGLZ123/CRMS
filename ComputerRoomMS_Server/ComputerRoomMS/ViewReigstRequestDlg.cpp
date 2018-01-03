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
    ON_BN_CLICKED(IDC_BTN_REGIESTER, &CViewReigstRequestDlg::OnBnClickedBtnRegiester)
END_MESSAGE_MAP()


// CViewReigstRequestDlg 消息处理程序
BOOL CViewReigstRequestDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // TODO:  在此添加额外的初始化
    SetEditText(m_vecRegisterData.at(0));

    return TRUE;  // return TRUE unless you set the focus to a control
                  // 异常: OCX 属性页应返回 FALSE
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

    strText = _T("姓名：") + personData.name + _T("\r\n");
    strText += _T("账号：") + personData.account + _T("\r\n");
    strText += _T("密码：") + personData.pwd + _T("\r\n");
    strText += _T("电话：") + personData.tel + _T("\r\n");
    switch (personData.authority)
    {
    case MANAGER:
        strText += _T("角色： 机房管理员\r\n \r\n");
        break;
    case REPAIR:
        strText += _T("角色： 机房维护人员\r\n \r\n");
        break;
    case TEACHER:
        strText += _T("角色： 教师\r\n \r\n");
        break;
    case STUDENT:
        strText += _T("角色： 学生\r\n \r\n");
        break;
    default:
        strText += _T("角色： 未知角色\r\n \r\n");
        break;
    }

    GetDlgItem(IDC_EDIT_REGISTER)->SetWindowText(strText);
}


void CViewReigstRequestDlg::OnBnClickedBtnOk()
{
    //m_registerData
}


void CViewReigstRequestDlg::OnBnClickedBtnRegiester()
{
    //PostThreadMessage(m_pDbThread->m_nThreadID, WM_DEL_ANNOUNCE, (WPARAM)m_pageIndex + 1, (LPARAM)m_hWnd);

    //vector<ANNOUNCE_DATA>::iterator ite;
    //for (ite = m_vecRegisterData.begin(); ite != m_vecRegisterData.end(); ++ite) {
    //    if (ite->announce_id == m_pageIndex + 1)
    //    {
    //        ite->strText = _T("公告已删除");
    //        GetDlgItem(IDC_EDIT_OLD_ANNOUNCE)->SetWindowText(_T("公告已删除"));
    //    }
    //}
}



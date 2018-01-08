
// ComputerRoomMSDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "DlgMain.h"
#include "ServerSetupDlg.h"

#include "registerDlg.h"

//class CServerSetupDlg;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CComputerRoomMSDlg �Ի���


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOGIN, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_editAccount.m_hWnd = NULL;
    m_editPwd.m_hWnd = NULL;
    m_comboRole.m_hWnd = NULL;
    m_pThreadDatabase = NULL;
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_STATIC_LOGO, m_staticLogo);
    DDX_Control(pDX, IDC_STATIC_REGISTER, m_linkCtrlRegister);
    DDX_Control(pDX, IDC_STATIC_SERVER_SETUP, m_linkCtrlSetup);
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BTN_LOGIN, &CLoginDlg::OnBnClickedLogin)
    ON_MESSAGE(WM_SHOWLOGIN, &CLoginDlg::OnShowLoginDlg)
    ON_WM_CTLCOLOR()
    ON_WM_CLOSE()
    ON_MESSAGE(WM_LOGIN_SUCCESS, &CLoginDlg::OnLoginSuccess)
    ON_MESSAGE(WM_LOGIN_ERROR, &CLoginDlg::OnLoginError)
    //ON_MESSAGE(WM_RECV, &CLoginDlg::OnRecv)
    ON_STN_CLICKED(IDC_STATIC_REGISTER, &CLoginDlg::OnStnClickedStaticRegister)
    ON_STN_CLICKED(IDC_STATIC_SERVER_SETUP, &CLoginDlg::OnStnClickedStaticServerSetup)
END_MESSAGE_MAP()


// CComputerRoomMSDlg ��Ϣ�������

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

    //////////////////////////////////////////////////////////////////
	ShowWindow(SW_SHOW);
    CenterWindow();
    ModifyStyle(0, WS_MINIMIZEBOX);

    SetBackgroundColor(RGB(255, 255, 255));

    m_fontLogo.CreateFont(40, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("΢���ź�"));
    m_staticLogo.SetFont(&m_fontLogo);

    //m_fontRegister.CreateFont(18, 0, 0, 0, FW_THIN, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("΢���ź�"));
    m_linkCtrlRegister.SetTextColor(RGB(0, 122, 204), RGB(0, 122, 204));
    m_linkCtrlRegister.SetHandCursor(TRUE);
    //m_linkCtrlRegister.SetFont(&m_fontRegister);

    m_linkCtrlSetup.SetTextColor(RGB(0, 122, 204), RGB(0, 122, 204));
    m_linkCtrlSetup.SetHandCursor(TRUE);

    InitCtrls();
    //ConnectServer();
    InitDatabaseThread();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CLoginDlg::InitDatabaseThread()
{
    m_pThreadDatabase = new CWinThreadDatabase;
    if (!m_pThreadDatabase->InitThread())
    {
        MessageBox(_T("CLoginDlg::InitDatabaseThread() Failed"));
    }
}


void CLoginDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CLoginDlg::CheckEmpty(CString strAccount, CString strPwd)
{
    if (strAccount.IsEmpty())
    {
        GetDlgItem(IDC_STATIC_ACCOUNT)->SetWindowText(_T("* �������û���"));
        return FALSE;
    }

    if (strPwd.IsEmpty())
    {
        GetDlgItem(IDC_STATIC_PWD)->SetWindowText(_T("* ����������"));
        return FALSE;
    }

    return TRUE;
}

//BOOL CLoginDlg::ConnectServer()
//{
//    m_strIP = _T("127.0.0.1");
//    m_strPort = _T("6666");
//
//    if (!m_sock.Create())
//    {
//        CString strErr;
//        strErr.Format(_T("Socket create error : %d"), m_sock.GetLastError());
//        ::MessageBox(NULL, strErr, _T("������ʾ"), MB_OK);
//        return FALSE;
//    }
//
//    m_sock.SetHwnd(this->m_hWnd);
//
//    USES_CONVERSION;
//    int nPort = atoi(W2A(m_strPort));
//    if (!m_sock.Connect(m_strIP, nPort))
//    {
//        CString strErr;
//        strErr.Format(_T("Connect server error : %d"), m_sock.GetLastError());
//        ::MessageBox(NULL, strErr, _T("������ʾ"), MB_OK);
//        return FALSE;
//    }
//
//    return TRUE;
//}


void CLoginDlg::OnBnClickedLogin()
{
    CString strAccount;
    CString strPwd;
    GetDlgItem(IDC_EDIT_ACCOUNT)->GetWindowText(strAccount);
    GetDlgItem(IDC_EDIT_PWD)->GetWindowText(strPwd);

    GetDlgItem(IDC_STATIC_ACCOUNT)->SetWindowText(_T(""));
    GetDlgItem(IDC_STATIC_PWD)->SetWindowText(_T(""));
    GetDlgItem(IDC_STATIC_LOGIN_FAIL)->SetWindowText(_T(""));

    if (!CheckEmpty(strAccount, strPwd))  // �ж��û������������Ƿ�Ϊ��
        return;

    //if (!m_pThreadDatabase->ConnectServer())    // ���ӷ�����
    //    return;

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_ROLE);
    ASSERT(pComboBox);

    // ���͵�¼����
    //m_curStrAccount = strAccount;
    //SendLoginRequestToServer(strAccount, strPwd, pComboBox->GetCurSel());

    LOGIN_REQUEST *ptagLoginRequest = new LOGIN_REQUEST;
    ptagLoginRequest->strAccount = strAccount;
    ptagLoginRequest->strPwd = strPwd;
    ptagLoginRequest->role = pComboBox->GetCurSel();
    ptagLoginRequest->hWnd = m_hWnd;
    PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_LOGIN, (WPARAM)ptagLoginRequest, (LPARAM)m_hWnd);

    m_curStrAccount = strAccount;
    

    GetDlgItem(IDC_EDIT_ACCOUNT)->SetWindowText(_T(""));
    GetDlgItem(IDC_EDIT_PWD)->SetWindowText(_T(""));
}

LRESULT CLoginDlg::OnShowLoginDlg(WPARAM wParam, LPARAM lParam)
{
    ShowWindow(SW_SHOW);

    return 1;
}

void CLoginDlg::InitCtrls()
{
    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_ROLE);
    ASSERT(pComboBox);

    pComboBox->InsertString(ADMIN, _T("ϵͳ��̨����Ա"));
    pComboBox->InsertString(MANAGER, _T("��������Ա"));
    pComboBox->InsertString(REPAIR, _T("����ά����Ա"));
    pComboBox->InsertString(TEACHER, _T("��ʦ"));
    pComboBox->InsertString(STUDENT, _T("ѧ��"));
    pComboBox->SetCurSel(ADMIN);
}

HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

    if (pWnd->GetDlgCtrlID() == IDC_STATIC_ACCOUNT || 
        pWnd->GetDlgCtrlID() == IDC_STATIC_PWD || 
        pWnd->GetDlgCtrlID() == IDC_STATIC_LOGIN_FAIL)
    {
        pDC->SetTextColor(RGB(255, 0, 0));
    }

    return hbr;
}


void CLoginDlg::OnClose()
{
    if (NULL != m_pThreadDatabase)
    {
        delete m_pThreadDatabase;
        m_pThreadDatabase;
    }

    CDialogEx::OnClose();
}


LRESULT CLoginDlg::OnLoginSuccess(WPARAM wParam, LPARAM lParam)
{
    //CString *pStr = (CString *)wParam;
    //delete pStr;
    //pStr = NULL;

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_ROLE);
    ASSERT(pComboBox);

    CDlgMain m_dlgMain;

    m_dlgMain.SetAccount(m_curStrAccount);
    m_dlgMain.SetThreadDatabase(m_pThreadDatabase);
    m_dlgMain.SetRole(pComboBox->GetCurSel());

    ShowWindow(SW_HIDE);
    m_dlgMain.DoModal();

    return 0;
}

LRESULT CLoginDlg::OnLoginError(WPARAM wParam, LPARAM lParam)
{
    GetDlgItem(IDC_STATIC_LOGIN_FAIL)->SetWindowText(_T("�û������������"));

    return 0;
}
//
//LRESULT CLoginDlg::OnRecv(WPARAM wParam, LPARAM lParam)
//{
//    CString *pStr = (CString *)wParam;
//
//    USES_CONVERSION;
//    char *pBuf = W2A(*pStr);
//    PraseJson(pBuf);
//
//    delete pStr;
//    pStr = NULL;
//
//    return 0;
//}

void CLoginDlg::OnStnClickedStaticRegister()
{
    CRegisterDlg registerDlg;

    registerDlg.SetTitleAndBtn(_T("���û������˺�"), _T("����"));
    registerDlg.SetThreadDatabase(m_pThreadDatabase);
    registerDlg.DoModal();
}
//
//BOOL CLoginDlg::SendMsgToServer(Json::Value root)
//{
//    // ��ʽ����json����
//    Json::FastWriter writer;
//    std::string out = writer.write(root);
//    const char * str = out.c_str();
//
//    m_sock.SendMsgToServer((LPSTR)str, strlen(str));
//                           
//    return TRUE;
//}
//
//BOOL CLoginDlg::SendLoginRequestToServer(CString strAccount, CString strPwd, int role)
//{
//    USES_CONVERSION;
//    char *pAccount = W2A(strAccount);
//    char *pPwd = W2A(strPwd);
//
//    Json::Value root;
//    root["code"] = S_CODE_LOGIN;
//    root["account"] = pAccount;
//    root["pwd"] = pPwd;
//    root["role"] = role;
//
//    if (!SendMsgToServer(root))
//        return FALSE;
//
//    return TRUE;
//}
//
//void CLoginDlg::PraseJson(char * buf)
//{
//    Json::Reader reader;
//    Json::Value root;
//    if (reader.parse(buf, root))  // reader��Json�ַ���������root��root������Json��������Ԫ��   
//    {
//        int code = root["code"].asInt();  // ���ʽڵ㣬upload_id = "UP000000"   
//        switch (code)
//        {
//        case S_CODE_LOGIN_SUCCESS:  // ��¼
//        { 
//            this->ShowWindow(SW_HIDE);
//
//            CDlgMain dlg;
//
//            CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_ROLE);
//            ASSERT(pComboBox);
//
//            dlg.SetAccount(m_curStrAccount);
//            //dlg.SetThreadDatabase(m_pThreadDatabase);
//            dlg.SetRole(pComboBox->GetCurSel());
//            
//            dlg.DoModal();
//        }
//        break;
//        case S_CODE_LOGIN_FAILED:
//        {
//            GetDlgItem(IDC_STATIC_LOGIN_FAIL)->SetWindowText(_T("�û������������"));
//        }
//        break;
//        default:
//            break;
//        }
//    }
//}
//


void CLoginDlg::OnStnClickedStaticServerSetup()
{
    CServerSetupDlg dlg;
    dlg.SetThreadDatabase(m_pThreadDatabase);
    dlg.DoModal();
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_ESCAPE)    //����ESC  
            return TRUE;

        if (pMsg->wParam == VK_RETURN)    //���λس� 
        {
            OnBnClickedLogin();
            return TRUE;
        }

        if (pMsg->wParam == VK_F1) 	//����F1������
            return TRUE;
    }

    if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) 	//����ALT+F4
        return TRUE;

    return CDialogEx::PreTranslateMessage(pMsg);
}

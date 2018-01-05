s
// ComputerRoomMSDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "DlgMain.h"

#include "registerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CComputerRoomMSDlg 对话框


CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOGIN, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pThreadDatabase = NULL;
    m_connect = false;
    m_port = 6666;
    m_userCount = 0;
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_LOG, m_event);
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
    ON_MESSAGE(WM_INITDATA_SUCCESS, OnInitDataSuccess)
    ON_MESSAGE(WM_INITDATA_FAILED, OnInitDataFailed)
    ON_STN_CLICKED(IDC_STATIC_REGISTER, &CLoginDlg::OnStnClickedStaticRegister)
    ON_BN_CLICKED(IDC_BTN_SWITCH, &CLoginDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CComputerRoomMSDlg 消息处理程序

BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

    //////////////////////////////////////////////////////////////////
	ShowWindow(SW_SHOW);
    CenterWindow();
    ModifyStyle(0, WS_MINIMIZEBOX);

    InitDatabaseThread();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CLoginDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CLoginDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CLoginDlg::CheckEmpty(CString strAccount, CString strPwd)
{
    if (strAccount.IsEmpty())
    {
        GetDlgItem(IDC_STATIC_ACCOUNT)->SetWindowText(_T("* 请输入用户名"));
        return FALSE;
    }

    if (strPwd.IsEmpty())
    {
        GetDlgItem(IDC_STATIC_PWD)->SetWindowText(_T("* 请输入密码"));
        return FALSE;
    }

    return TRUE;
}

void CLoginDlg::InitDatabaseThread()
{
    m_pThreadDatabase = new CWinThreadDatabase;
    if (!m_pThreadDatabase->InitThread())
    {
        MessageBox(_T("数据库线程启动失败！"));
    }
}


void CLoginDlg::OnBnClickedLogin()
{
    CString strAccount;
    CString strPwd;
    GetDlgItem(IDC_EDIT_ACCOUNT)->GetWindowText(strAccount);
    GetDlgItem(IDC_EDIT_PWD)->GetWindowText(strPwd);

    GetDlgItem(IDC_STATIC_ACCOUNT)->SetWindowText(_T(""));
    GetDlgItem(IDC_STATIC_PWD)->SetWindowText(_T(""));
    GetDlgItem(IDC_STATIC_LOGIN_FAIL)->SetWindowText(_T(""));

    //if (!CheckEmpty(strAccount, strPwd))  // 判断用户名密码输入是否为空
    //    return;

    //if (!ConnectServer())    // 连接服务器
    //    return;

    //OnLoginSuccess(0, 0);

    CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_ROLE);
    ASSERT(pComboBox);

    LOGIN_REQUEST *ptagLoginRequest = new LOGIN_REQUEST;
    ptagLoginRequest->strAccount = strAccount;
    ptagLoginRequest->strPwd = strPwd;
    ptagLoginRequest->role = pComboBox->GetCurSel();
    ptagLoginRequest->hWnd = m_hWnd;
    PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_LOGIN, (WPARAM)ptagLoginRequest, (LPARAM)m_hWnd);

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

    pComboBox->InsertString(0, _T("系统后台管理员"));
    pComboBox->InsertString(1, _T("机房管理员"));
    pComboBox->InsertString(2, _T("机房维护人员"));
    pComboBox->InsertString(3, _T("教师"));
    pComboBox->InsertString(4, _T("学生"));
    pComboBox->SetCurSel(0);
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
    USES_CONVERSION;

    Json::Value root;
    root["code"] = S_CODE_LOGIN_SUCCESS;

    if (!SendMsgToClient(root))
        return FALSE;

    //CString *pStr = (CString *)wParam;
    //CString str = *pStr;
    //delete pStr;
    //pStr = NULL;

    //ShowWindow(SW_HIDE);
    //CDlgMain dlg;

    //CComboBox *pComboBox = (CComboBox *)GetDlgItem(IDC_COMBO_ROLE);
    //ASSERT(pComboBox);



    //dlg.SetAccount(str);
    //dlg.SetThreadDatabase(m_pThreadDatabase);
    //dlg.SetRole(pComboBox->GetCurSel());

    //dlg.DoModal();

    return TRUE;
}

LRESULT CLoginDlg::OnLoginError(WPARAM wParam, LPARAM lParam)
{
    //GetDlgItem(IDC_STATIC_LOGIN_FAIL)->SetWindowText(_T("用户名或密码错误"));
    Json::Value root;
    root["code"] = S_CODE_LOGIN_FAILED;

    if (!SendMsgToClient(root))
        return FALSE;

    return 0;
}

LRESULT CLoginDlg::OnInitDataSuccess(WPARAM wParam, LPARAM lParam)
{
    //return LRESULT();
    //CString *pStr = (CString *)wParam;

    Json::Value *pRoot = (Json::Value *)wParam;
    //*pRoot = root;
    //SendMSG(*pStr);

    SendMsgToClient(*pRoot);

    delete pRoot;
    pRoot = NULL;

    return 0;
}

LRESULT CLoginDlg::OnInitDataFailed(WPARAM wParam, LPARAM lParam)
{
    return LRESULT();
}


void CLoginDlg::OnStnClickedStaticRegister()
{
    CRegisterDlg registerDlg;

    registerDlg.SetTitleAndBtn(_T("新用户申请账号"), _T("申请"));
    registerDlg.SetThreadDatabase(m_pThreadDatabase);
    registerDlg.DoModal();
}


// 启动服务器
void CLoginDlg::OnBnClickedButton1()
{
    if (m_connect)
    {
        delete listenSocket;
        listenSocket = NULL;
        m_connect = false;
        SetDlgItemText(IDC_BTN_SWITCH, _T("打开服务器"));
        UpdateEvent(_T("系统关闭服务器."));
        return;
    }
    listenSocket = new CSocketSrv();
    listenSocket->m_pDlg = this;
    // 指定对话框为主对话框，不能少了这句
    UpdateData(true);
    if (!listenSocket->Create(m_port))  // 创建服务器的套接字，IP地址默认本机IP
    {
        AfxMessageBox(_T("创建套接字错误！"));
        listenSocket->Close();
        return;
    }
    if (!listenSocket->Listen())
    {
        AfxMessageBox(_T("监听失败！"));
        listenSocket->Close();
        return;
    }
    m_connect = true;
    SetDlgItemText(IDC_BTN_SWITCH, _T("关闭服务器"));
    UpdateEvent(_T("系统打开服务器."));
}

void CLoginDlg::AddClient()
{
    CSocketSrv *pSocket = new CSocketSrv;
    pSocket->m_pDlg = this;
    listenSocket->Accept(*pSocket);
    pSocket->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE);
    m_clientList.AddTail(pSocket);
    m_userCount = m_clientList.GetCount();
    UpdateData(false);
    UpdateEvent(_T("用户连接服务器."));
    //SendMSG(_T("Hello!"));
}

void CLoginDlg::RemoveClient(CSocketSrv * pSocket)
{
    POSITION nPos = m_clientList.GetHeadPosition();
    POSITION nTmpPos = nPos;
    while (nPos)
    {
        CSocketSrv* pSockItem = (CSocketSrv*)m_clientList.GetNext(nPos);
        if (pSockItem->m_hSocket == pSocket->m_hSocket)
        {
            pSockItem->Close();
            delete pSockItem;
            m_clientList.RemoveAt(nTmpPos);
            m_userCount = m_clientList.GetCount();
            UpdateData(false);
            UpdateEvent(_T("用户离开."));
            return;
        }
        nTmpPos = nPos;
    }
}

void CLoginDlg::RecvData(CSocketSrv * pSocket)
{
    char* pData = NULL;
    pData = new char[1024];
    memset(pData, 0, sizeof(char) * 1024);
    UCHAR leng = 0;
    CString str;
    if (pSocket->Receive(pData, 1024, 0) != SOCKET_ERROR)
    {
        str = pData;
        PraseJson(pData);
        m_pRemoteSocket = pSocket;
        //SendMSG(str);     // 转发数据给所有用户，包括发送数据的用户
    }
    delete pData;
    pData = NULL;
}

void CLoginDlg::UpdateEvent(CString str)
{
    CString string;
    CTime time = CTime::GetCurrentTime();
    // 获取系统当前时间
    str += _T("\r\n");
    // 用于换行显示日志
    string = time.Format(_T("%Y/%m/%d %H:%M:%S  ")) + str;
    // 格式化当前时间
    int lastLine = m_event.LineIndex(m_event.GetLineCount() - 1);
    //获取编辑框最后一行索引
    m_event.SetSel(lastLine + 1, lastLine + 2, 0);
    //选择编辑框最后一行
    m_event.ReplaceSel(string);                                                             //替换所选那一行的内容
}

BOOL CLoginDlg::WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen)
{
    int n = 0;
    n = WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, 0, 0, FALSE);
    if (n<nlen)return FALSE;
    WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, nlen, 0, FALSE);
    return TRUE;
}

void CLoginDlg::SendMSG(CString str)
{
    char *pSend = new char[str.GetLength()];
    memset(pSend, 0, str.GetLength() * sizeof(char));
    if (!WChar2MByte(str.GetBuffer(0), pSend, str.GetLength()))
    {
        AfxMessageBox(_T("字符转换失败"));
        delete pSend;
        return;
    }
    POSITION nPos = m_clientList.GetHeadPosition();
    while (nPos)
    {
        CSocketSrv* pTemp = (CSocketSrv*)m_clientList.GetNext(nPos);
        pTemp->Send(pSend, str.GetLength());
    }
    delete pSend;
}

void CLoginDlg::PraseJson(char * buf)
{
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(buf, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素   
    {
        int code = root["code"].asInt();  // 访问节点，upload_id = "UP000000"   
        switch (code)
        {
        case S_CODE_LOGIN:  // 登录
            {
                LOGIN_REQUEST *ptagLoginRequest = new LOGIN_REQUEST;
                ptagLoginRequest->strAccount = root["account"].asCString();
                ptagLoginRequest->strPwd = root["pwd"].asCString();
                ptagLoginRequest->role = root["role"].asInt();
                ptagLoginRequest->hWnd = m_hWnd;
                PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_LOGIN, (WPARAM)ptagLoginRequest, (LPARAM)m_hWnd);
            }
            break;
        case S_CODE_INIT_DATA:
            {
                int role = root["role"].asInt();
                PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_INIT_DATA, role, (LPARAM)m_hWnd);
            }
            break;
        case S_CODE_INSERT_NEW_ACCOUNT: {
            ADD_PERSON_DATA * ptagAddPerson = new ADD_PERSON_DATA;

            ptagAddPerson->account = root["account"].asCString();
            ptagAddPerson->pwd = root["pwd"].asCString();
            ptagAddPerson->authority = root["role"].asInt();
            ptagAddPerson->bIsRegister = root["is_register"].asInt();
            ptagAddPerson->grade = root["grade"].asCString();
            ptagAddPerson->major = root["major"].asCString();
            //ptagAddPerson->id = root["stu_id"].asCString();
            ptagAddPerson->name = root["name"].asCString();
            ptagAddPerson->tel = root["tel"].asCString();
            ptagAddPerson->course = root["course"].asCString();

            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_ADD_PERSON, (WPARAM)ptagAddPerson, 0);
            break;
        }
        case S_CODE_INSERT_REPORT:
        {
            REPORT_DATA *pReportData = new REPORT_DATA;

            pReportData->submitPerson = root["submit_person"].asCString();
            pReportData->roomId = root["room_id"].asCString();
            pReportData->reason = root["reason"].asCString();
            pReportData->uploadDate = root["upload_date"].asCString();
            pReportData->notes = root["notes"].asCString();
            pReportData->reportType = (REPORT_TYPE)root["type"].asInt();
            pReportData->isView = root["is_view"].asInt();

            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_REQUEST_REPAIR, (WPARAM)pReportData, 0);
        }
        break;
        case S_CODE_INSERT_ANNOUNCE:
        {
            ANNOUNCE_DATA *pAnnouce = new ANNOUNCE_DATA;
            pAnnouce->type = root["type"].asInt();
            pAnnouce->strText = root["text"].asCString();
            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_DECLARE_ANNOUNCE, (WPARAM)pAnnouce, 0);
        }
        break;
        case S_CODE_UPDATE_ADMIN_PWD:
        {
            CString *pStrPwd = new CString(root["pwd"].asCString());
            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_ADMIN_SETUP_PWD, (WPARAM)pStrPwd, 0);
        }
        break;
        case S_CODE_UPDATE_PERSON_PWD:
        {
            UPDATE_PERSON_PWD *pPersonPwd = new UPDATE_PERSON_PWD;

            pPersonPwd->newPwd = root["pwd"].asCString();
            pPersonPwd->account = root["account"].asCString();
            pPersonPwd->authrity = root["authority"].asInt();

            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_SETUP_PWD, (WPARAM)pPersonPwd, 0);
        }
        break;
        case S_CODE_UPDATE_PERSON_INFO:
        {
            PERSON_INFO_PART *pPersonInfo = new PERSON_INFO_PART;
            pPersonInfo->account = root["account"].asCString();
            pPersonInfo->pwd = root["pwd"].asCString();
            pPersonInfo->name = root["name"].asCString();
            pPersonInfo->tel = root["tel"].asCString();
            USES_CONVERSION;
            pPersonInfo->authority = root["authority"].asInt();
            pPersonInfo->oldAuthority = root["old_authority"].asInt();
            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_UPDATE_PERSON_INFO, (WPARAM)pPersonInfo, 0);
        }
        break;
        case S_CODE_DEL_PERSON_INFO:
        {
            PERSON_INFO_PART *pPersonInfo = new PERSON_INFO_PART;
            pPersonInfo->account = root["account"].asCString();
            pPersonInfo->authority = root["authority"].asInt();
            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_DEL_PERSON_INFO, (WPARAM)pPersonInfo, 0);
        }
        break;
        case S_CODE_DEL_REGISTER:
        {
            CString* pStr = new CString(root["account"].asCString());
            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_DEL_REGISTER, (WPARAM)pStr, 0);
        }
        break;
        case S_CODE_INSERT_EXAM_INFO:
        {
            EXAM_INFO* pExamInfo = new EXAM_INFO;
            pExamInfo->strText = root["text"].asCString();
            pExamInfo->strPerson = root["person"].asCString();
            pExamInfo->strDate = root["date"].asCString();
            pExamInfo->strClass = root["class"].asCString();

            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_DECALRE_EXAM_INFO, (WPARAM)pExamInfo, 0);
        }
        break;
        case S_CODE_INSERT_CLASS_INFO:
        {
            CLASS_DATA* pClassData = new CLASS_DATA;
            
            pClassData->week = root["week"].asInt();
            pClassData->day = root["day"].asInt();
            pClassData->class_num = root["class_num"].asInt();
            pClassData->class_text = root["class_text"].asCString();

            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_SET_CLASS_DATA, (WPARAM)pClassData, 0);
        }
        break;
        case S_CODE_INSERT_WORKLOAD:
        {
            WORKLOAD_REPORT* pReportData = new WORKLOAD_REPORT;

            pReportData->strName = root["name"].asCString();
            pReportData->strReason = root["reason"].asCString();
            pReportData->strDate = root["date"].asCString();
            pReportData->strFixSalary = root["fix_salary"].asCString();
            pReportData->strWorkloadSalary = root["workload_salary"].asCString();
            pReportData->isView = root["is_view"].asInt();

            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_INSERT_WORKLOAD_REPORT, (WPARAM)pReportData, 0);
        }
        break;
        case S_CODE_INSERT_ASSERT_DATA:
        {
            ASSERT_DATA *pReportData = new ASSERT_DATA;

            pReportData->strDeviceName = root["device_name"].asCString();
            pReportData->strOwner = root["owner"].asCString();
            pReportData->strRoomNum = root["room_num"].asCString();
            pReportData->strInDate = root["in_date"].asCString();
            pReportData->strOutDate = root["out_date"].asCString();
            pReportData->strStatus = root["status"].asCString();
            pReportData->strReason = root["reason"].asCString();
            pReportData->isView = root["is_view"].asInt();

            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_INSERT_ASSERT_DATA, (WPARAM)pReportData, 0);
        }
        break;
        case S_CODE_UPDATE_ROOM_REQUEST:
        {
            REPORT_DATA *pReportData = new REPORT_DATA;
            pReportData->submitPerson = root["submit_person"].asCString();
            pReportData->uploadDate = root["upload_date"].asCString();
            pReportData->isView = root["is_view"].asInt();

            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_UPDATE_ROOM_REQUEST, (WPARAM)pReportData, 0);
        }
        break;
        case S_CODE_UPDATE_EXAM_INFO:
        {
            ATTENDENCE_INFO *pAttendenceInfo = new ATTENDENCE_INFO;
            pAttendenceInfo->student_id = root["student_id"].asCString();
            pAttendenceInfo->attendece_cnt = root["attendece_cnt"].asInt();
            pAttendenceInfo->attendece_score = root["attendece_score"].asInt();

            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_UPDATE_EXAM_INFO, (WPARAM)pAttendenceInfo, 0);
        }
        break;
        case S_CODE_UPDATE_SALARY_INFO:
        {
            WORKLOAD_REPORT *pWorkload = new WORKLOAD_REPORT;
            USES_CONVERSION;
            pWorkload->strName = root["name"].asCString();
            pWorkload->strDate = root["date"].asCString();
            pWorkload->strWorkloadSalary = root["workload"].asCString();
            pWorkload->isView = root["is_view"].asInt();

            PostThreadMessage(m_pThreadDatabase->m_nThreadID, WM_UPDATE_SALARY_INFO, (WPARAM)pWorkload, 0);
        }
        break;
        default:
            break;
        }
    }
}

BOOL CLoginDlg::SendMsgToClient(Json::Value root)
{
    // 格式化成json数据
    Json::FastWriter writer;
    std::string out = writer.write(root);
    const char * str = out.c_str();

    if (*m_pRemoteSocket != SOCKET_ERROR)
        m_pRemoteSocket->Send(str, strlen(str));
    //m_sock.SendMsgToServer((LPSTR)str, strlen(str));

    return TRUE;
}


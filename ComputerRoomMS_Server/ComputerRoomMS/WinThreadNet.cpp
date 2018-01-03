// WinThreadLog.cpp : 实现文件
//

#include "stdafx.h"
#include "WinThreadNet.h"
#include "json.h"

VOID CALLBACK TimerProc(HWND hwnd,         // handle to window
    UINT uMsg,         // WM_TIMER message  
    UINT_PTR idEvent,  // timer identifier
    DWORD dwTime       // current system time
    )
{
    CWinThreadNet *pThread = (CWinThreadNet *)hwnd;
    ASSERT(NULL != pThread);

    pThread->SendHeartBeatMsgToServer();
}

// CWinThreadLog

IMPLEMENT_DYNCREATE(CWinThreadNet, CWinThread)

CWinThreadNet::CWinThreadNet()
{
}

CWinThreadNet::~CWinThreadNet()
{
}

BOOL CWinThreadNet::InitInstance()
{
	
	return TRUE;
}

int CWinThreadNet::ExitInstance()
{
    if (INVALID_SOCKET != m_sock)
        closesocket(m_sock);

	return CWinThread::ExitInstance();
}

void CWinThreadNet::OnLogin(WPARAM wParam, LPARAM lParam)
{
    CString *pStr = (CString *)wParam;
    
    SendLoginMsgToServer(*pStr);
    
    delete pStr;
    pStr = NULL;
}

void CWinThreadNet::OnSendLog(WPARAM wParam, LPARAM lParam)
{
    CString *pStr = (CString *)wParam;

    SendLogMsgToServer(*pStr);

    delete pStr;
    pStr = NULL;
}

void CWinThreadNet::OnQuit(WPARAM wParam, LPARAM lParam)
{
    SendLogoutMsgToServer();
}

BEGIN_MESSAGE_MAP(CWinThreadNet, CWinThread)
    ON_THREAD_MESSAGE(WM_LOG_LOGIN, OnLogin)
    ON_THREAD_MESSAGE(WM_LOG_SEND, OnSendLog)
    ON_THREAD_MESSAGE(WM_LOG_QUIT, OnQuit)
END_MESSAGE_MAP()


// CWinThreadLog 消息处理程序
BOOL CWinThreadNet::InitSocket()
{
    if (!AfxSocketInit())
        ::MessageBox(NULL, _T("Failed to Initialize Sockets"), _T("错误"), MB_OK);

    m_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (INVALID_SOCKET == m_sock)
    {
        int errCode = WSAGetLastError();
        ::MessageBox(NULL, _T("socket failed!"), _T("Shit"), MB_OK);
        return FALSE;
    }

    //SOCKADDR_IN sockAddr;
    //sockAddr.sin_addr.s_addr = htonl(INADDR_ANY); // 与本机绑定
    //sockAddr.sin_family = AF_INET;
    //sockAddr.sin_port = htons(6666);

    //int nRet = bind(m_sock, (SOCKADDR *)&sockAddr, sizeof(SOCKADDR));
    //if (SOCKET_ERROR == nRet)
    //{
    //    int errCode = WSAGetLastError();
    //    ::MessageBox(NULL, _T("sendto failed!"), _T("Shit"), MB_OK);
    //    return FALSE;
    //}

    m_addrServer.sin_addr.S_un.S_addr = inet_addr("192.168.145.128");
    m_addrServer.sin_family = AF_INET;
    m_addrServer.sin_port = htons(6666);

    return TRUE;
}

BOOL CWinThreadNet::InitThread()
{
    if (CreateThread())
    {
        if (!InitSocket())
            return FALSE;

        return TRUE;
    }

    return FALSE;
}

BOOL CWinThreadNet::SendMsgToServer(Json::Value root)
{
    // 格式化成json数据
    Json::FastWriter writer;
    std::string out = writer.write(root);
    const char * str = out.c_str();

    int nRet = sendto(m_sock, str, strlen(str), 0, (SOCKADDR *)&m_addrServer, sizeof(SOCKADDR));
    if (SOCKET_ERROR == nRet)
    {
        int errCode = WSAGetLastError();
        ::MessageBox(NULL, _T("sendto failed!"), _T("Shit"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadNet::SendHeartBeatMsgToServer()
{
    CTime tm = CTime::GetCurrentTime();
    CString strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));

    USES_CONVERSION;
    char *pTime = W2A(strTime);
    char *pAccount = W2A(m_strUID);

    Json::Value root;
    root["code"] = 0;
    root["uid"] = pAccount;
    root["user_status"] = "login";

    if (!SendMsgToServer(root))
        return FALSE;

    return TRUE;
}

BOOL CWinThreadNet::SendLoginMsgToServer(CString strUid)
{
    m_strUID = strUid;

    CTime tm = CTime::GetCurrentTime();
    m_strLoginTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));

    USES_CONVERSION;
    char *pTime = W2A(m_strLoginTime);
    char *pAccount = W2A(m_strUID);

    Json::Value root;
    root["code"] = 1;
    root["uid"] = pAccount;
    root["login_time"] = pTime;

    if (!SendMsgToServer(root))
        return FALSE;

    return TRUE;
}

BOOL CWinThreadNet::SendLogoutMsgToServer()
{
    USES_CONVERSION;
    char *pTime = W2A(m_strLoginTime);
    char *pAccount = W2A(m_strUID);

    Json::Value root;
    root["code"] = 2;
    root["uid"] = pAccount;
    root["login_time"] = pTime;

    if (!SendMsgToServer(root))
        return FALSE;

    return TRUE;
}

BOOL CWinThreadNet::SendLogMsgToServer(CString strLog)
{
    if (strLog.IsEmpty())
        return FALSE;

    USES_CONVERSION;
    char *pAccount = W2A(m_strUID);
    char *pTime = W2A(m_strLoginTime);
    char *pMsg = W2A(strLog);

    Json::Value root;
    root["code"] = 3;
    root["uid"] = pAccount;
    root["login_time"] = pTime;
    root["msg"] = pMsg;

    if (!SendMsgToServer(root))
        return FALSE;

    return TRUE;
}

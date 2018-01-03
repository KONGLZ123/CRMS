#pragma once
#include "afxsock.h"
#include "json.h"

#pragma comment(lib, "json_vc71_libmtd.lib")

#define WM_LOG_LOGIN        WM_USER + 1000
#define WM_LOG_SEND         WM_USER + 1001
#define WM_LOG_QUIT         WM_USER + 1002

// CWinThreadLog

class CWinThreadNet : public CWinThread
{
	DECLARE_DYNCREATE(CWinThreadNet)

public:
	CWinThreadNet();           // 动态创建所使用的受保护的构造函数
	virtual ~CWinThreadNet();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
    void OnLogin(WPARAM wParam, LPARAM lParam);
    void OnSendLog(WPARAM wParam, LPARAM lParam);
    void OnQuit(WPARAM wParam, LPARAM lParam);

public:
    BOOL InitThread();
    BOOL SendHeartBeatMsgToServer();
    BOOL SendLoginMsgToServer(CString strUid);
    BOOL SendLogMsgToServer(CString strLog);
    BOOL SendLogoutMsgToServer();
    BOOL SendMsgToServer(Json::Value root);

private:
    BOOL InitSocket();

protected:
	DECLARE_MESSAGE_MAP()

private:
    SOCKET          m_sock;
    CString         m_strUID;
    SOCKADDR_IN     m_addrServer;
    CString         m_strLoginTime;
};



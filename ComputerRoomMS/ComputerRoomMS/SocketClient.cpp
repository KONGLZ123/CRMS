// SocketClient.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "SocketClient.h"


// CSocketClient

CSocketClient::CSocketClient()
{
}

CSocketClient::~CSocketClient()
{
}


// CSocketClient 成员函数


void CSocketClient::OnReceive(int nErrorCode)
{
    char szBuf[60000];
    ZeroMemory(szBuf, 60000 - 1);
    int len = Receive(szBuf, 60000 - 1, 0);
    szBuf[len] = '\0';
   
    CString *pStr = new CString(szBuf);
    //::SendMessage(m_hWnd, WM_RECV, (WPARAM)pStr, 0);
    PostThreadMessage(m_pDbThread->m_nThreadID, WM_RECV, (WPARAM)pStr, 0);

    CSocket::OnReceive(nErrorCode);
}


BOOL CSocketClient::SendMsgToServer(LPSTR lpBuff, int nlen)
{
    //生成协议头
    if (Send(lpBuff, nlen) == SOCKET_ERROR)
    {
        AfxMessageBox(_T("发送错误！"));
        return FALSE;
    }
    return  TRUE;
}

void CSocketClient::SetHwnd(HWND hWnd)
{
    m_hWnd = hWnd;
}

void CSocketClient::SetThread(CWinThreadDatabase * pThread)
{
    m_pDbThread = pThread;
}

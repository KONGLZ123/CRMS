#pragma once
#include "WinThreadDatabase.h"

// CSocketClient ÃüÁîÄ¿±ê
class CWinThreadDatabase;

class CSocketClient : public CSocket
{
public:
	CSocketClient();
	virtual ~CSocketClient();
    virtual void OnReceive(int nErrorCode);
    BOOL SendMsgToServer(LPSTR lpBuff, int nlen);
    void SetHwnd(HWND hWnd);
    void SetThread(CWinThreadDatabase *pThread);

private:
    HWND m_hWnd;
    CWinThreadDatabase *m_pDbThread;
};



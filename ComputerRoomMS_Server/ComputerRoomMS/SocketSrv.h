#pragma once
#include "WinThreadDatabase.h"
#include "LoginDlg.h"

// CSocketSrv ����Ŀ��
class CLoginDlg;

class CSocketSrv : public CSocket
{
public:
	CSocketSrv();
	virtual ~CSocketSrv();
    virtual void OnReceive(int nErrorCode);
    virtual void OnAccept(int nErrorCode);
    virtual void OnClose(int nErrorCode);
    CLoginDlg *             m_pDlg;

    
};



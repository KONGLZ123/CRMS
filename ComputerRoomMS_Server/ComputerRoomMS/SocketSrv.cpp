// SocketSrv.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SocketSrv.h"


// CSocketSrv

CSocketSrv::CSocketSrv()
{
}

CSocketSrv::~CSocketSrv()
{
}


// CSocketSrv ��Ա����

void CSocketSrv::OnReceive(int nErrorCode)
{
    m_pDlg->RecvData(this);                           // ��������
    CSocket::OnReceive(nErrorCode);
}

void CSocketSrv::OnAccept(int nErrorCode)
{
    m_pDlg->AddClient();                                  //��������û�
    CSocket::OnAccept(nErrorCode);

}


void CSocketSrv::OnClose(int nErrorCode)
{
    m_pDlg->RemoveClient(this);                   // ɾ�������û�
    CSocket::OnClose(nErrorCode);
}

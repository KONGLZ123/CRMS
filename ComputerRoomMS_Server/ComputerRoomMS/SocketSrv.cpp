// SocketSrv.cpp : 实现文件
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


// CSocketSrv 成员函数

void CSocketSrv::OnReceive(int nErrorCode)
{
    m_pDlg->RecvData(this);                           // 接收数据
    CSocket::OnReceive(nErrorCode);
}

void CSocketSrv::OnAccept(int nErrorCode)
{
    m_pDlg->AddClient();                                  //添加上线用户
    CSocket::OnAccept(nErrorCode);

}


void CSocketSrv::OnClose(int nErrorCode)
{
    m_pDlg->RemoveClient(this);                   // 删除下线用户
    CSocket::OnClose(nErrorCode);
}

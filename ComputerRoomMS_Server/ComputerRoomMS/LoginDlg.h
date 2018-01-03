
// ComputerRoomMSDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "data.h"
#include "WinThreadDatabase.h"
#include "AGLinkCtrl.h"
#include "SocketSrv.h"
#include "resource.h"

class CSocketSrv;
class CWinThreadDatabase;

// CComputerRoomMSDlg 对话框
class CLoginDlg : public CDialogEx
{
// 构造
public:
	CLoginDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DLG_LOGIN };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HCURSOR OnQueryDragIcon();
    
    afx_msg LRESULT OnShowLoginDlg(WPARAM wParam, LPARAM lParam); 
    afx_msg void OnClose();
    afx_msg void OnBnClickedLogin();
    afx_msg void OnStnClickedStaticRegister();
	DECLARE_MESSAGE_MAP()

    
    LRESULT OnLoginSuccess(WPARAM wParam, LPARAM lParam);
    LRESULT OnLoginError(WPARAM wParam, LPARAM lParam);
    LRESULT OnInitDataSuccess(WPARAM wParam, LPARAM lParam);
    LRESULT OnInitDataFailed(WPARAM wParam, LPARAM lParam);


private:
    void InitCtrls();
    BOOL CheckEmpty(CString strAccount, CString strPwd);
    void InitDatabaseThread();

public:
    CWinThreadDatabase          *m_pThreadDatabase;

    CSocketSrv* listenSocket;     // 用于打开服务器
    CSocketSrv *m_pRemoteSocket;
    CPtrList m_clientList;                 // 链表用于存储用户
    bool m_connect;                       // 用于标记服务器状态
    int m_userCount;
    int m_port;


    void AddClient();                       // 增加用户，响应用户请求
    void RemoveClient(CSocketSrv* pSocket);          // 移除下线的用户
    void RecvData(CSocketSrv* pSocket);                 // 获取数据
    void UpdateEvent(CString str);  // 更新事件日志
    BOOL WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen);        
    //字符转换
    void SendMSG(CString str);       
    void PraseJson(char * buf);

    BOOL SendMsgToClient(Json::Value root);

public:
    afx_msg void OnBnClickedButton1();
    CEdit m_event;
};


// ComputerRoomMSDlg.h : ͷ�ļ�
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

// CComputerRoomMSDlg �Ի���
class CLoginDlg : public CDialogEx
{
// ����
public:
	CLoginDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DLG_LOGIN };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

    CSocketSrv* listenSocket;     // ���ڴ򿪷�����
    CSocketSrv *m_pRemoteSocket;
    CPtrList m_clientList;                 // �������ڴ洢�û�
    bool m_connect;                       // ���ڱ�Ƿ�����״̬
    int m_userCount;
    int m_port;


    void AddClient();                       // �����û�����Ӧ�û�����
    void RemoveClient(CSocketSrv* pSocket);          // �Ƴ����ߵ��û�
    void RecvData(CSocketSrv* pSocket);                 // ��ȡ����
    void UpdateEvent(CString str);  // �����¼���־
    BOOL WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen);        
    //�ַ�ת��
    void SendMSG(CString str);       
    void PraseJson(char * buf);

    BOOL SendMsgToClient(Json::Value root);

public:
    afx_msg void OnBnClickedButton1();
    CEdit m_event;
};

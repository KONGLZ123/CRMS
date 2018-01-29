
// SnifferDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "AdpDlg.h"
#include "FilterDlg.h"

// CSnifferDlg �Ի���
class CSnifferDlg : public CDialogEx
{
// ����
public:
	CSnifferDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SNIFFER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    CListCtrl m_list1;
    CTreeCtrl m_tree1;
    CString m_tcpnum;
    CString m_udpnum;
    CString m_arpnum;
    CString m_icmpnum;
    CString m_igmpnum;
    CString m_httpnum;
    CString m_dnsnum;
    CEdit m_edit1;
    CString m_qqnum;
    CString m_bullnum;

    CArray<const struct pcap_pkthdr *, const struct pcap_pkthdr *>  m_pktHeaders;
    CArray<const u_char *, const u_char *>  m_pktDatas;


    pcap_if_t *m_pDevice;//��ŷ���������������
    LPSTR m_filtername;//���ѡ��Ĺ���Э��
    long m_tcpCount;
    long m_udpCount;
    long m_arpCount;
    long m_icmpCount;
    long m_httpCount;
    
    BOOL m_bFlag;

    afx_msg void OnAdp();
    afx_msg void OnFilter();
    void ShowPacketList(struct pcap_pkthdr *pkt_header, const u_char *pkt_data);
    void ShowPacketTree(const pcap_pkthdr *pkt_header, const u_char *pkt_data, long index);
    void ShowPckNum();
    afx_msg void OnStart();
    afx_msg void OnStop();
};

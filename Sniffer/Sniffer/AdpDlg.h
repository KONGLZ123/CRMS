#pragma once
#include "afxcmn.h"


// CAdpDlg �Ի���

class CAdpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdpDlg)

public:
	CAdpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAdpDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CListCtrl m_list1;
    afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnBnClickedOk();

    pcap_if_t* GetDevice();
    pcap_if_t *returnd() { return d; }

private:
    pcap_if_t *alldevs;
    pcap_if_t *d;
    CString adpname;
};

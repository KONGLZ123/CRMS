#pragma once
#include "afxcmn.h"


// CAdpDlg 对话框

class CAdpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAdpDlg)

public:
	CAdpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAdpDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

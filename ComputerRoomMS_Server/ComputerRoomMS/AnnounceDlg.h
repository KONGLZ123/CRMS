#pragma once
#include "WinThreadDatabase.h"

// CAnnounceDlg 对话框

class CAnnounceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAnnounceDlg)

public:
	CAnnounceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAnnounceDlg();

	enum { IDD = IDD_DLG_ANNOUNCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    afx_msg void OnBnClickedBtnAnnounce();
    afx_msg void OnBnClickedBtnDelAnnounce();
	DECLARE_MESSAGE_MAP()

public:
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);
    void SetAnnounceData(vector<ANNOUNCE_DATA> &vecAnnounceData);

private:
    CWinThreadDatabase *            m_pDbThread;
    vector<ANNOUNCE_DATA>           m_vecAnnounceData;
    ANNOUNCE_DATA                   m_announceData;
    int                             m_pageIndex;
public:
    afx_msg void OnBnClickedBtnPageup();
    afx_msg void OnBnClickedBtnPagedow();
};

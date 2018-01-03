#pragma once


// CViewAnnounceDlg �Ի���

class CViewAnnounceDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewAnnounceDlg)

public:
	CViewAnnounceDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CViewAnnounceDlg();

	enum { IDD = IDD_DLG_VIEW_ANNOUNCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()

public:
    void SetAnnounceData(vector<ANNOUNCE_DATA> &vecAnnounceData);

private:
    vector<ANNOUNCE_DATA>           m_vecAnnounceData;
    ANNOUNCE_DATA                   m_announceData;
    int                             m_pageIndex;
public:
    afx_msg void OnBnClickedBtnPage1();
    afx_msg void OnBnClickedBtnPage2();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

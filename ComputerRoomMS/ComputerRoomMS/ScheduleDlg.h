#pragma once
#include "afxcmn.h"
#include "ListCtrlOwner.h"
#include "afxwin.h"

// CScheduleDlg �Ի���

class CScheduleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScheduleDlg)

public:
	CScheduleDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScheduleDlg();

	enum { IDD = IDD_DLG_VIEW_SCHEDULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
    void SetScheduleInfo(vector<CLASS_DATA> & vecScheduleData);

private:
    CListCtrlOwner              m_listSchedule;
    vector<CLASS_DATA>          m_vecScheduleData;
    map<int, vector<CLASS_DATA>> m_mapWeek2Class;
public:
    CComboBox m_comboWeek;
    afx_msg void OnCbnSelchangeCombo1();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnBnClickedBtnSearch();
};

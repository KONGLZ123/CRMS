#pragma once
#include "afxcmn.h"
#include "ListCtrlOwner.h"
#include "afxwin.h"

// CScheduleDlg 对话框

class CScheduleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScheduleDlg)

public:
	CScheduleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScheduleDlg();

	enum { IDD = IDD_DLG_VIEW_SCHEDULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
    void SetScheduleInfo(vector<SCHEDULE_DATA> & vecScheduleData);

private:
    CListCtrlOwner              m_listSchedule;
    vector<SCHEDULE_DATA>       m_vecScheduleData;

public:
    CComboBox m_comboWeek;
    afx_msg void OnCbnSelchangeCombo1();
};

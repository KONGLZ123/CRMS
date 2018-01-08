#pragma once
#include "afxcmn.h"
#include "ListCtrlOwner.h"

// CViewExamResultDlg 对话框

class CViewExamResultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewExamResultDlg)

public:
	CViewExamResultDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewExamResultDlg();

	enum { IDD = IDD_DLG_VIEW_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    CListCtrlOwner m_listInfo;
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    void SetAttendenceInfo(vector<ATTENDENCE_INFO>& vecStudentInfo);
};

#pragma once


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
};

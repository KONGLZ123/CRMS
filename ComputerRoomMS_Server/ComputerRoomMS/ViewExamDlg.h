#pragma once


// CViewExamDlg 对话框

class CViewExamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewExamDlg)

public:
	CViewExamDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewExamDlg();


	enum { IDD = IDD_DLG_VIEW_EXAM };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

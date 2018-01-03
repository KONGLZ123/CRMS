#pragma once


// CAddPersonDlg 对话框

class CAddPersonDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddPersonDlg)

public:
	CAddPersonDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddPersonDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_ADD_PERSON };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

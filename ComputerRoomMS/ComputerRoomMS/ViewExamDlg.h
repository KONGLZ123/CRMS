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

public:
    void SetExamInfo(vector<EXAM_INFO>& vecExamInfo);
    void SetCurStuClass(CString strClass);

private:
    vector<EXAM_INFO>           m_vecExamInfo;
    EXAM_INFO                   m_examInfo;
    int                         m_pageIndex;
    CString                     m_curStuClass;
public:
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

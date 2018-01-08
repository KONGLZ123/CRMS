#pragma once


// CSearchResultDlg 对话框

class CSearchResultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchResultDlg)

public:
	CSearchResultDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSearchResultDlg();

	enum { IDD = IDD_DLG_SEARCH_RESULT };
    void SetStudentInfo(STUDETN_INFO &studentInfo);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
    STUDETN_INFO m_stuInfo;
public:
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);

};

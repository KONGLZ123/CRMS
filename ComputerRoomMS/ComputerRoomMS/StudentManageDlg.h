#pragma once
#include "ListCtrlOwner.h"
#include "WinThreadDatabase.h"
#include "afxcmn.h"
#include "afxwin.h"

// CStudentManageDlg 对话框

class CStudentManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStudentManageDlg)

public:
	CStudentManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStudentManageDlg();

	enum { IDD = IDD_DLG_STUDENT_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);
    void SetStudentInfo(vector<STUDETN_INFO>& vecStudentInfo);
    void SetClassName(set<CString> setClassName);

private:
    CListCtrlOwner              m_listPersonInfo;
    CWinThreadDatabase*         m_pDbThread;
    vector<STUDETN_INFO>        m_vecStudentInfo;
    set<CString>                m_setClassName;
public:
    afx_msg void OnBnClickedBtnDeclareExam();
    //CComboBox m_comboExamClass;
    CComboBox m_comboClass1;
    CComboBox m_comboClass2;
    afx_msg void OnCbnSelchangeComboClassName1();
    map<CString, vector<STUDETN_INFO>> m_mapClass2StuInfo;
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    LRESULT OnUpdateStudentListItem(WPARAM wParam, LPARAM lParam);
};

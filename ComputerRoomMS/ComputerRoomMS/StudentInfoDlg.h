#pragma once
#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "SetupPwdDlg.h"
#include "afxdialogex.h"
// CStudentInfoDlg 对话框

class CStudentInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStudentInfoDlg)

public:
	CStudentInfoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStudentInfoDlg();

	enum { IDD = IDD_DLG_VIEW_STU_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    afx_msg void OnBnClickedBtnPersonSetup();
	DECLARE_MESSAGE_MAP()

public:
    void SetStudentInfo(STUDETN_INFO &studentInfo);
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);


private:
    STUDETN_INFO        m_studentInfo;
    int                     m_role;
    CWinThreadDatabase *    m_pDbThread;
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

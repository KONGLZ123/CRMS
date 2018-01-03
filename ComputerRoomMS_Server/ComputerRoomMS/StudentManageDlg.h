#pragma once


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
};

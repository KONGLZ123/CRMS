#pragma once


// CStudentManageDlg �Ի���

class CStudentManageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CStudentManageDlg)

public:
	CStudentManageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStudentManageDlg();

	enum { IDD = IDD_DLG_STUDENT_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#pragma once


// CViewExamResultDlg �Ի���

class CViewExamResultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewExamResultDlg)

public:
	CViewExamResultDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CViewExamResultDlg();

	enum { IDD = IDD_DLG_VIEW_RESULT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

#pragma once


// CViewExamDlg �Ի���

class CViewExamDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewExamDlg)

public:
	CViewExamDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CViewExamDlg();


	enum { IDD = IDD_DLG_VIEW_EXAM };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

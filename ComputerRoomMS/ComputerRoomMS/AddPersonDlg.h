#pragma once


// CAddPersonDlg �Ի���

class CAddPersonDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddPersonDlg)

public:
	CAddPersonDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddPersonDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_ADD_PERSON };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

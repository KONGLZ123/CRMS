
// testplayDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CtestplayDlg �Ի���
class CtestplayDlg : public CDialogEx
{
// ����
public:
	CtestplayDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTPLAY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedBtnPlay();
    afx_msg void OnBnClickedBtnStop();
    afx_msg void OnBnClickedBtnBreak();
    afx_msg void OnBnClickedBtnOpen();
    CEdit m_url;
    CStatic m_videoWnd;
};

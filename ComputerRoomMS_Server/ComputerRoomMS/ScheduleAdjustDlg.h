#pragma once


// CScheduleAdjustDlg �Ի���

class CScheduleAdjustDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScheduleAdjustDlg)

public:
	CScheduleAdjustDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScheduleAdjustDlg();

	enum { IDD = IDD_DLG_SCHEDULE_ADJUST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};

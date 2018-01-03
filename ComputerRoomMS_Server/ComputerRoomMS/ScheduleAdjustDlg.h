#pragma once


// CScheduleAdjustDlg 对话框

class CScheduleAdjustDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CScheduleAdjustDlg)

public:
	CScheduleAdjustDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScheduleAdjustDlg();

	enum { IDD = IDD_DLG_SCHEDULE_ADJUST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};

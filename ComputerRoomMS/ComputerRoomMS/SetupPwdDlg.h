#pragma once
#include "winThreadDatabase.h"

// CSetupPwdDlg �Ի���

class CSetupPwdDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupPwdDlg)

public:
	CSetupPwdDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetupPwdDlg();

	enum { IDD = IDD_DLG_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
    afx_msg void OnBnClickedBtnPersonSetup();
	DECLARE_MESSAGE_MAP()

public:
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);
    void SetRoomManagerInfo(ROOM_MANAGER_INFO & personInfo);
    void SetRepairInfo(ROOM_REPAIR_INFO & personInfo);
    void SetTeacherInfo(TEACHER_INFO & personInfo);


private:
    ROOM_MANAGER_INFO       m_managerInfo;
    ROOM_REPAIR_INFO        m_repairInfo;
    TEACHER_INFO            m_teacherInfo;
    int                     m_role;
    CWinThreadDatabase *    m_pDbThread;
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

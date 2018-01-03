#pragma once
#include "ButtonCtrl.h"
#include "PersonInfo.h"
#include "announceDlg.h"
#include "RoomManageDlg.h"
#include "ViewInfoDlg.h"
#include "ScheduleDlg.h"
#include "ViewAnnounceDlg.h"
#include "RepairDlg.h"
#include "UseReportDlg.h"
#include "SetupPwdDlg.h"
#include "RequestRoomDlg.h"
#include "StudentInfoDlg.h"
#include "WinThreadDatabase.h"
#include "ScheduleAdjustDlg.h"
#include "RoomFaultDlg.h"
#include "ViewExamDlg.h"
#include "ViewExamResultDlg.h"
#include "StudentManageDlg.h"
#include "WorkloadDlg.h"
#include "SalaryCheckDlg.h"
#include "AssetsHandoverDlg.h"
#include "ViewRepairDlg.h"
#include "RoomRunningReportDlg.h"
#include "SetupAdminDlg.h"

#include <map>
#include <vector>
using namespace std;

#define WM_SHOWLOGIN        WM_USER + 1000

// CDlgMain 对话框


struct BUTTON_INFO
{
    CString strName;
    CDialogEx *pDlg;
};

class CDlgMain : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgMain)

public:
	CDlgMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMain();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    //afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

    afx_msg void OnPaint();
    afx_msg void OnButtonClick(UINT uID);
    afx_msg void OnClose();

public:
    void SetAccount(CString account);
    void SetRole(int curSel);
    void ResetButtonStatus();
    void CreateButton();
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);

private:
    void InitData(int role);
    LRESULT OnInitDataSuccess(WPARAM wParam, LPARAM lParam);

private:
    CString                 m_curAccount;
    CButtonCtrl *           m_pBtn;
    map<int, CDialogEx *>   m_mapBtnToDlg;
    int                     m_btnNum;
    int                     m_role;
    CFont                   m_fontBtn;
    CPersonInfo             m_personDlg;
    CAnnounceDlg            m_announceDlg;
    CViewInfoDlg            m_viewInfoDlg;
    CAssetsListDlg          m_roomManagerDlg;
    CScheduleDlg            m_scheduleDlg;
    CViewAnnounceDlg        m_viewAnnounceDlg;
    CRepairDlg              m_repairDlg;
    CUseReportDlg           m_useReportDlg;
    CSetupPwdDlg            m_setupPwdDlg;
    CRequestRoomDlg         m_requestRoomDlg;
    CStudentInfoDlg         m_studentInfoDlg;
    CScheduleAdjustDlg      m_scheduleAdjustDlg;
    CRoomFaultDlg           m_roomFaultDlg;
    CViewExamDlg            m_viewExamDlg;
    CViewExamResultDlg      m_viewExamResultDlg;
    CStudentManageDlg       m_studentManageDlg;
    CWorkloadDlg            m_workloadDlg;
    CSalaryCheckDlg         m_salaryCheckDlg;
    CAssetsHandoverDlg      m_assetsHandoverDlg;
    CViewRepairDlg          m_viewRepairDlg;
    CRoomRunningReportDlg   m_roomRunningReportDlg;
    CSetupAdminDlg          m_setupAdminDlg;

    vector<BUTTON_INFO>     m_vecBtnInfo;
    BOOL                    m_isCloseLogin;
    CWinThreadDatabase *    m_pDbThread;
    DATA_STOCK              m_dataStock;
public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

// DlgMain.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "DlgMain.h"
#include "afxdialogex.h"
#include <set>

#define IDC_BTN     1001
// CDlgMain 对话框

IMPLEMENT_DYNAMIC(CDlgMain, CDialogEx)

CDlgMain::CDlgMain(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_MAIN, pParent)
{
    m_isCloseLogin = TRUE;
}

CDlgMain::~CDlgMain()
{
    for (UINT i = 0; i < m_vecBtnInfo.size(); i++)
    {
        if (NULL != m_pBtn && NULL != m_pBtn[i].GetSafeHwnd())
        {
            m_pBtn[i].DestroyWindow();
        }
    }

    if (NULL != m_pBtn)
    {
        delete[] m_pBtn;
        m_pBtn = NULL;
    }
}

void CDlgMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgMain, CDialogEx)
    ON_WM_PAINT()
    ON_COMMAND_RANGE(IDC_BTN, IDC_BTN + 10 - 1, OnButtonClick)
    ON_WM_CLOSE()
    ON_MESSAGE(WM_INITDATA_SUCCESS, OnInitDataSuccess)
END_MESSAGE_MAP()


// CDlgMain 消息处理程序


BOOL CDlgMain::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    SetBackgroundColor(RGB(250, 250, 250));

    MoveWindow(CRect(0, 0, 846, 598), TRUE);
    CenterWindow();

    m_fontBtn.CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("微软雅黑"));

    InitData(m_role);

    ModifyStyle(0, WS_MINIMIZEBOX);
    ModifyStyleEx(WS_EX_TOOLWINDOW, WS_EX_APPWINDOW);

    return TRUE;  
}

void CDlgMain::OnButtonClick(UINT uID)
{
    map<int, CDialogEx *>::iterator it;
    for (it = m_mapBtnToDlg.begin(); it != m_mapBtnToDlg.end(); it++)
    {
        int nBtnId = it->first;
        CDialogEx * pDlg = it->second;
        if (NULL != pDlg)
            pDlg->ShowWindow(SW_HIDE);

        if (nBtnId == uID)
        {
            ResetButtonStatus();

            CButtonCtrl *pBtn = (CButtonCtrl *)GetDlgItem(uID);
            CString strText;
            pBtn->GetWindowText(strText);
            if (strText == _T("注销登录"))
            {
                m_isCloseLogin = FALSE;
                //OnClose();
                ::PostMessage(this->GetSafeHwnd(), WM_CLOSE, 0, 0);
                ::SendMessage(this->GetParent()->GetSafeHwnd(), WM_SHOWLOGIN, 0, 0);
                return;
            }
            else
            {
                pBtn->SwitchButtonStatus(CButtonCtrl::AGBTN_PUSH);
                if (NULL != pDlg)
                    pDlg->ShowWindow(SW_SHOW);
            }
        }
    }
}

void CDlgMain::ResetButtonStatus()
{
    for (UINT i = 0; i < m_vecBtnInfo.size(); i++)
    {
        if (NULL != m_pBtn && NULL != m_pBtn[i].GetSafeHwnd())
        {
            m_pBtn[i].SwitchButtonStatus(CButtonCtrl::AGBTN_NORMAL);
            Invalidate();
        }
    }
}

void CDlgMain::CreateButton()
{
    CDC *pDC = GetDC();
    m_btnNum = 3;
    int lastBtnCY = 0;
    m_pBtn = new CButtonCtrl[m_vecBtnInfo.size()];
    for (UINT i = 0; i < m_vecBtnInfo.size(); i++)
    {
        m_pBtn[i].Create(_T(""), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(0, lastBtnCY, 150, lastBtnCY + 40), this, IDC_BTN + i);
        m_pBtn[i].SetBackColor(RGB(250, 250, 250), RGB(243, 243, 255), RGB(240, 240, 240), RGB(255, 255, 255));
        m_pBtn[i].SetTextColor(RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0), RGB(0, 0, 0));
        m_pBtn[i].SwitchButtonStatus(CButtonCtrl::AGBTN_NORMAL);
        m_pBtn[i].EnableFrameEffect(FALSE);
        m_pBtn[i].SetFont(&m_fontBtn);
        m_mapBtnToDlg[IDC_BTN + i] = m_vecBtnInfo.at(i).pDlg;
        m_pBtn[i].SetWindowText(m_vecBtnInfo.at(i).strName);
        lastBtnCY = lastBtnCY + 40;

        // 初始化时显示第一卷
        if (0 == i)
        {
            m_pBtn[i].SwitchButtonStatus(CButtonCtrl::AGBTN_PUSH);
        }
    }
}

void CDlgMain::SetThreadDatabase(CWinThreadDatabase *pDbThread)
{
    m_pDbThread = pDbThread;
}

void CDlgMain::SetAccount(CString account)
{
    m_curAccount = account;
}

void CDlgMain::SetRole(int curSel)
{
    m_role = curSel;
}


void CDlgMain::InitData(int role)
{
    CRect rcClient;
    GetClientRect(&rcClient);
    BUTTON_INFO btnInfo;

    switch (role)
    {
    case ADMIN:
    {
        btnInfo.strName = _T("人员信息维护");
        m_personDlg.Create(CPersonInfo::IDD, this);
        m_personDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_personDlg;
        m_personDlg.SetThreadDatabase(m_pDbThread);
        m_personDlg.ShowWindow(SW_SHOW);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("信息处理");
        m_viewInfoDlg.Create(CViewInfoDlg::IDD, this);
        m_viewInfoDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        m_viewInfoDlg.SetThreadDatabase(m_pDbThread);
        btnInfo.pDlg = &m_viewInfoDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("发布公告");
        m_announceDlg.Create(CAnnounceDlg::IDD, this);
        m_announceDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        m_announceDlg.SetThreadDatabase(m_pDbThread);
        btnInfo.pDlg = &m_announceDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("课表查询");
        m_scheduleDlg.Create(CScheduleDlg::IDD, this);
        m_scheduleDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_scheduleDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("课表调整");
        m_scheduleAdjustDlg.Create(CScheduleAdjustDlg::IDD, this);
        m_scheduleAdjustDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        m_scheduleAdjustDlg.SetThreadDatabase(m_pDbThread);
        btnInfo.pDlg = &m_scheduleAdjustDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("津贴核算");
        m_salaryCheckDlg.Create(CSalaryCheckDlg::IDD, this);
        m_salaryCheckDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        m_salaryCheckDlg.SetThreadDatabase(m_pDbThread);
        btnInfo.pDlg = &m_salaryCheckDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("机房资产清单");
        m_roomManagerDlg.Create(CAssetsListDlg::IDD, this);
        m_roomManagerDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_roomManagerDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("修改密码");
        m_setupAdminDlg.Create(CSetupAdminDlg::IDD, this);
        m_setupAdminDlg.SetThreadDatabase(m_pDbThread);
        m_setupAdminDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_setupAdminDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("注销登录");
        btnInfo.pDlg = NULL;
        m_vecBtnInfo.push_back(btnInfo);

        m_btnNum = m_vecBtnInfo.size();
        CreateButton();       
    }
    break;
    case MANAGER:
    {
        btnInfo.strName = _T("查看公告");
        m_viewAnnounceDlg.Create(CViewAnnounceDlg::IDD, this);
        m_viewAnnounceDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_viewAnnounceDlg;
        m_viewAnnounceDlg.ShowWindow(SW_SHOW);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("机房故障申报");
        m_roomFaultDlg.Create(CRoomFaultDlg::IDD, this);
        m_roomFaultDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        m_roomFaultDlg.SetThreadDatabase(m_pDbThread);
        btnInfo.pDlg = &m_roomFaultDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("课表查询");
        m_scheduleDlg.Create(CScheduleDlg::IDD, this);
        m_scheduleDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_scheduleDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("机房运行报告");
        m_roomRunningReportDlg.Create(CRoomRunningReportDlg::IDD, this);
        m_roomRunningReportDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_roomRunningReportDlg;
        m_roomRunningReportDlg.SetThreadDatabase(m_pDbThread);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("资产交接报告");
        m_assetsHandoverDlg.Create(CAssetsHandoverDlg::IDD, this);
        m_assetsHandoverDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_assetsHandoverDlg;
        m_assetsHandoverDlg.SetThreadDatabase(m_pDbThread);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("额外工作量申报");
        m_workloadDlg.Create(CWorkloadDlg::IDD, this);
        m_workloadDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_workloadDlg;
        m_workloadDlg.SetThreadDatabase(m_pDbThread);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("个人信息处理");
        m_setupPwdDlg.Create(CSetupPwdDlg::IDD, this);
        m_setupPwdDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_setupPwdDlg;
        m_setupPwdDlg.SetThreadDatabase(m_pDbThread);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("注销登录");
        btnInfo.pDlg = NULL;
        m_vecBtnInfo.push_back(btnInfo);

        m_btnNum = m_vecBtnInfo.size();
        CreateButton();
    }
    break;
    case REPAIR:
    {
        btnInfo.strName = _T("维修需求查看");
        m_viewRepairDlg.Create(CViewRepairDlg::IDD, this);
        m_viewRepairDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_viewRepairDlg;
        m_viewRepairDlg.ShowWindow(SW_SHOW);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("维修报告");
        m_repairDlg.Create(CRepairDlg::IDD, this);
        m_repairDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_repairDlg;
        m_repairDlg.SetThreadDatabase(m_pDbThread);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("个人信息处理");
        m_setupPwdDlg.Create(CSetupPwdDlg::IDD, this);
        m_setupPwdDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_setupPwdDlg;
        m_setupPwdDlg.SetThreadDatabase(m_pDbThread);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("注销登录");
        btnInfo.pDlg = NULL;
        m_vecBtnInfo.push_back(btnInfo);

        m_btnNum = m_vecBtnInfo.size();
        CreateButton();
    }
    break;
    case TEACHER:
    {
        btnInfo.strName = _T("查看公告");
        m_viewAnnounceDlg.Create(CViewAnnounceDlg::IDD, this);
        m_viewAnnounceDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_viewAnnounceDlg;
        m_viewAnnounceDlg.ShowWindow(SW_SHOW);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("机房故障申报");
        m_roomFaultDlg.Create(CRoomFaultDlg::IDD, this);
        m_roomFaultDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        m_roomFaultDlg.SetThreadDatabase(m_pDbThread);
        btnInfo.pDlg = &m_roomFaultDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("课表查询");
        m_scheduleDlg.Create(CScheduleDlg::IDD, this);
        m_scheduleDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_scheduleDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("学生管理");
        m_studentManageDlg.Create(CStudentManageDlg::IDD, this);
        m_studentManageDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        m_studentManageDlg.SetThreadDatabase(m_pDbThread);
        btnInfo.pDlg = &m_studentManageDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("机房使用报告");
        m_useReportDlg.Create(CUseReportDlg::IDD, this);
        m_useReportDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        m_useReportDlg.SetThreadDatabase(m_pDbThread);
        btnInfo.pDlg = &m_useReportDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("机房使用申请");
        m_requestRoomDlg.Create(CRequestRoomDlg::IDD, this);
        m_requestRoomDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        m_requestRoomDlg.SetThreadDatabase(m_pDbThread);
        btnInfo.pDlg = &m_requestRoomDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("个人信息处理");
        m_setupPwdDlg.Create(CSetupPwdDlg::IDD, this);
        m_setupPwdDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_setupPwdDlg;
        m_setupPwdDlg.SetThreadDatabase(m_pDbThread);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("注销登录");
        btnInfo.pDlg = NULL;
        m_vecBtnInfo.push_back(btnInfo);

        m_btnNum = m_vecBtnInfo.size();
        CreateButton();
    }
    break;
    case STUDENT:
    {
        btnInfo.strName = _T("查看公告");
        m_viewAnnounceDlg.Create(CViewAnnounceDlg::IDD, this);
        m_viewAnnounceDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_viewAnnounceDlg;
        m_viewAnnounceDlg.ShowWindow(SW_SHOW);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("查看课表");
        m_scheduleDlg.Create(CScheduleDlg::IDD, this);
        m_scheduleDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_scheduleDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("考试信息查看");
        m_viewExamDlg.Create(CViewExamDlg::IDD, this);
        m_viewExamDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_viewExamDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("查看出勤情况");
        m_viewExamResultDlg.Create(CViewExamResultDlg::IDD, this);
        m_viewExamResultDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_viewExamResultDlg;
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("个人信息处理");
        m_studentInfoDlg.Create(CStudentInfoDlg::IDD, this);
        m_studentInfoDlg.MoveWindow(150, 0, rcClient.Width() - 150, rcClient.Height(), TRUE);
        btnInfo.pDlg = &m_studentInfoDlg;
        m_studentInfoDlg.SetThreadDatabase(m_pDbThread);
        m_vecBtnInfo.push_back(btnInfo);

        btnInfo.strName = _T("注销登录");
        btnInfo.pDlg = NULL;
        m_vecBtnInfo.push_back(btnInfo);

        m_btnNum = m_vecBtnInfo.size();
        CreateButton();
    }
    break;
    default:
        break;
    }

    PostThreadMessage(m_pDbThread->m_nThreadID, WM_INIT_DATA, role, (LPARAM)m_hWnd);
}

LRESULT CDlgMain::OnInitDataSuccess(WPARAM wParam, LPARAM lParam)
{
    DATA_STOCK *pDataStock = (DATA_STOCK *)wParam;
    m_dataStock = *pDataStock;

    // 获取班级集合
    for (UINT i = 0; i < pDataStock->vecStudentInfo.size(); i++)
    {
        pDataStock->setClass.insert(pDataStock->vecStudentInfo.at(i).classes);
    }

    switch (m_role)
    {
    case ADMIN:
    {
        m_personDlg.SetPersonInfo(m_dataStock);
        m_personDlg.SetRegisterData(pDataStock->vecRegisterData);

        m_viewInfoDlg.SetReportData(pDataStock->vecReportData);
        m_announceDlg.SetAnnounceData(pDataStock->vecAnnounceData);
        m_setupAdminDlg.SetAdminInfo(pDataStock->vecAdminInfo.at(0));   // 只有一个管理员
        m_scheduleDlg.SetScheduleInfo(pDataStock->vecScheduleData);
        m_scheduleAdjustDlg.SetClassName(pDataStock->setClass);

        m_salaryCheckDlg.SetSalaryInfo(pDataStock->vecSalaryInfo);
        m_roomManagerDlg.SetAssertDate(pDataStock->vecAssertDataInfo);
    }
    break;
    case MANAGER:
    {
        for (UINT i = 0; i < pDataStock->vecRoomManagerInfo.size(); i++)
        {
            if (m_curAccount == pDataStock->vecRoomManagerInfo.at(i).account)
            {
                m_setupPwdDlg.SetRoomManagerInfo(pDataStock->vecRoomManagerInfo.at(i));
                m_roomFaultDlg.SetCurUserName(pDataStock->vecRoomManagerInfo.at(i).name);
                m_useReportDlg.SetCurUserName(pDataStock->vecRoomManagerInfo.at(i).name);
                m_roomRunningReportDlg.SetCurUserName(pDataStock->vecRoomManagerInfo.at(i).name);
                m_assetsHandoverDlg.SetCurUserName(pDataStock->vecRoomManagerInfo.at(i).name);
                m_workloadDlg.SetCurUserName(pDataStock->vecRoomManagerInfo.at(i).name);
                break;
            }
        }

        vector<ANNOUNCE_DATA>::iterator it;
        for (it = pDataStock->vecAnnounceData.begin();
            it != pDataStock->vecAnnounceData.end(); )
        {
            if (0x0001 != (it->type & 0x0001))
            {
                it = pDataStock->vecAnnounceData.erase(it);
            }
            else
            {
                ++it;
            }
        }
        
        m_viewAnnounceDlg.SetAnnounceData(pDataStock->vecAnnounceData);
        m_scheduleDlg.SetScheduleInfo(pDataStock->vecScheduleData);
    }
    break;
    case TEACHER:
    {
        for (UINT i = 0; i < pDataStock->vecTeacherInfo.size(); i++)
        {
            if (m_curAccount == pDataStock->vecTeacherInfo.at(i).account)
            {
                m_setupPwdDlg.SetTeacherInfo(pDataStock->vecTeacherInfo.at(i));
                m_roomFaultDlg.SetCurUserName(pDataStock->vecTeacherInfo.at(i).name);
                m_requestRoomDlg.SetCurUserName(pDataStock->vecTeacherInfo.at(i).name);

                CString strResult = _T("无结果");
                for (UINT i = 0; i < pDataStock->vecReportData.size(); i++)
                {
                    if (REQUEST_ROOM == pDataStock->vecReportData.at(i).reportType &&
                        pDataStock->vecTeacherInfo.at(i).name == pDataStock->vecReportData.at(i).submitPerson)
                    {
                        if (pDataStock->vecReportData.at(i).isView)
                            strResult = _T("同意使用");
                        else
                            strResult = _T("拒绝使用");
                        break;
                    }
                }
                m_requestRoomDlg.SetRequestResult(strResult);
                break;
            }
        }
        vector<ANNOUNCE_DATA>::iterator it;
        for (it = pDataStock->vecAnnounceData.begin(); it != pDataStock->vecAnnounceData.end(); )
        {
            if (0x0010 != (it->type & 0x0010))
            {
                it = pDataStock->vecAnnounceData.erase(it);
            }
            else
            {
                ++it;
            }
        }


        m_studentManageDlg.SetStudentInfo(pDataStock->vecStudentInfo);
        m_studentManageDlg.SetClassName(pDataStock->setClass);
        m_viewAnnounceDlg.SetAnnounceData(pDataStock->vecAnnounceData);
        m_scheduleDlg.SetScheduleInfo(pDataStock->vecScheduleData);
    }
    break;
    case REPAIR:
    {
        for (UINT i = 0; i < pDataStock->vecRoomRepairInfo.size(); i++)
        {
            if (m_curAccount == pDataStock->vecRoomRepairInfo.at(i).account)
            {
                m_setupPwdDlg.SetRepairInfo(pDataStock->vecRoomRepairInfo.at(i));
                m_repairDlg.SetCurUserName(pDataStock->vecRoomRepairInfo.at(i).name);
                break;
            }
        }
        m_viewRepairDlg.SeRepairData(pDataStock->vecReportData);
    }
    break;
    case STUDENT:
    {
        for (UINT i = 0; i < pDataStock->vecStudentInfo.size(); i++)
        {
            if (m_curAccount == pDataStock->vecStudentInfo.at(i).student_id)
            {
                m_studentInfoDlg.SetStudentInfo(pDataStock->vecStudentInfo.at(i));
                m_viewExamDlg.SetCurStuClass(pDataStock->vecStudentInfo.at(i).classes);
                break;
            }
        }
        vector<ANNOUNCE_DATA>::iterator it;
        for (it = pDataStock->vecAnnounceData.begin();
            it != pDataStock->vecAnnounceData.end(); )
        {
            if (0x0100 != (it->type & 0x0100))
            {
                it = pDataStock->vecAnnounceData.erase(it);
            }
            else
            {
                ++it;
            }
        }
        m_scheduleDlg.SetScheduleInfo(pDataStock->vecScheduleData);
        m_viewAnnounceDlg.SetAnnounceData(pDataStock->vecAnnounceData);
        m_viewExamDlg.SetExamInfo(pDataStock->vecExamInfo);
    }
    break;
    default:
        break;
    }

    delete pDataStock;
    pDataStock = NULL;

    return 0;
}


void CDlgMain::OnPaint()
{
    CPaintDC dc(this); 

    CRect rcClient;
    GetClientRect(&rcClient);

    CPen pen(PS_SOLID, 1, RGB(210, 226, 255));
    CPen *pOldPen = dc.SelectObject(&pen);

    dc.MoveTo(150, 0);
    dc.LineTo(150, rcClient.Height());

    dc.SelectObject(pOldPen);
}


void CDlgMain::OnClose()
{
    CDialogEx::OnClose();

    if (m_isCloseLogin)
        ::SendMessage(this->GetParent()->GetSafeHwnd(), WM_CLOSE, 0, 0);
}



BOOL CDlgMain::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_KEYDOWN)
    {
        if (pMsg->wParam == VK_ESCAPE)    //屏蔽ESC  
            return TRUE;

        if (pMsg->wParam == VK_RETURN)    //屏蔽回车 
            return TRUE;

        if (pMsg->wParam == VK_F1) 	//屏蔽F1帮助键
            return TRUE;
    }

    if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) 	//屏蔽ALT+F4
        return TRUE;

    return CDialogEx::PreTranslateMessage(pMsg);
}

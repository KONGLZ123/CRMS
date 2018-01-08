#pragma once
#include <vector>
#include "mysql.h"
#include "json.h"
#include "afxsock.h"
#include "data.h"
#include "SocketClient.h"

#pragma comment(lib, "libmysql.lib")
using namespace std;

// CWinThreadDatabase

class CSocketClient;

class CWinThreadDatabase : public CWinThread
{
	DECLARE_DYNCREATE(CWinThreadDatabase)

public:
	CWinThreadDatabase();           // 动态创建所使用的受保护的构造函数
	virtual ~CWinThreadDatabase();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

private:
    BOOL SendMsgToServer(Json::Value root);
    BOOL SendLoginRequestToServer(CString strAccount, CString strPwd, int role);

    void OnRecv(WPARAM wParam, LPARAM lParam);
    void PraseJson(char * buf);

    void OnLoginRequest(WPARAM wParam, LPARAM lParam);
    void OnInitData(WPARAM wParam, LPARAM lParam);
    void OnDeclareAnnounce(WPARAM wParam, LPARAM lParam);
    void OnDeleteAnnounce(WPARAM wParam, LPARAM lParam);
    void OnAdminSetupPwd(WPARAM wParam, LPARAM lParam);
    void OnSetupPwd(WPARAM wParam, LPARAM lParam);
    void OnSetupServer(WPARAM wParam, LPARAM lParam);
    void OnAddPerson(WPARAM wParam, LPARAM lParam);
    void OnInsertReport(WPARAM wParam, LPARAM lParam);
    void OnDelPersonInof(WPARAM wParam, LPARAM lParam);
    void OnUpdatePersonInof(WPARAM wParam, LPARAM lParam);
    void OnDelRegister(WPARAM wParam, LPARAM lParam);
    void OnDeclareExamInfo(WPARAM wParam, LPARAM lParam);
    void OnSetClassData(WPARAM wParam, LPARAM lParam);
    void OnInsertWorkload(WPARAM wParam, LPARAM lParam);
    void OnInsertAssertData(WPARAM wParam, LPARAM lParam);
    void OnUpdateRoomRequest(WPARAM wParam, LPARAM lParam);
    void OnUpdateExamInfo(WPARAM wParam, LPARAM lParam);
    void OnUpdateSalaryInfo(WPARAM wParam, LPARAM lParam);

    BOOL GetLoginData(LOGIN_REQUEST *ptagLoginRequest);
    void CheckPwd(LOGIN_REQUEST *ptagLoginRequest);
    BOOL SelectAdminInfoFromDb(vector<SYSADMIN_INFO> &vecAdminInfo, Json::Value & root);
    BOOL SelectRoomManagerInfoFromDb(vector<ROOM_MANAGER_INFO> &vecRoomManagerInfo, Json::Value & root);
    BOOL SelectRoomRepairInfoFromDb(vector<ROOM_REPAIR_INFO> &vecRepairInfo, Json::Value & root);
    BOOL SelectTeacherInfoFromDb(vector<TEACHER_INFO> &vecTeacherInfo, Json::Value & root);
    BOOL SelectStudentInfoFromDb(vector<STUDETN_INFO> &vecStudentInfo, Json::Value & root);
    BOOL SelectAttendenceInfoFromDb(vector<ATTENDENCE_INFO> &vecAttendenceInfo, Json::Value & root);
    BOOL SelectScheduleFromDb(vector<CLASS_DATA> &vecSheduleInfo, Json::Value & root);
    BOOL SelectReportDataFromDb(vector<REPORT_DATA> &vecReportData, Json::Value & root);
    BOOL SelectAnnounceFromDb(vector<ANNOUNCE_DATA> &vecAnnounceData, Json::Value & root);
    BOOL SelectRegisterInfoFromDb(vector<ADD_PERSON_DATA> &vecRegisterData, Json::Value & root);
    BOOL SelectExamInfoFromDb(vector<EXAM_INFO>& vecExamInfo, Json::Value& root);
    BOOL SelectWorkloadSalaryInfoFromDb(vector<WORKLOAD_REPORT>& vecSalaryInfo, Json::Value& root);
    BOOL SelectAssertHandoverFromDb(vector<ASSERT_DATA>& vecAssertDataInfo, Json::Value& root);
    DECLARE_MESSAGE_MAP()

public:
    BOOL ConnectServer();
    //void InitSocket();
    BOOL InitThread();

private:
    HANDLE                      m_hThread;
    //SOCKET                      m_socket;
    MYSQL                       m_mysql;
    vector<LOGIN_RESULT>        m_vecLoginResult;
    //HWND                        m_tmpHwnd;
    HWND                        m_curHwnd;

    CSocketClient *             m_pSocket;
    CString                     m_strIP;
    CString                     m_strPort;
};



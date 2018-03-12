#pragma once
#include <vector>
#include "mysql.h"
#include "json.h"
#include "afxsock.h"
#include "data.h"
#include "SocketSrv.h"

#pragma comment(lib, "libmysql.lib")
using namespace std;


class CWinThreadDatabase;

//struct SOCK_DATA
//{
//    SOCKET socket;
//    CWinThreadDatabase *pDbThread;
//};

// CWinThreadDatabase

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
    BOOL SendLoginRequestToServer(LOGIN_REQUEST *ptagLoginRequest);

    void OnRecv(WPARAM wParam, LPARAM lParam);
    void OnLoginRequest(WPARAM wParam, LPARAM lParam);
    void OnInitData(WPARAM wParam, LPARAM lParam);
    void OnDeclareAnnounce(WPARAM wParam, LPARAM lParam);
    void OnDeleteAnnounce(WPARAM wParam, LPARAM lParam);
    void OnAdminSetupPwd(WPARAM wParam, LPARAM lParam);
    void OnSetupPwd(WPARAM wParam, LPARAM lParam);
    void OnAddPerson(WPARAM wParam, LPARAM lParam);
    void OnRequestRepair(WPARAM wParam, LPARAM lParam);
    void OnDelPersonInof(WPARAM wParam, LPARAM lParam);
    void OnUpdatePersonInof(WPARAM wParam, LPARAM lParam);
    void OnDelRegister(WPARAM wParam, LPARAM lParam);
    void OnDeclareExamInfo(WPARAM wParam, LPARAM lParam);
    void OnSetClassData(WPARAM wParam, LPARAM lParam);
    void OnInsertWorkload(WPARAM wParam, LPARAM lParam);
    void OnInsertAssertData(WPARAM wParam, LPARAM lParam);
    void OnUpdateRoomRequest(WPARAM wParam, LPARAM lParam);
    void OnUpdateSalaryInfo(WPARAM wParam, LPARAM lParam);
    void OnUpdateExamInfo(WPARAM wParam, LPARAM lParam);
    void OnUpdateAssertList(WPARAM wParam, LPARAM lParam);

    BOOL GetLoginData(LOGIN_REQUEST *ptagLoginRequest);
    void CheckPwd(LOGIN_REQUEST *ptagLoginRequest);
    BOOL SelectAdminInfoFromDb(vector<SYSADMIN_INFO> &vecAdminInfo, Json::Value &vec);
    BOOL SelectRoomManagerInfoFromDb(vector<ROOM_MANAGER_INFO> &vecRoomManagerInfo, Json::Value &vec);
    BOOL SelectRoomRepairInfoFromDb(vector<ROOM_REPAIR_INFO> &vecRepairInfo, Json::Value &vec);
    BOOL SelectTeacherInfoFromDb(vector<TEACHER_INFO> &vecTeacherInfo, Json::Value &vec);
    BOOL SelectStudentInfoFromDb(vector<STUDETN_INFO> &vecStudentInfo, Json::Value &vec);
    BOOL SelectAttendenceInfoFromDb(vector<ATTENDENCE_INFO> &vecAttendenceInfo, Json::Value &vec);
    BOOL SelectScheduleFromDb(vector<SCHEDULE_DATA> &vecSheduleInfo, Json::Value &vec);
    BOOL SelectReportDataFromDb(vector<REPORT_DATA> &vecReportData, Json::Value &vec);
    BOOL SelectAnnounceFromDb(vector<ANNOUNCE_DATA> &vecAnnounceData, Json::Value &vec);
    BOOL SelectRegisterInfoFromDb(vector<ADD_PERSON_DATA> &vecRegisterData, Json::Value &vec);
    BOOL SelectExamInfoFromDb(vector<EXAM_INFO>& vecExamInfo, Json::Value &vec);
    BOOL SelectWorkloadSalaryInfoFromDb(vector<WORKLOAD_REPORT>& vecSalaryInfo, Json::Value &vec);
    BOOL SelectAssertHandoverFromDb(vector<ASSERT_DATA>& vecAssertDataInfo, Json::Value &vec);

    BOOL InsertNewAnnounceToDb(CString strText, int type);

    BOOL UpdateAdminPwdToDb(CString strPwd);
    BOOL UpdatePersonPwdToDb(UPDATE_PERSON_PWD *pPersonPwd);

    //void InitSocket();
    char* NotNull(char* str);
    void PraseJson(char * buf);
    DECLARE_MESSAGE_MAP()

public:
    BOOL InitThread();
    void SendMsgToDlg(CString str);
    void SetMainHwnd(HWND hWnd) { m_mainHwnd = hWnd; }
    void PersonIsAlreadyExist();

private:
    HANDLE                      m_hThread;
    //SOCKET                      m_socket;
    MYSQL                       m_mysql;
    vector<LOGIN_RESULT>        m_vecLoginResult;
    HWND                        m_tmpHwnd;
    HWND                        m_mainHwnd;
    //CSocketSrv                  m_sock;
    CString                     m_strIP;
    CString                     m_strPort;
};



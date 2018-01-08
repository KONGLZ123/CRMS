// WinThreadDatabase.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "WinThreadDatabase.h"

// CWinThreadDatabase


IMPLEMENT_DYNCREATE(CWinThreadDatabase, CWinThread)

CWinThreadDatabase::CWinThreadDatabase()
{
    m_hThread = NULL;
    m_strIP = _T("127.0.0.1");
    m_strPort = _T("6666");
}

CWinThreadDatabase::~CWinThreadDatabase()
{
    //m_sock.Close();

    if (NULL != m_hThread)
        TerminateThread(m_hThread, 0);
}

BEGIN_MESSAGE_MAP(CWinThreadDatabase, CWinThread)
    ON_THREAD_MESSAGE(WM_RECV, OnRecv)
    ON_THREAD_MESSAGE(WM_LOGIN, OnLoginRequest)
    ON_THREAD_MESSAGE(WM_INIT_DATA, OnInitData)
    ON_THREAD_MESSAGE(WM_DECLARE_ANNOUNCE, OnDeclareAnnounce)
    ON_THREAD_MESSAGE(WM_DEL_ANNOUNCE, OnDeleteAnnounce)
    ON_THREAD_MESSAGE(WM_ADMIN_SETUP_PWD, OnAdminSetupPwd)
    ON_THREAD_MESSAGE(WM_ADD_PERSON, OnAddPerson)
    ON_THREAD_MESSAGE(WM_SETUP_PWD, OnSetupPwd)
    ON_THREAD_MESSAGE(WM_INSERT_REPORT, OnInsertReport)
    ON_THREAD_MESSAGE(WM_SETUP_SERVER, OnSetupServer)
    ON_THREAD_MESSAGE(WM_DEL_PERSON_INFO, OnDelPersonInof)
    ON_THREAD_MESSAGE(WM_UPDATE_PERSON_INFO, OnUpdatePersonInof)
    ON_THREAD_MESSAGE(WM_DEL_REGISTER, OnDelRegister)
    ON_THREAD_MESSAGE(WM_DECALRE_EXAM_INFO, OnDeclareExamInfo)
    ON_THREAD_MESSAGE(WM_UPDATE_CLASS_DATA, OnSetClassData)
    ON_THREAD_MESSAGE(WM_INSERT_WORKLOAD_REPORT, OnInsertWorkload)
    ON_THREAD_MESSAGE(WM_INSERT_ASSERT_DATA, OnInsertAssertData)
    ON_THREAD_MESSAGE(WM_UPDATE_ROOM_REQUEST, OnUpdateRoomRequest)
    ON_THREAD_MESSAGE(WM_UPDATE_EXAM_INFO, OnUpdateExamInfo)
    ON_THREAD_MESSAGE(WM_UPDATE_SALARY_INFO, OnUpdateSalaryInfo)
END_MESSAGE_MAP()

BOOL CWinThreadDatabase::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	return TRUE;
}

int CWinThreadDatabase::ExitInstance()
{
    //mysql_close(&m_mysql);//关闭sql

	return CWinThread::ExitInstance();
}

void CWinThreadDatabase::OnRecv(WPARAM wParam, LPARAM lParam)
{
    CString *pStr = (CString *)wParam;

    USES_CONVERSION;
    char *pBuf = W2A(*pStr);
    PraseJson(pBuf);

    delete pStr;
    pStr = NULL;
}

void CWinThreadDatabase::OnLoginRequest(WPARAM wParam, LPARAM lParam)
{
    LOGIN_REQUEST *ptagLoginRequest = (LOGIN_REQUEST *)wParam;

    m_curHwnd = ptagLoginRequest->hWnd;
    SendLoginRequestToServer(ptagLoginRequest->strAccount, ptagLoginRequest->strPwd, ptagLoginRequest->role);
    //GetLoginData(ptagLoginRequest);
    //CheckPwd(ptagLoginRequest);

    delete ptagLoginRequest;
    ptagLoginRequest = NULL;
}

BOOL CWinThreadDatabase::SendMsgToServer(Json::Value root)
{
    // 格式化成json数据
    Json::FastWriter writer;
    std::string out = writer.write(root);
    const char * str = out.c_str();

    //m_sock.SendMsgToServer((LPSTR)str, strlen(str));
    m_pSocket->SendMsgToServer((LPSTR)str, strlen(str));

    return TRUE;
}

BOOL CWinThreadDatabase::SendLoginRequestToServer(CString strAccount, CString strPwd, int role)
{
    USES_CONVERSION;
    char *pAccount = W2A(strAccount);
    char *pPwd = W2A(strPwd);

    Json::Value root;
    root["code"] = S_CODE_LOGIN;
    root["account"] = pAccount;
    root["pwd"] = pPwd;
    root["role"] = role;

    if (!SendMsgToServer(root))
        return FALSE;

    return TRUE;
}

void CWinThreadDatabase::OnInitData(WPARAM wParam, LPARAM lParam)
{
    m_curHwnd = (HWND)lParam;

    Json::Value root;
    root["code"] = S_CODE_INIT_DATA;
    //root["role"] = role;

    SendMsgToServer(root);
}

void CWinThreadDatabase::OnDeclareAnnounce(WPARAM wParam, LPARAM lParam)
{
    ANNOUNCE_DATA *pAnnouce = (ANNOUNCE_DATA *)wParam;
    HWND hWnd = (HWND)lParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_INSERT_ANNOUNCE;
    root["type"] = pAnnouce->type;
    root["text"] = W2A(pAnnouce->strText);

    SendMsgToServer(root);

    delete pAnnouce;
    pAnnouce = NULL;
}

void CWinThreadDatabase::OnDeleteAnnounce(WPARAM wParam, LPARAM lParam)
{
    int index = (int)wParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_DELETE_OLD_ANNOUNCE;
    root["index"] = index;

    SendMsgToServer(root);
}

void CWinThreadDatabase::OnAdminSetupPwd(WPARAM wParam, LPARAM lParam)
{
    CString *pStrPwd = (CString *)wParam;
    
    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_UPDATE_ADMIN_PWD;
    root["pwd"] = W2A(*pStrPwd);

    SendMsgToServer(root);

    delete pStrPwd;
    pStrPwd;
}

void CWinThreadDatabase::OnSetupPwd(WPARAM wParam, LPARAM lParam)
{
    UPDATE_PERSON_PWD *pPersonPwd = (UPDATE_PERSON_PWD *)wParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_UPDATE_PERSON_PWD;
    root["account"] = W2A(pPersonPwd->account);
    root["pwd"] = W2A(pPersonPwd->newPwd);
    root["authority"] = pPersonPwd->authrity;

    SendMsgToServer(root);

    delete pPersonPwd;
    pPersonPwd = NULL;
}

void CWinThreadDatabase::OnSetupServer(WPARAM wParam, LPARAM lParam)
{
    SERVER_SETUP *pServerSetup = (SERVER_SETUP *)wParam;

    m_strIP = pServerSetup->strIP;
    m_strPort = pServerSetup->strPort;

    delete pServerSetup;
    pServerSetup = NULL;
}

void CWinThreadDatabase::OnAddPerson(WPARAM wParam, LPARAM lParam)
{
    ADD_PERSON_DATA *ptagAddPerson = (ADD_PERSON_DATA *)wParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_INSERT_NEW_ACCOUNT;
    root["account"] = W2A(ptagAddPerson->account);
    root["pwd"] = W2A(ptagAddPerson->pwd);
    root["role"] = ptagAddPerson->authority;
    root["is_register"] = ptagAddPerson->bIsRegister;
    root["grade"] = W2A(ptagAddPerson->grade);
    root["major"] = W2A(ptagAddPerson->major);
    //root["stu_id"] = W2A(ptagAddPerson->id);
    root["name"] = W2A(ptagAddPerson->name);
    root["tel"] = W2A(ptagAddPerson->tel);
    root["course"] = W2A(ptagAddPerson->course);

    SendMsgToServer(root);

    delete ptagAddPerson;
    ptagAddPerson = NULL;
}

void CWinThreadDatabase::OnInsertReport(WPARAM wParam, LPARAM lParam)
{
    REPORT_DATA *pReportData = (REPORT_DATA *)wParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_INSERT_REPORT;
    root["type"] = pReportData->reportType;
    root["reason"] = W2A(pReportData->reason);
    root["submit_person"] = W2A(pReportData->submitPerson);
    root["upload_date"] = W2A(pReportData->uploadDate);
    root["room_id"] = W2A(pReportData->roomId);
    root["notes"] = W2A(pReportData->notes);
    root["is_view"] = 0;

    SendMsgToServer(root);

    delete pReportData;
    pReportData = NULL;
}

void CWinThreadDatabase::OnDelPersonInof(WPARAM wParam, LPARAM lParam)
{
    PERSON_INFO_PART *pPersonInfo = (PERSON_INFO_PART *)wParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_DEL_PERSON_INFO;
    root["account"] = W2A(pPersonInfo->account);
    root["authority"] = pPersonInfo->authority;

    SendMsgToServer(root);

    delete pPersonInfo;
    pPersonInfo = NULL;
}

void CWinThreadDatabase::OnUpdatePersonInof(WPARAM wParam, LPARAM lParam)
{
    PERSON_INFO_PART *pPersonInfo = (PERSON_INFO_PART *)wParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_UPDATE_PERSON_INFO;
    root["account"] = W2A(pPersonInfo->account);
    root["pwd"] = W2A(pPersonInfo->pwd);
    root["name"] = W2A(pPersonInfo->name);
    root["tel"] = W2A(pPersonInfo->tel);
    root["authority"] = pPersonInfo->authority;
    root["old_authority"] = pPersonInfo->oldAuthority;

    SendMsgToServer(root);

    delete pPersonInfo;
    pPersonInfo = NULL;
}

void CWinThreadDatabase::OnDelRegister(WPARAM wParam, LPARAM lParam)
{
    CString* pAccount = (CString*)wParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_DEL_REGISTER;
    root["account"] = W2A(*pAccount);

    SendMsgToServer(root);

    delete pAccount;
    pAccount = NULL;
}

void CWinThreadDatabase::OnDeclareExamInfo(WPARAM wParam, LPARAM lParam)
{
    EXAM_INFO* pExamInfo = (EXAM_INFO*)wParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_INSERT_EXAM_INFO;
    root["person"] = W2A(pExamInfo->strPerson);
    root["text"] = W2A(pExamInfo->strPerson);
    root["date"] = W2A(pExamInfo->strDate);
    root["class"] = W2A(pExamInfo->strClass);

    SendMsgToServer(root);

    delete pExamInfo;
    pExamInfo = NULL;
}

void CWinThreadDatabase::OnSetClassData(WPARAM wParam, LPARAM lParam)
{
    CLASS_DATA* pClassData = (CLASS_DATA*)wParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_INSERT_CLASS_INFO;
    root["week"] = pClassData->week;
    root["day"] = pClassData->day;
    root["class_num"] = pClassData->class_num;
    root["class_text"] = W2A(pClassData->class_text);

    SendMsgToServer(root);

    delete pClassData;
    pClassData = NULL;
}

void CWinThreadDatabase::OnInsertWorkload(WPARAM wParam, LPARAM lParam)
{
    WORKLOAD_REPORT* pReportData = (WORKLOAD_REPORT*)wParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_INSERT_WORKLOAD;
    root["name"] = W2A(pReportData->strName);
    root["reason"] = W2A(pReportData->strReason);
    root["date"] = W2A(pReportData->strDate);
    root["fix_salary"] = W2A(pReportData->strFixSalary);
    root["workload_salary"] = W2A(pReportData->strWorkloadSalary);
    root["is_view"] = 0;

    SendMsgToServer(root);

    delete pReportData;
    pReportData = NULL;
}

void CWinThreadDatabase::OnInsertAssertData(WPARAM wParam, LPARAM lParam)
{
    ASSERT_DATA* pReportData = (ASSERT_DATA*)wParam;

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_INSERT_ASSERT_DATA;
    root["device_name"] = W2A(pReportData->strDeviceName);
    root["owner"] = W2A(pReportData->strOwner);
    root["room_num"] = W2A(pReportData->strRoomNum);
    root["in_date"] = W2A(pReportData->strInDate);
    root["out_date"] = W2A(pReportData->strOutDate);
    root["status"] = W2A(pReportData->strStatus);
    root["reason"] = W2A(pReportData->strReason);
    root["is_view"] = 0;

    SendMsgToServer(root);

    delete pReportData;
    pReportData = NULL;
}

void CWinThreadDatabase::OnUpdateRoomRequest(WPARAM wParam, LPARAM lParam)
{
    REPORT_DATA *pReportData = reinterpret_cast<REPORT_DATA *>(wParam);
    
    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_UPDATE_ROOM_REQUEST;
    root["submit_person"] = W2A(pReportData->submitPerson);
    root["upload_date"] = W2A(pReportData->uploadDate);
    root["is_view"] = pReportData->isView;

    SendMsgToServer(root);

    delete pReportData;
    pReportData = NULL;
}

void CWinThreadDatabase::OnUpdateExamInfo(WPARAM wParam, LPARAM lParam)
{
    ATTENDENCE_INFO *pStuInfo = reinterpret_cast<ATTENDENCE_INFO *>(wParam);

    Json::Value root;

    USES_CONVERSION;
    char *pStuId = W2A(pStuInfo->student_id);
    root["code"] = S_CODE_UPDATE_EXAM_INFO;
    root["student_id"] = pStuId;
    root["attendece_cnt"] = pStuInfo->attendece_cnt;
    root["attendece_score"] = pStuInfo->attendece_score;

    SendMsgToServer(root);

    delete pStuInfo;
    pStuInfo = NULL;
}

void CWinThreadDatabase::OnUpdateSalaryInfo(WPARAM wParam, LPARAM lParam)
{
    WORKLOAD_REPORT *pWorkload = reinterpret_cast<WORKLOAD_REPORT *>(wParam);

    Json::Value root;

    USES_CONVERSION;
    root["code"] = S_CODE_UPDATE_SALARY_INFO;
    root["name"] = W2A(pWorkload->strName);
    root["date"] = W2A(pWorkload->strDate);
    root["workload"] = W2A(pWorkload->strWorkloadSalary);
    root["is_view"] = pWorkload->isView;

    SendMsgToServer(root);

    delete pWorkload;
    pWorkload = NULL;
}

BOOL CWinThreadDatabase::GetLoginData(LOGIN_REQUEST *ptagLoginRequest)
{
    m_vecLoginResult.clear();

    char * pszSql = NULL;
    switch (ptagLoginRequest->role)
    {
    case ADMIN:{
        pszSql = SELECT_ADMIN_LOING;
    }
    break;
    case MANAGER:{
        pszSql = SELECT_ROOM_NANAGER_LOGIN;
    }
    break;
    case TEACHER:{
        pszSql = SELECT_TEACHER_LOING;
    }
    break;
    case REPAIR:{
        pszSql = SELECT_ROOM_REPAIR_LOGIN;
    }
    break;
    case STUDENT:{
        pszSql = SELECT_STUDENT_LOGIN;
    }
    break;
    default:
        break;
    }

    MYSQL_RES *result;
    MYSQL_ROW sql_row;
    //MYSQL_FIELD *fd;
    mysql_query(&m_mysql, "SET NAMES 'gbk'");//设置数据库字符格式，解决中文乱码问题
    //int res = mysql_query(&m_mysql, "select * from usertable");//执行sql命令

    //char * pszSql = "select * from usertable";
    int res = mysql_real_query(&m_mysql, pszSql, (unsigned long)strlen(pszSql));// 查询数据库中的"物料属性"表  
    if (!res)
    {
        result = mysql_store_result(&m_mysql);    //保存查询到的数据到result
        if (result)
        {
            //int i, j;
            //for (i = 0; fd = mysql_fetch_field(result); i++)    //获取列名
            //{
            //    //str[i].Format("s", fd->name);
            //    //ForShow = ForShow + str[i] + "\t";
            //}

            int j = mysql_num_fields(result);	 // 获取列数
            while (sql_row = mysql_fetch_row(result))    //获取具体的数据
            {
                LOGIN_RESULT loginRes;
                for (int i = 0; i < j; i++)
                {
                    if (0 == i)
                    {
                        USES_CONVERSION;
                        loginRes.strAccount = A2W(sql_row[i]);
                        //loginRes.strAccount.Format(_T("%s"), sql_row[i]);
                    }
                    else
                    {
                        USES_CONVERSION;
                        loginRes.strPwd = A2W(sql_row[i]);
                        //loginRes.strPwd.Format(_T("%s"), sql_row[i]);
                        m_vecLoginResult.push_back(loginRes);
                    }
                }
            }
            if (result != NULL)
                mysql_free_result(result);
        }
    }
    else
    {
        MessageBox(NULL, _T("query sql failed!"), _T("错误提示"), MB_OK);
    }

    return 0;
}

void CWinThreadDatabase::CheckPwd(LOGIN_REQUEST * ptagLoginRequest)
{
    CString *pStr = new CString(_T("test"));
    PostMessage(ptagLoginRequest->hWnd, WM_LOGIN_SUCCESS, (WPARAM)pStr, 0);

    //int size = m_vecLoginResult.size();
    //for (int i = 0; i < size; i++)
    //{
    //    if (ptagLoginRequest->strAccount == m_vecLoginResult.at(i).strAccount && 
    //        ptagLoginRequest->strPwd == m_vecLoginResult.at(i).strPwd)
    //    {
    //        CString *pStr = new CString(ptagLoginRequest->strAccount);
    //        PostMessage(ptagLoginRequest->hWnd, WM_LOGIN_SUCCESS, (WPARAM)pStr, 0);
    //        return;
    //    }
    //}
    //PostMessage(ptagLoginRequest->hWnd, WM_LOGIN_ERROR, 0, 0);
}

BOOL CWinThreadDatabase::SelectAdminInfoFromDb(vector<SYSADMIN_INFO> &vecAdminInfo, Json::Value & root)
{
    vecAdminInfo.clear();

    for (unsigned int i = 0; i < root["adminInfo"].size(); i++)
    {
        SYSADMIN_INFO sysAdminInfo;
        sysAdminInfo.id = root["adminInfo"][i]["id"].asInt();
        sysAdminInfo.account = CString(root["adminInfo"][i]["account"].asString().c_str());
        sysAdminInfo.pwd = CString(root["adminInfo"][i]["pwd"].asString().c_str());
        vecAdminInfo.push_back(sysAdminInfo);
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectRoomManagerInfoFromDb(vector<ROOM_MANAGER_INFO> &vecRoomManagerInfo, Json::Value & root)
{
    vecRoomManagerInfo.clear();

    for (unsigned int i = 0; i < root["managerInfo"].size(); i++)
    {
        ROOM_MANAGER_INFO roomManagerInfo;

        roomManagerInfo.id = root["managerInfo"][i]["id"].asInt();
        roomManagerInfo.account = CString(root["managerInfo"][i]["account"].asString().c_str());
        roomManagerInfo.pwd = CString(root["managerInfo"][i]["pwd"].asString().c_str());
        roomManagerInfo.name = CString(root["managerInfo"][i]["name"].asString().c_str());
        roomManagerInfo.tel = CString(root["managerInfo"][i]["tel"].asString().c_str());
        roomManagerInfo.authority = root["managerInfo"][i]["authority"].asInt();

        vecRoomManagerInfo.push_back(roomManagerInfo);
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectRoomRepairInfoFromDb(vector<ROOM_REPAIR_INFO> &vecRepairInfo, Json::Value & root)
{
    vecRepairInfo.clear();

    for (unsigned int i = 0; i < root["repairInfo"].size(); i++)
    {
        ROOM_REPAIR_INFO roomRepairInfo;

        roomRepairInfo.id = root["repairInfo"][i]["id"].asInt();
        roomRepairInfo.account = CString(root["repairInfo"][i]["account"].asString().c_str());
        roomRepairInfo.pwd = CString(root["repairInfo"][i]["pwd"].asString().c_str());
        roomRepairInfo.name = CString(root["repairInfo"][i]["name"].asString().c_str());
        roomRepairInfo.tel = CString(root["repairInfo"][i]["tel"].asString().c_str());
        roomRepairInfo.authority = root["repairInfo"][i]["authority"].asInt();

        vecRepairInfo.push_back(roomRepairInfo);
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectTeacherInfoFromDb(vector<TEACHER_INFO> &vecTeacherInfo, Json::Value & root)
{
    vecTeacherInfo.clear();

    for (unsigned int i = 0; i < root["teacherInfo"].size(); i++)
    {
        TEACHER_INFO teacherInfo;

        //teacherInfo.id = root["teacherInfo"][i]["id"].asInt();
        teacherInfo.account = root["teacherInfo"][i]["account"].asCString();
        teacherInfo.pwd = root["teacherInfo"][i]["pwd"].asCString();
        teacherInfo.name = root["teacherInfo"][i]["name"].asCString();
        teacherInfo.tel = root["teacherInfo"][i]["tel"].asCString();
        teacherInfo.authority = root["teacherInfo"][i]["authority"].asInt();
        //teacherInfo.classes = root["teacherInfo"][i]["classes_id"].asCString();
        teacherInfo.course = root["teacherInfo"][i]["course_id"].asCString();

        vecTeacherInfo.push_back(teacherInfo);
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectStudentInfoFromDb(vector<STUDETN_INFO> &vecStudentInfo, Json::Value & root)
{
    vecStudentInfo.clear();

    for (unsigned int i = 0; i < root["studentInfo"].size(); i++)
    {
        STUDETN_INFO studentInfo;

        studentInfo.student_id = CString(root["studentInfo"][i]["student_id"].asString().c_str());
        studentInfo.pwd = CString(root["studentInfo"][i]["pwd"].asString().c_str());
        studentInfo.name = CString(root["studentInfo"][i]["name"].asString().c_str());
        studentInfo.sex = CString(root["studentInfo"][i]["sex"].asString().c_str());
        studentInfo.tel = CString(root["studentInfo"][i]["tel"].asString().c_str());
        studentInfo.classes = CString(root["studentInfo"][i]["classes"].asString().c_str());
        studentInfo.major = CString(root["studentInfo"][i]["major"].asString().c_str());
        //studentInfo.college = CString(root["studentInfo"][i]["college"].asString().c_str());
        //studentInfo.exam = CString(root["studentInfo"][i]["exam"].asString().c_str());
        studentInfo.authority = root["studentInfo"][i]["authority"].asInt();
        //studentInfo.attendece_cnt = root["studentInfo"][i]["attendece_cnt"].asInt();
        //studentInfo.attendece_score = root["studentInfo"][i]["attendece_score"].asInt();

        vecStudentInfo.push_back(studentInfo);
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectAttendenceInfoFromDb(vector<ATTENDENCE_INFO>& vecAttendenceInfo, Json::Value & root)
{
    vecAttendenceInfo.clear();

    for (unsigned int i = 0; i < root["attendenceInfo"].size(); i++)
    {
        ATTENDENCE_INFO attendenceInfo;

        attendenceInfo.student_id = root["attendenceInfo"][i]["student_id"].asCString();
        attendenceInfo.course_name = root["attendenceInfo"][i]["course_name"].asCString();
        attendenceInfo.teacher_name = root["attendenceInfo"][i]["teacher_name"].asCString();
        attendenceInfo.student_name = root["attendenceInfo"][i]["student_name"].asCString();
        attendenceInfo.attendece_cnt = root["attendenceInfo"][i]["attendece_cnt"].asInt();
        attendenceInfo.attendece_score = root["attendenceInfo"][i]["attendece_score"].asInt();

        vecAttendenceInfo.push_back(attendenceInfo);
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectScheduleFromDb(vector<CLASS_DATA>& vecSheduleInfo, Json::Value & root)
{
    vecSheduleInfo.clear();

    for (unsigned int i = 0; i < root["classData"].size(); i++)
    {
        CLASS_DATA classData;

        classData.week = root["classData"][i]["week"].asInt();
        classData.day = root["classData"][i]["day"].asInt();
        //classData.class_num = root["classData"][i]["class_num"].asInt();
        //classData.class_text = root["classData"][i]["class_text"].asCString();
        //classData.schedule_id = root["classData"][i]["schedule_id"].asCString();
        classData.class_12 = root["classData"][i]["class_12"].asCString();
        classData.class_34 = root["classData"][i]["class_34"].asCString();
        classData.class_56 = root["classData"][i]["class_56"].asCString();
        classData.class_78 = root["classData"][i]["class_78"].asCString();

        vecSheduleInfo.push_back(classData);
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectReportDataFromDb(vector<REPORT_DATA>& vecReportData, Json::Value & root)
{
    vecReportData.clear();

    for (unsigned int i = 0; i < root["reportInfo"].size(); i++)
    {
        REPORT_DATA reportData;

        reportData.id = root["reportInfo"][i]["id"].asInt();
        reportData.reportType = (REPORT_TYPE)root["reportInfo"][i]["reportType"].asInt();
        reportData.submitPerson = CString(root["reportInfo"][i]["submitPerson"].asString().c_str());
        reportData.uploadDate = CString(root["reportInfo"][i]["uploadDate"].asString().c_str());
        reportData.roomId = CString(root["reportInfo"][i]["roomId"].asString().c_str());
        reportData.reason = CString(root["reportInfo"][i]["reason"].asString().c_str());
        reportData.notes = CString(root["reportInfo"][i]["notes"].asString().c_str());
        reportData.isView = root["reportInfo"][i]["isView"].asInt();

        vecReportData.push_back(reportData);
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectAnnounceFromDb(vector<ANNOUNCE_DATA> &vecAnnounceData, Json::Value & root)
{
    vecAnnounceData.clear();

    for (unsigned int i = 0; i < root["announceInfo"].size(); i++)
    {
        ANNOUNCE_DATA announceData;

        announceData.announce_id = root["announceInfo"][i]["announce_id"].asInt();
        announceData.strText = CString(root["announceInfo"][i]["strText"].asString().c_str());
        announceData.type = root["announceInfo"][i]["type"].asInt();

        vecAnnounceData.push_back(announceData);
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectRegisterInfoFromDb(vector<ADD_PERSON_DATA>& vecRegisterData, Json::Value & root)
{
    vecRegisterData.clear();

    for (unsigned int i = 0; i < root["registerInfo"].size(); i++)
    {
        ADD_PERSON_DATA registerData;

        registerData.account = CString(root["registerInfo"][i]["account"].asString().c_str());
        registerData.pwd = CString(root["registerInfo"][i]["pwd"].asString().c_str());
        registerData.name = CString(root["registerInfo"][i]["name"].asString().c_str());
        registerData.tel = CString(root["registerInfo"][i]["tel"].asString().c_str());
        registerData.authority = root["registerInfo"][i]["authority"].asInt();

        vecRegisterData.push_back(registerData);
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectExamInfoFromDb(vector<EXAM_INFO>& vecExamInfo, Json::Value & root)
{
    vecExamInfo.clear();

    for (unsigned int i = 0; i < root["examInfo"].size(); i++)
    {
        EXAM_INFO examInfo;

        examInfo.strClass = CString(root["examInfo"][i]["classes"].asString().c_str());
        examInfo.strPerson = CString(root["examInfo"][i]["person"].asString().c_str());
        examInfo.strText = CString(root["examInfo"][i]["text"].asString().c_str());
        examInfo.strDate = CString(root["examInfo"][i]["date"].asString().c_str());

        vecExamInfo.push_back(examInfo);
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectWorkloadSalaryInfoFromDb(vector<WORKLOAD_REPORT>& vecSalaryInfo, Json::Value & root)
{
    vecSalaryInfo.clear();

    for (unsigned int i = 0; i < root["salaryInfo"].size(); i++)
    {
        WORKLOAD_REPORT salaryInfo;

        salaryInfo.strDate = root["salaryInfo"][i]["date"].asCString();
        salaryInfo.strFixSalary = root["salaryInfo"][i]["fix_salary"].asCString();
        salaryInfo.strName = root["salaryInfo"][i]["name"].asCString();
        salaryInfo.strReason = root["salaryInfo"][i]["reason"].asCString();
        salaryInfo.strWorkloadSalary = root["salaryInfo"][i]["workload_salary"].asCString();
        salaryInfo.result = root["salaryInfo"][i]["workload_salary"].asInt();
        salaryInfo.isView = root["salaryInfo"][i]["workload_salary"].asInt();

        vecSalaryInfo.push_back(salaryInfo);
    }

    return TRUE;
}


BOOL CWinThreadDatabase::SelectAssertHandoverFromDb(vector<ASSERT_DATA>& vecAssertDataInfo, Json::Value & root)
{
    vecAssertDataInfo.clear();

    for (unsigned int i = 0; i < root["assertInfo"].size(); i++)
    {
        ASSERT_DATA assertInfo;

        assertInfo.strDeviceName = CString(root["assertInfo"][i]["device_name"].asString().c_str());
        assertInfo.strInDate = CString(root["assertInfo"][i]["in_date"].asString().c_str());
        assertInfo.strOutDate = CString(root["assertInfo"][i]["out_date"].asString().c_str());
        assertInfo.strOwner = CString(root["assertInfo"][i]["owner"].asString().c_str());
        assertInfo.strReason = CString(root["assertInfo"][i]["reason"].asString().c_str());
        assertInfo.strRoomNum = CString(root["assertInfo"][i]["room_num"].asString().c_str());
        assertInfo.strStatus = CString(root["assertInfo"][i]["status"].asString().c_str());

        vecAssertDataInfo.push_back(assertInfo);
    }

    return TRUE;
}


BOOL CWinThreadDatabase::ConnectServer()
{
    m_pSocket = new CSocketClient();
    if (!m_pSocket->Create())
    {
        CString strErr;
        strErr.Format(_T("Socket create error : %d"), m_pSocket->GetLastError());
        ::MessageBox(NULL, strErr, _T("错误提示"), MB_OK);
        return FALSE;
    }

    //m_sock.SetHwnd(this->m_hWnd);
    m_pSocket->SetThread(this);

    USES_CONVERSION;
    int nPort = atoi(W2A(m_strPort));
    if (!m_pSocket->Connect(m_strIP, nPort))
    {
        CString strErr;
        strErr.Format(_T("Connect server error : %d"), m_pSocket->GetLastError());
        ::MessageBox(NULL, strErr, _T("错误提示"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::InitThread()
{
    if (CreateThread())
    {
        ConnectServer();
    }

    return TRUE;
}



void CWinThreadDatabase::PraseJson(char * buf)
{
    Json::Reader reader;
    Json::Value root;
    if (reader.parse(buf, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素   
    {
        int code = root["code"].asInt();  // 访问节点，upload_id = "UP000000"   
        switch (code)
        {
        case S_CODE_LOGIN_SUCCESS:  // 登录
        {
            //CString *pStr = new CString(_T("test"));
            PostMessage(m_curHwnd, WM_LOGIN_SUCCESS, 0, 0);

            //CString *pStr = new CString(_T("test"));
            //PostMessage(m_curHwnd, WM_LOGIN_SUCCESS, (WPARAM)pStr, 0);
        }
        break;
        case S_CODE_LOGIN_FAILED:
        {
            //GetDlgItem(IDC_STATIC_LOGIN_FAIL)->SetWindowText(_T("用户名或密码错误"));
            PostMessage(m_curHwnd, WM_LOGIN_ERROR, 0, 0);
        }
        break;
        case S_CODE_INIT_DATA_SUCCESS:
        {
            //int role = (int)wParam;
            //HWND hWnd = (HWND)lParam;

            DATA_STOCK *pDataStock = new DATA_STOCK;

            SelectAdminInfoFromDb(pDataStock->vecAdminInfo, root);
            SelectRoomManagerInfoFromDb(pDataStock->vecRoomManagerInfo, root);
            SelectRoomRepairInfoFromDb(pDataStock->vecRoomRepairInfo, root);
            SelectTeacherInfoFromDb(pDataStock->vecTeacherInfo, root);
            SelectStudentInfoFromDb(pDataStock->vecStudentInfo, root);
            SelectAttendenceInfoFromDb(pDataStock->vecAttendenceInfo, root);
            SelectReportDataFromDb(pDataStock->vecReportData, root);
            SelectAnnounceFromDb(pDataStock->vecAnnounceData, root);
            SelectScheduleFromDb(pDataStock->vecScheduleData, root);
            SelectRegisterInfoFromDb(pDataStock->vecRegisterData, root);
            SelectExamInfoFromDb(pDataStock->vecExamInfo, root);
            SelectWorkloadSalaryInfoFromDb(pDataStock->vecSalaryInfo, root);
            SelectAssertHandoverFromDb(pDataStock->vecAssertDataInfo, root);


            PostMessage(m_curHwnd, WM_INITDATA_SUCCESS, (WPARAM)pDataStock, 0);
        }
        break;
        case S_CODE_INIT_DATA_FAILED:
        {

        }
        break;
        case S_CODE_PERSON_EXIT_ERROR: {
            PostMessage(m_curHwnd, WM_PERSON_EXIST_ERROR, 0, 0);
            break;
        }
        default:
            break;
        }
    }
}


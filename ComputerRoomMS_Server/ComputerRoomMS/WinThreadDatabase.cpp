// WinThreadDatabase.cpp : 实现文件
//

#include "stdafx.h"
#include "ComputerRoomMS.h"
#include "WinThreadDatabase.h"
#include "json.h"
#include "data.h"

// CWinThreadDatabase


UINT ThreadProc(LPVOID pParam)
{
    //SOCK_DATA *pData = (SOCK_DATA *)pParam;
    //SOCKET socket = pData->socket;
    //CWinThreadDatabase *pDbThread = pData->pDbThread;

    //delete pData;
    //pData = NULL;

    //int nRet = listen(socket, SOMAXCONN);
    //if (0 != nRet)
    //{
    //    int err = WSAGetLastError();
    //    CString str;
    //    str.Format(_T("listen error : %d"), err);
    //    ::MessageBox(NULL, str, _T("错误"), MB_OK);
    //    return 1;
    //}

    //SOCKADDR_IN sockRecvAddr;
    //int addrLen = sizeof(SOCKADDR);
    //SOCKET clientSock = INVALID_SOCKET;
    //char szBuf[1024];

    //while (1)
    //{
    //    ZeroMemory(szBuf, 0);
    //    clientSock = accept(socket, (SOCKADDR *)&sockRecvAddr, &addrLen);
    //    if (INVALID_SOCKET == clientSock)
    //    {
    //        int err = WSAGetLastError();
    //        CString str;
    //        str.Format(_T("accept error : %d"), err);
    //        ::MessageBox(NULL, str, _T("错误"), MB_OK);
    //        return 1;
    //    }

    //    int recvLen = recv(clientSock, szBuf, sizeof(szBuf), 0);
    //    szBuf[recvLen] = '\0';

    //    CString *pStr = new CString(szBuf);
    //    PostThreadMessage(pDbThread->m_nThreadID, WM_RECV, (WPARAM)pStr, 0);
    //    //::PostMessage(hWnd, WM_RECV, (WPARAM)pBuf, 0);
    //}

    //closesocket(clientSock);

    return 0;
}

IMPLEMENT_DYNCREATE(CWinThreadDatabase, CWinThread)

CWinThreadDatabase::CWinThreadDatabase()
{
    m_hThread = NULL;
}

CWinThreadDatabase::~CWinThreadDatabase()
{
    //if (INVALID_SOCKET == m_socket)
    //    closesocket(m_socket);
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
    ON_THREAD_MESSAGE(WM_SETUP_PWD, OnSetupPwd)
    ON_THREAD_MESSAGE(WM_REQUEST_REPAIR, OnRequestRepair)
    ON_THREAD_MESSAGE(WM_DEL_PERSON_INFO, OnDelPersonInof)
    ON_THREAD_MESSAGE(WM_UPDATE_PERSON_INFO, OnUpdatePersonInof)
    ON_THREAD_MESSAGE(WM_DEL_REGISTER, OnDelRegister)
    ON_THREAD_MESSAGE(WM_DECALRE_EXAM_INFO, OnDeclareExamInfo)
    ON_THREAD_MESSAGE(WM_SET_CLASS_DATA, OnSetClassData)
    ON_THREAD_MESSAGE(WM_INSERT_WORKLOAD_REPORT, OnInsertWorkload)
    ON_THREAD_MESSAGE(WM_INSERT_ASSERT_DATA, OnInsertAssertData)
    ON_THREAD_MESSAGE(WM_UPDATE_ROOM_REQUEST, OnUpdateRoomRequest)
    ON_THREAD_MESSAGE(WM_UPDATE_SALARY_INFO, OnUpdateSalaryInfo)
    ON_THREAD_MESSAGE(WM_ADD_PERSON, OnAddPerson)
    ON_THREAD_MESSAGE(WM_UPDATE_EXAM_INFO, OnUpdateExamInfo)
END_MESSAGE_MAP()

BOOL CWinThreadDatabase::InitInstance()
{
	// TODO:    在此执行任意逐线程初始化
	return TRUE;
}

int CWinThreadDatabase::ExitInstance()
{
    mysql_close(&m_mysql);//关闭sql

	return CWinThread::ExitInstance();
}

void CWinThreadDatabase::OnLoginRequest(WPARAM wParam, LPARAM lParam)
{
    LOGIN_REQUEST *ptagLoginRequest = (LOGIN_REQUEST *)wParam;

    GetLoginData(ptagLoginRequest); // 从数据库查询数据
    CheckPwd(ptagLoginRequest);     // 校验密码

    delete ptagLoginRequest;
    ptagLoginRequest = NULL;
}

BOOL CWinThreadDatabase::SendMsgToServer(Json::Value root)
{
    // 格式化成json数据
    Json::FastWriter writer;
    std::string out = writer.write(root);
    const char * str = out.c_str();

    //int nRet = send(m_socket, str, strlen(str), 0);
    //int nRet = m_sock.Send(str, strlen(str));
    //if (SOCKET_ERROR == nRet)
    //{
    //    int err = WSAGetLastError();
    //    CString str;
    //    str.Format(_T("send error : %d"), err);
    //    ::MessageBox(NULL, str, _T("错误提示"), MB_OK);
    //    return FALSE;
    //}

    return TRUE;
}

BOOL CWinThreadDatabase::SendLoginRequestToServer(LOGIN_REQUEST *ptagLoginRequest)
{
    USES_CONVERSION;
    char *pAccount = W2A(ptagLoginRequest->strAccount);
    char *pPwd = W2A(ptagLoginRequest->strPwd);

    Json::Value root;
    root["code"] = S_CODE_LOGIN;
    root["account"] = pAccount;
    root["pwd"] = pPwd;
    root["role"] = ptagLoginRequest->role;

    if (!SendMsgToServer(root))
        return FALSE;

    return TRUE;
}


void CWinThreadDatabase::OnInitData(WPARAM wParam, LPARAM lParam)
{
    int role = (int)wParam;
    HWND hWnd = (HWND)lParam;

    DATA_STOCK *pDataStock = new DATA_STOCK;

    Json::Value root;
    Json::Value tmpArr;
    Json::Value adminInfo;

    root["code"] = S_CODE_INIT_DATA_SUCCESS;

    SelectAdminInfoFromDb(pDataStock->vecAdminInfo, tmpArr);
    root["adminInfo"] = tmpArr;
  
    SelectRoomManagerInfoFromDb(pDataStock->vecRoomManagerInfo, tmpArr);
    root["managerInfo"] = tmpArr;

    SelectRoomRepairInfoFromDb(pDataStock->vecRoomRepairInfo, tmpArr);
    root["repairInfo"] = tmpArr;

    SelectTeacherInfoFromDb(pDataStock->vecTeacherInfo, tmpArr);
    root["teacherInfo"] = tmpArr;

    SelectStudentInfoFromDb(pDataStock->vecStudentInfo, tmpArr);
    root["studentInfo"] = tmpArr;

    SelectAttendenceInfoFromDb(pDataStock->vecAttendenceInfo, tmpArr);
    root["attendenceInfo"] = tmpArr;

    SelectReportDataFromDb(pDataStock->vecReportData, tmpArr);
    root["reportInfo"] = tmpArr;

    SelectAnnounceFromDb(pDataStock->vecAnnounceData, tmpArr);
    root["announceInfo"] = tmpArr;

    SelectScheduleFromDb(pDataStock->vecScheduleData, tmpArr);
    root["classData"] = tmpArr;

    SelectRegisterInfoFromDb(pDataStock->vecRegisterData, tmpArr);
    root["registerInfo"] = tmpArr;

    SelectExamInfoFromDb(pDataStock->vecExamInfo, tmpArr);
    root["examInfo"] = tmpArr;

    SelectWorkloadSalaryInfoFromDb(pDataStock->vecSalaryInfo, tmpArr);
    root["salaryInfo"] = tmpArr;

    SelectAssertHandoverFromDb(pDataStock->vecAssertDataInfo, tmpArr);
    root["assertInfo"] = tmpArr;

    Json::Value *pRoot = new Json::Value;
    *pRoot = root;

    PostMessage(hWnd, WM_INITDATA_SUCCESS, (WPARAM)pRoot, 0);
}

void CWinThreadDatabase::OnDeclareAnnounce(WPARAM wParam, LPARAM lParam)
{
    ANNOUNCE_DATA *pAnnouce = (ANNOUNCE_DATA *)wParam;
    HWND hWnd = (HWND)lParam;

    InsertNewAnnounceToDb(pAnnouce->strText, pAnnouce->type);

    delete pAnnouce;
    pAnnouce = NULL;
}

void CWinThreadDatabase::OnDeleteAnnounce(WPARAM wParam, LPARAM lParam)
{
    int index = (int)wParam;

    char *pSql = SELECT_REPORT_DATA;
    char szTemp[200];
    sprintf(szTemp, DELETE_OLD_ANNOUNCE, index);
    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("OnDeleteAnnounce faied！"), _T("错误"), MB_OK);
    }
}

void CWinThreadDatabase::OnAdminSetupPwd(WPARAM wParam, LPARAM lParam)
{
    CString *pStrPwd = (CString *)wParam;
    
    UpdateAdminPwdToDb(*pStrPwd);

    delete pStrPwd;
    pStrPwd;
}

void CWinThreadDatabase::OnSetupPwd(WPARAM wParam, LPARAM lParam)
{
    UPDATE_PERSON_PWD *pPersonPwd = (UPDATE_PERSON_PWD *)wParam;

    UpdatePersonPwdToDb(pPersonPwd);

    delete pPersonPwd;
    pPersonPwd = NULL;
}

void CWinThreadDatabase::OnAddPerson(WPARAM wParam, LPARAM lParam)
{
    ADD_PERSON_DATA *ptagAddPerson = (ADD_PERSON_DATA *)wParam;
    m_tmpHwnd = (HWND)lParam;

    USES_CONVERSION;
    char *pAccount = W2A(ptagAddPerson->account);
    char *pPwd = W2A(ptagAddPerson->pwd);
    char *pName = W2A(ptagAddPerson->name);
    char *pTel = W2A(ptagAddPerson->tel);
    //char *pStuId = W2A(ptagAddPerson->id);
    char *pMajor = W2A(ptagAddPerson->major);
    char *pGrade = W2A(ptagAddPerson->grade);
    char *pCourse = W2A(ptagAddPerson->course);
    int auth = ptagAddPerson->authority;

    char szTemp[1024];

    if (ptagAddPerson->bIsRegister) { // 注册消息
        if (STUDENT == ptagAddPerson->authority) {
            sprintf(szTemp, INSERT_NEW_STU_ACCOUNT, pAccount, pPwd, pName, pTel, pMajor, pGrade, auth, pCourse);
        }
        else if (TEACHER == ptagAddPerson->authority) {
            sprintf(szTemp, INSERT_NEW_TEACHER_ACCOUNT, pAccount, pPwd, pName, pTel, pGrade, auth, pCourse);
        }
        else {
            sprintf(szTemp, INSERT_NEW_ACCOUNT, pAccount, pPwd, pName, pTel, auth);
        }
    }
    else { // 管理员增加人员的消息
        switch (ptagAddPerson->authority) {
        case MANAGER: {
            sprintf(szTemp, INSERT_MANAGER_ACCOUNT, pAccount, pPwd, pName, pTel, auth);
            break;
        }
        case REPAIR: {
            sprintf(szTemp, INSERT_REPAIR_ACCOUNT, pAccount, pPwd, pName, pTel, auth);
            break;
        }
        case TEACHER: {
            sprintf(szTemp, "INSERT t_teacher_info(teacher_id, pwd, teacher_name, tel, authority) VALUES('%s', '%s', '%s', '%s', %d);",
                pAccount, pPwd, pName, pTel, auth);
            int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
            if (0 != res)
                ::MessageBox(NULL, _T("InsertNewAccountToDb faied！"), _T("错误"), MB_OK);

            sprintf(szTemp, "INSERT t_classes(classes_id, teacher_id) VALUES('%s', '%s'); ", pGrade, pAccount);
            res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
            if (0 != res)
                ::MessageBox(NULL, _T("InsertNewAccountToDb faied！"), _T("错误"), MB_OK);

            sprintf(szTemp, "INSERT t_course(course_id, teacher_id) VALUES('%s', '%s'); ", pCourse, pAccount);
            res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
            if (0 != res)
                ::MessageBox(NULL, _T("InsertNewAccountToDb faied！"), _T("错误"), MB_OK);

            delete ptagAddPerson;
            ptagAddPerson = NULL;
            return;
        }
        case STUDENT: {
            char *pTmp = "SET FOREIGN_KEY_CHECKS = 0";
            int res = mysql_real_query(&m_mysql, pTmp, (unsigned long)strlen(pTmp));//插入数据
            if (0 != res)
                ::MessageBox(NULL, _T("InsertNewAccountToDb faied！"), _T("错误"), MB_OK);

            sprintf(szTemp, "INSERT t_student_info(student_id, pwd, user_name, tel, major, class_id, authority) VALUES('%s', '%s', '%s', '%s', '%s', '%s', %d);",
                pAccount, pPwd, pName, pTel, pMajor, pGrade, auth);
            res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
            if (0 != res)
                ::MessageBox(NULL, _T("InsertNewAccountToDb faied！"), _T("错误"), MB_OK);

            res = mysql_real_query(&m_mysql, pTmp, (unsigned long)strlen(pTmp));//插入数据
            if (0 != res)
                ::MessageBox(NULL, _T("InsertNewAccountToDb faied！"), _T("错误"), MB_OK);

            sprintf(szTemp, "INSERT t_student_course(student_id, course_id, attendance_num, score) VALUES('%s', '%s', 0, 0);", pAccount, pCourse);
            res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
            if (0 != res)
                ::MessageBox(NULL, _T("InsertNewAccountToDb faied！"), _T("错误"), MB_OK);

            delete ptagAddPerson;
            ptagAddPerson = NULL;
            return;
            //break;
        }
        default:
            break;
        }
    }

    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("InsertNewAccountToDb faied！"), _T("错误"), MB_OK);
    }

    delete ptagAddPerson;
    ptagAddPerson = NULL;
}

void CWinThreadDatabase::OnRequestRepair(WPARAM wParam, LPARAM lParam)
{
    REPORT_DATA *pReportData = (REPORT_DATA *)wParam;

    USES_CONVERSION;
    int type = pReportData->reportType;
    char *pReason = W2A(pReportData->reason);
    char *pSubmitPerson = W2A(pReportData->submitPerson);
    char *pUploadDate = W2A(pReportData->uploadDate);
    char *pRoomId = W2A(pReportData->roomId);
    char *pNotes = W2A(pReportData->notes);
    pReportData->isView = 0;

    char *pSql = SELECT_REPORT_DATA;
    char szTemp[200];
    sprintf(szTemp, INSERT_REPORT, type, pSubmitPerson, pUploadDate, pRoomId, pReason, pNotes, pReportData->isView);
    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectReportDataFromDb faied！"), _T("错误"), MB_OK);
        //return FALSE;
    }

    delete pReportData;
    pReportData = NULL;
}

void CWinThreadDatabase::OnDelPersonInof(WPARAM wParam, LPARAM lParam)
{
    PERSON_INFO_PART *pPersonInfo = (PERSON_INFO_PART *)wParam;

    USES_CONVERSION;
    char *pAccount = W2A(pPersonInfo->account);
    char szTemp[200];

    switch (pPersonInfo->authority)
    {
    case MANAGER:
        sprintf(szTemp, DELETE_PERSON_INFO, "t_manager_info","manager_id", pAccount);
        break;
    case REPAIR:
        sprintf(szTemp, DELETE_PERSON_INFO, "t_repair_info", "repair_id", pAccount);
        break;
    case TEACHER:
        sprintf(szTemp, DELETE_PERSON_INFO, "t_teacher_info", "teacher_id", pAccount);
        break;
    default:
        break;
    }

    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));
    if (0 != res)
        ::MessageBox(NULL, _T("OnDelPersonInof faied！"), _T("错误"), MB_OK);

    delete pPersonInfo;
    pPersonInfo = NULL;
}

void CWinThreadDatabase::OnUpdatePersonInof(WPARAM wParam, LPARAM lParam)
{
    PERSON_INFO_PART *pPersonInfo = (PERSON_INFO_PART *)wParam;

    USES_CONVERSION;
    char *pAccount = W2A(pPersonInfo->account);
    char *pPwd = W2A(pPersonInfo->pwd);
    char *pName = W2A(pPersonInfo->name);
    char *pTel = W2A(pPersonInfo->tel);
    char szTemp[200];

    switch (pPersonInfo->oldAuthority)
    {
    case MANAGER:
        sprintf(szTemp, UPDATE_PERSONS_INFO, "t_manager_info", "manager_id", pAccount, pPwd, pName, pTel, pPersonInfo->authority, "manager_id", pAccount);
        break;
    case REPAIR:
        sprintf(szTemp, UPDATE_PERSONS_INFO, "t_repair_info", "repair_id", pAccount, pPwd, pName, pTel, pPersonInfo->authority, "repair_id", pAccount);
        break;
    case TEACHER:
        sprintf(szTemp, UPDATE_PERSONS_INFO, "t_teacher_info", "teacher_id", pAccount, pPwd, pName, pTel, pPersonInfo->authority, "teacher_id", pAccount);
        break;
    default:
        break;
    }

    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));
    if (0 != res)
        ::MessageBox(NULL, _T("OnUpdatePersonInof faied！"), _T("错误"), MB_OK);

    delete pPersonInfo;
    pPersonInfo = NULL;
}

void CWinThreadDatabase::OnDelRegister(WPARAM wParam, LPARAM lParam)
{
    CString* pStr = (CString*)wParam;

    USES_CONVERSION;
    char *pAccount = W2A(*pStr);
    char szTemp[200];

    sprintf(szTemp, DELETE_REGISTER, pAccount);

    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));
    if (0 != res)
        ::MessageBox(NULL, _T("OnDelPersonInof faied！"), _T("错误"), MB_OK);

    delete pStr;
    pStr = NULL;
}

void CWinThreadDatabase::OnDeclareExamInfo(WPARAM wParam, LPARAM lParam)
{
    EXAM_INFO* pExamInfo = (EXAM_INFO*)wParam;

    USES_CONVERSION;
    char *pPerson = W2A(pExamInfo->strPerson);
    char *pText = W2A(pExamInfo->strText);
    char *pClass = W2A(pExamInfo->strClass);
    char *pDate = W2A(pExamInfo->strDate);

    char *pSql = INSERT_EXAM_INFO;
    char szTemp[200];
    sprintf(szTemp, INSERT_EXAM_INFO, pPerson, pText, pClass, pDate);
    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("OnDeclareExamInfo faied！"), _T("错误"), MB_OK);
    }

    delete pExamInfo;
    pExamInfo = NULL;
}

void CWinThreadDatabase::OnSetClassData(WPARAM wParam, LPARAM lParam)
{
    CLASS_DATA* pClassData = (CLASS_DATA*)wParam;
    USES_CONVERSION;
    char *pText = W2A(pClassData->class_text);

    char szTemp[200];
    switch (pClassData->class_num)
    {
    case 1: // 12节
    {
        sprintf(szTemp, UPDATE_CLASS12_DATA, pText,pClassData->week, pClassData->day);
    }
    break;
    case 2: // 34节
    {
        sprintf(szTemp, UPDATE_CLASS34_DATA, pText, pClassData->week, pClassData->day);
    }
    break;
    case 3: // 45节
    {
        sprintf(szTemp, UPDATE_CLASS56_DATA, pText, pClassData->week, pClassData->day);
    }
    break;
    case 4: // 56节
    {
        sprintf(szTemp, UPDATE_CLASS78_DATA, pText, pClassData->week, pClassData->day);
    }
    default:
        break;
    }

    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("OnDeclareExamInfo faied！"), _T("错误"), MB_OK);
    }

    delete pClassData;
    pClassData = NULL;
}

void CWinThreadDatabase::OnInsertWorkload(WPARAM wParam, LPARAM lParam)
{
    WORKLOAD_REPORT *pReportData = (WORKLOAD_REPORT *)wParam;

    USES_CONVERSION;
    char *pReason = W2A(pReportData->strReason);
    char *pName = W2A(pReportData->strName);
    char *pDate = W2A(pReportData->strDate);
    char *pFixSalary = W2A(pReportData->strFixSalary);
    char *pWorkloadSalary = W2A(pReportData->strWorkloadSalary);
    pReportData->isView = 0;

    char szTemp[200];
    sprintf(szTemp, INSERT_WORKLOAD_REPORT, pName, pDate, pFixSalary, pWorkloadSalary, pReason, pReportData->isView);
    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("OnInsertWorkload faied！"), _T("错误"), MB_OK);
        //return FALSE;
    }

    delete pReportData;
    pReportData = NULL;
}

void CWinThreadDatabase::OnInsertAssertData(WPARAM wParam, LPARAM lParam)
{
    ASSERT_DATA* pReportData = (ASSERT_DATA*)wParam;

    USES_CONVERSION;
    char* pDeviceName = W2A(pReportData->strDeviceName);
    char* pOwner = W2A(pReportData->strOwner);
    char* pRoomNum = W2A(pReportData->strRoomNum);
    char* pInDate = W2A(pReportData->strInDate);
    char* pOutDate = W2A(pReportData->strOutDate);
    char* pStatus = W2A(pReportData->strStatus);
    char* pReason = W2A(pReportData->strReason);
    pReportData->isView = 0;

    char szTemp[200];
    sprintf(szTemp, INSERT_ASSERT_DATA, pDeviceName, pOwner, pRoomNum, pInDate, pOutDate, pStatus, pReason);
    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("OnInsertAssertData faied！"), _T("错误"), MB_OK);
        //return FALSE;
    }

    delete pReportData;
    pReportData = NULL;
}

void CWinThreadDatabase::OnUpdateRoomRequest(WPARAM wParam, LPARAM lParam)
{
    REPORT_DATA *pReportData = reinterpret_cast<REPORT_DATA *>(wParam);

    USES_CONVERSION;
    char* pSubmitPerson = W2A(pReportData->submitPerson);
    char* pDate = W2A(pReportData->uploadDate);

    char szTemp[200];
    sprintf(szTemp, UPDATE_ROOM_REQUEST, pReportData->isView, pSubmitPerson, pDate);
    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("OnUpdateRoomRequest faied！"), _T("错误"), MB_OK);
        //return FALSE;
    }

    delete pReportData;
    pReportData = NULL;
}

void CWinThreadDatabase::OnUpdateSalaryInfo(WPARAM wParam, LPARAM lParam)
{
    WORKLOAD_REPORT *pWorkload = reinterpret_cast<WORKLOAD_REPORT *>(wParam);

    USES_CONVERSION;
    char* pName = W2A(pWorkload->strName);
    char* pDate = W2A(pWorkload->strDate);
    char* pload = W2A(pWorkload->strWorkloadSalary);

    char szTemp[200];
    sprintf(szTemp, UPDATE_SALARY_DATE, pload, pWorkload->isView, pName, pDate);
    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("OnUpdateSalaryInfo faied！"), _T("错误"), MB_OK);
        //return FALSE;
    };

    delete pWorkload;
    pWorkload = NULL;
}

void CWinThreadDatabase::OnUpdateExamInfo(WPARAM wParam, LPARAM lParam)
{
    ATTENDENCE_INFO *pStuInfo = reinterpret_cast<ATTENDENCE_INFO *>(wParam);

    USES_CONVERSION;
    char* pStuId = W2A(pStuInfo->student_id);

    char szTemp[200];
    sprintf(szTemp, UPDATE_ATTENDENCE_DATE, pStuInfo->attendece_cnt, pStuInfo->attendece_score, pStuId);
    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("OnUpdateSalaryInfo faied！"), _T("错误"), MB_OK);
        //return FALSE;
    };

    delete pStuInfo;
    pStuInfo = NULL;
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

    mysql_query(&m_mysql, "SET NAMES 'gbk'");//设置数据库字符格式，解决中文乱码问题
    int res = mysql_real_query(&m_mysql, pszSql, (unsigned long)strlen(pszSql));// 查询数据库中的"物料属性"表  
    if (!res)
    {
        result = mysql_store_result(&m_mysql);    //保存查询到的数据到result
        if (result)
        {
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
    // added for test
    //CString *pStr = new CString(_T("test"));
    //PostMessage(ptagLoginRequest->hWnd, WM_LOGIN_SUCCESS, (WPARAM)pStr, 0);
    // end add

    int size = m_vecLoginResult.size();
    for (int i = 0; i < size; i++)
    {
        if (ptagLoginRequest->strAccount == m_vecLoginResult.at(i).strAccount && 
            ptagLoginRequest->strPwd == m_vecLoginResult.at(i).strPwd)
        {
            CString *pStr = new CString(ptagLoginRequest->strAccount);
            PostMessage(ptagLoginRequest->hWnd, WM_LOGIN_SUCCESS, (WPARAM)pStr, 0);
            return;
        }
    }
    PostMessage(ptagLoginRequest->hWnd, WM_LOGIN_ERROR, 0, 0);
}

BOOL CWinThreadDatabase::SelectAdminInfoFromDb(vector<SYSADMIN_INFO> &vecAdminInfo, Json::Value &vec)
{
    vecAdminInfo.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_ADMIN_INFO;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectAdminInfoFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            SYSADMIN_INFO sysAdminInfo;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    //sysAdminInfo.id = (int)*sqlRow[i] - 48;
                    root["id"] = (int)*sqlRow[i] - 48;
                    break;
                case 1:
                    //sysAdminInfo.account = A2W(NotNull(sqlRow[i]));
                    root["account"] = NotNull(sqlRow[i]);
                    break;
                case 2:
                    //sysAdminInfo.pwd = A2W(NotNull(sqlRow[i]));
                    root["pwd"] = NotNull(sqlRow[i]);
                    break;
                default:
                    break;
                }
            }
            vec.append(root);
            //vecAdminInfo.push_back(sysAdminInfo);
        }
        if (mysqlResutl != NULL)
            mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectRoomManagerInfoFromDb(vector<ROOM_MANAGER_INFO> &vecRoomManagerInfo, Json::Value &vec)
{
    vecRoomManagerInfo.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_ROOM_MANAGER_INFO;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectRoomManagerInfoFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            ROOM_MANAGER_INFO roomManagerInfo;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    //roomManagerInfo.id = (int)*sqlRow[i] - 48;
                    root["id"] = (int)*sqlRow[i] - 48;
                    break;
                case 1:
                    //roomManagerInfo.account = A2W(NotNull(sqlRow[i]));
                    root["account"] = NotNull(sqlRow[i]);
                    break;
                case 2:
                    //roomManagerInfo.pwd = A2W(NotNull(sqlRow[i]));
                    root["pwd"] = NotNull(sqlRow[i]);
                    break;
                case 3:
                    //roomManagerInfo.name = A2W(NotNull(sqlRow[i]));
                    root["name"] = NotNull(sqlRow[i]);
                    break;
                case 4:
                    //roomManagerInfo.tel = A2W(NotNull(sqlRow[i]));
                    root["tel"] = NotNull(sqlRow[i]);
                    break;
                case 5:
                    //roomManagerInfo.authority = (int)*sqlRow[i] - 48;
                    root["authority"] = (int)*sqlRow[i] - 48;
                    break;
                default:
                    break;
                }
            }
            vec.append(root);
            vecRoomManagerInfo.push_back(roomManagerInfo);
        }
        
        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectRoomRepairInfoFromDb(vector<ROOM_REPAIR_INFO> &vecRepairInfo, Json::Value &vec)
{
    vecRepairInfo.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_ROOM_REPAIR_INFO;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectRoomRepairInfoFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            ROOM_REPAIR_INFO roomRepairInfo;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    //roomRepairInfo.id = (int)*sqlRow[i] - 48;
                    root["id"] = (int)*sqlRow[i] - 48;
                    break;
                case 1:
                    //roomRepairInfo.account = A2W(sqlRow[i]);
                    root["account"] = NotNull(sqlRow[i]);
                    break;
                case 2:
                    //roomRepairInfo.pwd = A2W(NotNull(sqlRow[i]));
                    root["pwd"] = NotNull(sqlRow[i]);
                    break;
                case 3:
                    //roomRepairInfo.name = A2W(NotNull(sqlRow[i]));
                    root["name"] = NotNull(sqlRow[i]);
                    break;
                case 4:
                    //roomRepairInfo.tel = A2W(NotNull(sqlRow[i]));
                    root["tel"] = NotNull(sqlRow[i]);
                    break;
                case 5:
                    //roomRepairInfo.authority = (int)*sqlRow[i] - 48;
                    root["authority"] = (int)*sqlRow[i] - 48;
                    break;
                default:
                    break;
                }
            }
            vec.append(root);
            //vecRepairInfo.push_back(roomRepairInfo);
        }

        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectTeacherInfoFromDb(vector<TEACHER_INFO> &vecTeacherInfo, Json::Value &vec)
{
    vecTeacherInfo.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_TEACHER_INFO;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectTeacherInfoFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            TEACHER_INFO teacherInfo;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    root["account"] = NotNull(sqlRow[i]);
                    break;
                case 1:
                    root["pwd"] = NotNull(sqlRow[i]);
                    break;
                case 2:
                    root["name"] = NotNull(sqlRow[i]);
                    break;
                case 3:
                    root["tel"] = NotNull(sqlRow[i]);
                    break;
                case 4:
                    root["authority"] = atoi(sqlRow[i]);
                    break;
                case 5:
                    root["classes_id"] = NotNull(sqlRow[i]);
                    break;
                case 6:
                    root["course_id"] = NotNull(sqlRow[i]);
                    break;
                default:
                    break;
                }
            }
            vec.append(root);
        }

        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectStudentInfoFromDb(vector<STUDETN_INFO> &vecStudentInfo, Json::Value &vec)
{
    vecStudentInfo.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_STUDENT_INFO;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectStudentInfoFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            STUDETN_INFO studentInfo;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    root["student_id"] = NotNull(sqlRow[i]);
                    break;
                case 1:
                    root["pwd"] = NotNull(sqlRow[i]);
                    break;
                case 2:
                    root["name"] = NotNull(sqlRow[i]);
                    break;
                case 3:
                    root["sex"] = NotNull(sqlRow[i]);
                    break;
                case 4:
                    root["tel"] = NotNull(sqlRow[i]);
                    break;
                case 5:
                    root["classes"] = NotNull(sqlRow[i]);
                    break;
                case 6:
                    root["major"] = NotNull(sqlRow[i]);
                    break;
                case 7:
                    root["authority"] = atoi(sqlRow[i]);
                    break;
                default:
                    break;
                }
            } 
            vec.append(root);
            vecStudentInfo.push_back(studentInfo);
        }

        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectAttendenceInfoFromDb(vector<ATTENDENCE_INFO>& vecAttendenceInfo, Json::Value & vec)
{
    vecAttendenceInfo.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_ATTENDENCE_INFO;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectAttendenceInfoFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            ATTENDENCE_INFO attendeceInfo;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    root["student_id"] = NotNull(sqlRow[i]);
                    break;
                case 1:
                    root["course_name"] = NotNull(sqlRow[i]);
                    break;
                case 2:
                    root["teacher_name"] = NotNull(sqlRow[i]);
                    break;
                case 3:
                    root["attendece_cnt"] = atoi(sqlRow[i]);
                    break;
                case 4:
                    root["attendece_score"] = atoi(sqlRow[i]);
                    break;
                case 5:
                    root["student_name"] = NotNull(sqlRow[i]);
                    break;
                default:
                    break;
                }
            }
            vec.append(root);
            vecAttendenceInfo.push_back(attendeceInfo);
        }

        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectScheduleFromDb(vector<SCHEDULE_DATA>& vecSheduleInfo, Json::Value &vec)
{
    vecSheduleInfo.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_SCHEDULE_DATA;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectScheduleFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            SCHEDULE_DATA scheduleData;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    root["week"] = atoi(sqlRow[i]);
                    break;
                case 1:
                    root["day"] = atoi(sqlRow[i]);
                    break;
                case 2:
                    root["class_12"] = NotNull(sqlRow[i]);
                    //root["class_num"] = 0;
                    break;
                case 3:
                    root["class_34"] = NotNull(sqlRow[i]);
                    //root["class_num"] = 1;
                    break;
                case 4:
                    root["class_56"] = NotNull(sqlRow[i]);
                    //root["class_num"] = 2;
                    break;
                case 5:
                    root["class_78"] = NotNull(sqlRow[i]);
                    //root["class_num"] = 3;
                    break;
                default:
                    break;
                }
                /*
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    //scheduleData.strMon12 = A2W(sqlRow[i]);
                    root["strMon12"] = sqlRow[i];
                    break;
                case 1:
                    //scheduleData.strMon34 = A2W(sqlRow[i]);
                    root["strMon34"] = sqlRow[i];
                    break;
                case 2:
                    //scheduleData.strMon56 = A2W(sqlRow[i]);
                    root["strMon56"] = sqlRow[i];
                    break;
                case 3:
                    //scheduleData.strMon78 = A2W(sqlRow[i]);
                    root["strMon78"] = sqlRow[i];
                    break;
                case 4:
                    //scheduleData.strTues12 = A2W(sqlRow[i]);
                    root["strTues12"] = sqlRow[i];
                    break;
                case 5:
                    //scheduleData.strTues34 = A2W(sqlRow[i]);
                    root["strTues34"] = sqlRow[i];
                    break;
                case 6:
                    //scheduleData.strTues56 = A2W(sqlRow[i]);
                    root["strTues56"] = sqlRow[i];
                    break;
                case 7:
                    //scheduleData.strTues78 = A2W(sqlRow[i]);
                    root["strTues78"] = sqlRow[i];
                    break;
                case 8:
                    //scheduleData.strWed12 = A2W(sqlRow[i]);
                    root["strWed12"] = sqlRow[i];
                    break;
                case 9:
                    //scheduleData.strWed34 = A2W(sqlRow[i]);
                    root["strWed34"] = sqlRow[i];
                    break;
                case 10:
                    //scheduleData.strWed56 = A2W(sqlRow[i]);
                    root["strWed56"] = sqlRow[i];
                    break;
                case 11:
                    //scheduleData.strWed78 = A2W(sqlRow[i]);
                    root["strWed78"] = sqlRow[i];
                    break;
                case 12:
                    //scheduleData.strThu12 = A2W(sqlRow[i]);
                    root["strThu12"] = sqlRow[i];
                    break;
                case 13:
                    //scheduleData.strThu34 = A2W(sqlRow[i]);
                    root["strThu34"] = sqlRow[i];
                    break;
                case 14:
                    //scheduleData.strThu56 = A2W(sqlRow[i]);
                    root["strThu56"] = sqlRow[i];
                    break;
                case 15:
                    //scheduleData.strThu78 = A2W(sqlRow[i]);
                    root["strThu78"] = sqlRow[i];
                    break;
                case 16:
                    //scheduleData.strFri12 = A2W(sqlRow[i]);
                    root["strFri12"] = sqlRow[i];;
                    break;
                case 17:
                    //scheduleData.strFri34 = A2W(sqlRow[i]);
                    root["strFri34"] = sqlRow[i];
                    break;
                case 18:
                    //scheduleData.strFri56 = A2W(sqlRow[i]);
                    root["strFri56"] = sqlRow[i];
                    break;
                case 19:
                    //scheduleData.strFri78 = A2W(sqlRow[i]);
                    root["strFri78"] = sqlRow[i];
                    break;
                default:
                    break;
                }*/
            }
            vec.append(root);
            //vecSheduleInfo.push_back(scheduleData);
        }

        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectReportDataFromDb(vector<REPORT_DATA>& vecReportData, Json::Value &vec)
{
    vecReportData.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_REPORT_DATA;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectReportDataFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            REPORT_DATA reportData;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    //reportData.id = (int)*sqlRow[i] - 48;
                    root["id"] = (int)*sqlRow[i] - 48;
                    break;
                case 1:
                    //reportData.reportType = (REPORT_TYPE)((int)*sqlRow[i] - 48);
                    root["reportType"] = (int)*sqlRow[i] - 48;
                    break;
                case 2:
                    //reportData.submitPerson = A2W(sqlRow[i]);
                    root["submitPerson"] = NotNull(sqlRow[i]);
                    break;
                case 3:
                    //reportData.uploadDate = A2W(NotNull(sqlRow[i]));
                    root["uploadDate"] = NotNull(sqlRow[i]);
                    break;
                case 4:
                    //reportData.roomId = A2W(NotNull(sqlRow[i]));
                    root["roomId"] = NotNull(sqlRow[i]);
                    break;
                case 5:
                    //reportData.reason = A2W(NotNull(sqlRow[i]));
                    root["reason"] = NotNull(sqlRow[i]);
                    break;
                case 6:
                    //reportData.notes = A2W(NotNull(sqlRow[i]));
                    root["notes"] = NotNull(sqlRow[i]);
                    break;
                case 7:
                    //reportData.isView = ((int)*sqlRow[i] - 48);
                    root["isView"] = ((int)*sqlRow[i] - 48);
                    break;
                default:
                    break;
                }
            }
            vec.append(root);
            //vecReportData.push_back(reportData);
        }

        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::InsertNewAnnounceToDb(CString strText, int type)
{
    USES_CONVERSION;
    char *pStr = W2A(strText);

    char *pSql = SELECT_REPORT_DATA;
    char szTemp[200];
    sprintf(szTemp, INSERT_NEW_ANNOUNCE, pStr, type);
    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectReportDataFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}


BOOL CWinThreadDatabase::SelectAnnounceFromDb(vector<ANNOUNCE_DATA> &vecAnnounceData, Json::Value &vec)
{
    vecAnnounceData.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_ANNOUNCE_DATA;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectAnnounceFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            ANNOUNCE_DATA announceData;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    //announceData.announce_id = (int)*sqlRow[i] - 48;
                    root["announce_id"] = (int)*sqlRow[i] - 48;
                    break;
                case 1:
                    //announceData.strText = sqlRow[i];
                    root["strText"] = NotNull(sqlRow[i]);
                    break;
                case 2:
                    //announceData.type = (int)*sqlRow[i] - 48;
                    root["type"] = (int)*sqlRow[i] - 48;
                    break;
                default:
                    break;
                }
            }
            vec.append(root);
            //vecAnnounceData.push_back(announceData);
        }

        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectRegisterInfoFromDb(vector<ADD_PERSON_DATA>& vecRegisterData, Json::Value &vec)
{
    vecRegisterData.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_REGISTER_DATA;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectRegisterInfoFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            ADD_PERSON_DATA registerData;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    //registerData.account = A2W(NotNull(sqlRow[i]));
                    root["account"] = NotNull(sqlRow[i]);
                    break;
                case 1:
                    //registerData.pwd = A2W(NotNull(sqlRow[i]));
                    root["pwd"] = NotNull(sqlRow[i]);
                    break;
                case 2:
                    //registerData.name = A2W(NotNull(sqlRow[i]));
                    root["name"] = NotNull(sqlRow[i]);
                    break;
                case 3:
                    //registerData.tel = A2W(NotNull(sqlRow[i]));
                    root["tel"] = NotNull(sqlRow[i]);
                    break;
                case 4:
                    //registerData.authority = (int)*sqlRow[i] - 48;
                    root["authority"] = (int)*sqlRow[i] - 48;
                    break;
                default:
                    break;
                }
            }
            vec.append(root);
            //vecRegisterData.push_back(registerData);
        }

        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectExamInfoFromDb(vector<EXAM_INFO>& vecExamInfo, Json::Value &vec)
{
    vecExamInfo.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_EXAM_INFO;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));//插入数据
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectExamInfoFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            EXAM_INFO examInfo;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    //registerData.account = A2W(NotNull(sqlRow[i]));
                    root["person"] = NotNull(sqlRow[i]);
                    break;
                case 1:
                    //registerData.pwd = A2W(NotNull(sqlRow[i]));
                    root["text"] = NotNull(sqlRow[i]);
                    break;
                case 2:
                    //registerData.name = A2W(NotNull(sqlRow[i]));
                    root["classes"] = NotNull(sqlRow[i]);
                    break;
                case 3:
                    //registerData.tel = A2W(NotNull(sqlRow[i]));
                    root["date"] = NotNull(sqlRow[i]);
                    break;
                default:
                    break;
                }
            }
            vec.append(root);
            //vecRegisterData.push_back(registerData);
        }

        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectAssertHandoverFromDb(vector<ASSERT_DATA>& vecAssertDataInfo, Json::Value &vec)
{
    vecAssertDataInfo.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_ASSERT_INFO;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectAssertHandoverFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            WORKLOAD_REPORT workloadInfo;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    //registerData.account = A2W(NotNull(sqlRow[i]));
                    root["device_name"] = NotNull(sqlRow[i]);
                    break;
                case 1:
                    //registerData.pwd = A2W(NotNull(sqlRow[i]));
                    root["owner"] = NotNull(sqlRow[i]);
                    break;
                case 2:
                    //registerData.name = A2W(NotNull(sqlRow[i]));
                    root["room_num"] = NotNull(sqlRow[i]);
                    break;
                case 3:
                    //registerData.tel = A2W(NotNull(sqlRow[i]));
                    root["in_date"] = NotNull(sqlRow[i]);
                    break;
                case 4:
                    //registerData.tel = A2W(NotNull(sqlRow[i]));
                    root["out_date"] = NotNull(sqlRow[i]);
                    break;
                case 5:
                    //registerData.tel = A2W(NotNull(sqlRow[i]));
                    root["status"] = NotNull(sqlRow[i]);
                    break;
                case 6:
                    //registerData.tel = A2W(NotNull(sqlRow[i]));
                    root["reason"] = NotNull(sqlRow[i]);
                    break;

                default:
                    break;
                }
            }
            vec.append(root);
        }

        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::SelectWorkloadSalaryInfoFromDb(vector<WORKLOAD_REPORT>& vecSalaryInfo, Json::Value &vec)
{
    vecSalaryInfo.clear();
    vec.clear();

    MYSQL_RES *mysqlResutl;
    MYSQL_ROW sqlRow;

    char *pSql = SELECT_SALARY_INFO;
    int res = mysql_real_query(&m_mysql, pSql, (unsigned long)strlen(pSql));
    if (0 != res)
    {
        ::MessageBox(NULL, _T("SelectWorkloadSalaryInfoFromDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    Json::Value root;
    mysqlResutl = mysql_store_result(&m_mysql); // 保存查询的所有数据
    if (NULL != mysqlResutl)
    {
        int j = mysql_num_fields(mysqlResutl);	 // 获取列数
        while (sqlRow = mysql_fetch_row(mysqlResutl))    //获取具体的数据
        {
            EXAM_INFO examInfo;
            for (int i = 0; i < j; i++)
            {
                USES_CONVERSION;
                switch (i)
                {
                case 0:
                    //registerData.account = A2W(NotNull(sqlRow[i]));
                    root["name"] = NotNull(sqlRow[i]);
                    break;
                case 1:
                    //registerData.pwd = A2W(NotNull(sqlRow[i]));
                    root["date"] = NotNull(sqlRow[i]);
                    break;
                case 2:
                    //registerData.name = A2W(NotNull(sqlRow[i]));
                    root["fix_salary"] = NotNull(sqlRow[i]);
                    break;
                case 3:
                    //registerData.tel = A2W(NotNull(sqlRow[i]));
                    root["workload_salary"] = NotNull(sqlRow[i]);
                    break;
                case 4:
                    //registerData.tel = A2W(NotNull(sqlRow[i]));
                    root["reason"] = NotNull(sqlRow[i]);
                    break;
                default:
                    break;
                }
            }
            vec.append(root);
            //vecRegisterData.push_back(registerData);
        }

        mysql_free_result(mysqlResutl);
    }
    else
    {
        ::MessageBox(NULL, _T("mysql_store_result faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::UpdateAdminPwdToDb(CString strPwd)
{
    USES_CONVERSION;
    char *pStr = W2A(strPwd);

    char szTemp[200];
    sprintf(szTemp, UPDATE_ADMIN_PWD, pStr);
    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));
    if (0 != res)
    {
        ::MessageBox(NULL, _T("UpdateAdminPwdToDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::UpdatePersonPwdToDb(UPDATE_PERSON_PWD * pPersonPwd)
{
    USES_CONVERSION;
    char *pAccount = W2A(pPersonPwd->account);
    char *pPwd = W2A(pPersonPwd->newPwd);
    char szTemp[200];

    switch (pPersonPwd->authrity)
    {
    case MANAGER:
        sprintf(szTemp, UPDATE_PERSONS_PWD, "t_manager_info", pPwd, "manager_id", pAccount);
        break;
    case REPAIR:
        sprintf(szTemp, UPDATE_PERSONS_PWD, "t_repair_info", pPwd, "repair_id", pAccount);
        break;
    case TEACHER:
        sprintf(szTemp, UPDATE_PERSONS_PWD, "t_teacher_info", pPwd, "teacher_id", pAccount);
        break;
    case STUDENT:
        sprintf(szTemp, UPDATE_PERSONS_PWD, "t_student_info", pPwd, "student_id", pAccount);
        break;
    default:
        break;
    }

    int res = mysql_real_query(&m_mysql, szTemp, (unsigned long)strlen(szTemp));
    if (0 != res)
    {
        ::MessageBox(NULL, _T("UpdateAdminPwdToDb faied！"), _T("错误"), MB_OK);
        return FALSE;
    }

    return TRUE;
}

BOOL CWinThreadDatabase::InitThread()
{
    m_strIP = _T("127.0.0.1");
    m_strPort = _T("6666");

    if (CreateThread())
    {
        mysql_init(&m_mysql);//初始化
        bool isConnected = mysql_real_connect(&m_mysql, "localhost", "root", "12345", "crms", 3306, 0, 0);
        if (!isConnected)
        {
            int i = mysql_errno(&m_mysql);//连接出错
            const char * s = mysql_error(&m_mysql);
            ::MessageBox(NULL, _T("failed to connect! Please check for your db service."), _T("错误提示"), MB_OK);
            return FALSE;
        }
    }

    return TRUE;
}


void CWinThreadDatabase::OnRecv(WPARAM wParam, LPARAM lParam)
{
    CString *pStr = (CString *)wParam;

    USES_CONVERSION;
    char *pBuf = W2A(*pStr);
    PraseJson(pBuf);

    delete[] pStr; // 内存访问越界
    pStr = NULL;
}


char* CWinThreadDatabase::NotNull(char * str)
{
    if (NULL != str)
    {
        USES_CONVERSION;
        return str;
    }

    return " ";
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
        default:
            break;
        }
    }
}

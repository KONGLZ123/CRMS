#pragma once
#include <vector>
#include <map>
#include <set>
using namespace std;

#define S_CODE_LOGIN                2000
#define S_CODE_LOGIN_SUCCESS        2001
#define S_CODE_LOGIN_FAILED         2002
#define S_CODE_INIT_DATA            2003
#define S_CODE_INIT_DATA_SUCCESS    2004
#define S_CODE_INIT_DATA_FAILED     2005
#define S_CODE_INSERT_NEW_ACCOUNT   2006
#define S_CODE_INSERT_REPORT        2007
#define S_CODE_INSERT_ANNOUNCE      2008
#define S_CODE_UPDATE_ADMIN_PWD     2009
#define S_CODE_UPDATE_PERSON_PWD    2010
#define S_CODE_UPDATE_PERSON_INFO   2011
#define S_CODE_DEL_PERSON_INFO      2012
#define S_CODE_DEL_REGISTER         2013
#define S_CODE_INSERT_EXAM_INFO     2014
#define S_CODE_INSERT_CLASS_INFO    2015
#define S_CODE_INSERT_WORKLOAD      2016   
#define S_CODE_INSERT_ASSERT_DATA   2017   
#define S_CODE_UPDATE_ROOM_REQUEST  2018
#define S_CODE_UPDATE_SALARY_INFO   2019
#define S_CODE_UPDATE_EXAM_INFO     2020
#define S_CODE_DELETE_OLD_ANNOUNCE  2021
#define S_CODE_PERSON_EXIT_ERROR    2022

enum {
    ADMIN = 0,
    MANAGER,
    REPAIR,
    TEACHER,
    STUDENT,
};

enum REPORT_TYPE 
{
    RUNING_REPORT = 0,
    USE_REPORT,
    REPAIR_REPORT,
    REQUEST_REPORT,
    REQUEST_REPAIR,
    REQUEST_WORKLOAD,
    REQUEST_ROOM,
    HANDOVER_REPORT,
    
};

enum LIST_STYLE
{
    LIST_STYLE_NORMAL = 0,
    LIST_STYLE_PERSON_INFO,
    LIST_STYLE_SCHEDULE,
    LIST_STYLE_STUDENT_MANAGER,
    LIST_STYLE_SALARY,
    LIST_STYLE_ROOM_REQUEST,
};

struct EXAM_INFO
{
    CString strText;
    CString strPerson;
    CString strDate;
    CString strClass; // 班级
};

struct SERVER_SETUP
{
    CString strIP;
    CString strPort;
};

struct SOCK_DATA
{
    SOCKET socket;
    HWND hWnd;
};

struct LOGIN_REQUEST
{
    CString strAccount;
    CString strPwd;
    HWND hWnd;
    int role;
};

struct LOGIN_RESULT
{
    CString strAccount;
    CString strPwd;
};

struct SYSADMIN_INFO
{
    int id;
    CString account;
    CString pwd;
};

struct PERSON_INFO_PART
{
    int id;
    CString account;
    CString pwd;
    CString name;
    CString tel;
    int authority;
    int oldAuthority;
};

struct ROOM_MANAGER_INFO 
{
    int id;
    CString account;
    CString pwd;
    CString name;
    CString tel;
    int authority;
};

struct ROOM_REPAIR_INFO
{
    int id;
    CString account;
    CString pwd;
    CString name;
    CString tel;
    int authority;
};

struct TEACHER_INFO
{
    //int id;
    CString account;
    CString pwd;
    CString name;
    CString tel;
    int authority;
    CString classes;
    CString course;
};

struct STUDETN_INFO
{
    CString student_id;
    CString pwd;
    CString name;
    CString sex;
    CString tel;
    CString classes;
    CString major;
    //CString college;
    //CString exam;
    int authority;
    //int attendece_cnt;      // 出勤次数
    //int attendece_score;    // 考勤分数
};

struct ATTENDENCE_INFO
{
    CString student_id;
    CString student_name;
    CString course_name;
    CString teacher_name;
    int attendece_cnt;      // 出勤次数
    int attendece_score;    // 考勤分数
};

struct WORKLOAD_REPORT
{
    CString strReason;
    CString strName;
    CString strDate;
    CString strFixSalary;
    CString strWorkloadSalary;
    int result;
    //CString reason;
    //CString notes;
    int isView;
};

struct ASSERT_DATA
{
    CString strDeviceName;
    CString strOwner;
    CString strRoomNum;
    CString strInDate;
    CString strOutDate;
    CString strReason;
    CString strStatus;
    int isView;
};

struct REPORT_DATA
{
    int id;
    REPORT_TYPE reportType;
    CString submitPerson;
    CString uploadDate;
    CString roomId;
    CString reason;
    CString notes;
    int isView;
};

struct ANNOUNCE_DATA
{
    int announce_id;
    CString strText;
    int type;
};

struct WEEK_SCHEDULE
{
    int day;
    CString class_12;
    CString class_34;
    CString class_56;
    CString class_78;
};

struct CLASS_DATA
{
    CString schedule_id;
    //std::map<int, WEEK_SCHEDULE> week_scheduel;
    int week;
    int day;
    int class_num;
    CString class_text;
    CString class_12;
    CString class_34;
    CString class_56;
    CString class_78;
};

struct SCHEDULE_DATA
{
    CString strMon12;
    CString strMon34;
    CString strMon56;
    CString strMon78;
    CString strTues12;
    CString strTues34;
    CString strTues56;
    CString strTues78;
    CString strWed12;
    CString strWed34;
    CString strWed56;
    CString strWed78;
    CString strThu12;
    CString strThu34;
    CString strThu56;
    CString strThu78;
    CString strFri12;
    CString strFri34;
    CString strFri56;
    CString strFri78;
};

struct ADD_PERSON_DATA
{
    CString account;
    CString pwd;
    CString name;
    CString tel;
    int authority;
    CString id;
    CString grade;
    CString major;
    CString sex;
    CString course;
    BOOL bIsRegister;
};

struct UPDATE_PERSON_PWD
{
    CString account;
    CString newPwd;
    int     authrity;
};

struct DATA_STOCK
{
    vector<SYSADMIN_INFO>           vecAdminInfo;
    vector<ROOM_MANAGER_INFO>       vecRoomManagerInfo;
    vector<ROOM_REPAIR_INFO>        vecRoomRepairInfo;
    vector<TEACHER_INFO>            vecTeacherInfo;
    vector<STUDETN_INFO>            vecStudentInfo;

    vector<REPORT_DATA>             vecReportData;
    vector<ANNOUNCE_DATA>           vecAnnounceData;

    vector<CLASS_DATA>           vecScheduleData;
    vector<ADD_PERSON_DATA>         vecRegisterData;

    vector<EXAM_INFO>               vecExamInfo;

    vector<WORKLOAD_REPORT>         vecSalaryInfo;
    vector<ASSERT_DATA>             vecAssertDataInfo;
    vector<ATTENDENCE_INFO>         vecAttendenceInfo;

    set<CString>                    setClass;   // 班级集合
};


///////////////////////////////////////////////////////////////////
// SQL Interface
#define SELECT_ADMIN_LOING          "SELECT account, pwd FROM t_sysadmin_info"
#define SELECT_ROOM_NANAGER_LOGIN   "SELECT manager_id, pwd FROM t_manager_info"
#define SELECT_ROOM_REPAIR_LOGIN    "SELECT repair_id, pwd FROM t_repair_info"
#define SELECT_TEACHER_LOING        "SELECT teacher_id, pwd FROM t_teacher_info"
#define SELECT_STUDENT_LOGIN        "SELECT student_id, pwd FROM t_student_info"
#define SELECT_SCHEDULE_DATA        "SELECT mon_12, mon_34, mon_56, mon_78, \
                                     tues_12, tues_34, tues_56, tues_78,\
                                     wed_12, wed_34, wed_56, wed_78, \
                                     thu_12, thu_34, thu_56, thu_78, \
                                     fri_12, fri_34, fri_56, fri_78 FROM t_schedule"

#define SELECT_ADMIN_INFO           "SELECT id, account, pwd FROM t_sysadmin_info"
#define SELECT_ROOM_MANAGER_INFO    "SELECT id, manager_id, pwd, user_name, tel, authority FROM t_manager_info"
#define SELECT_ROOM_REPAIR_INFO     "SELECT id, repair_id, pwd, user_name, tel, authority FROM t_repair_info"
#define SELECT_TEACHER_INFO         "SELECT id, teacher_id, pwd, user_name, tel, authority FROM t_teacher_info"
#define SELECT_STUDENT_INFO         "SELECT student_id, pwd, user_name, tel, classes, major, college, exam, authority FROM t_student_info"
#define SELECT_REPORT_DATA          "SELECT id, report_type, submit_person, upload_date, room_id, reason, notes, is_view FROM t_report"
#define SELECT_ANNOUNCE_DATA        "SELECT announce_id, announce_text, authority FROM t_announce"
#define SELECT_REGISTER_DATA        "SELECT account, pwd, user_name, tel, authority FROM t_register"


#define INSERT_NEW_ANNOUNCE         "INSERT t_announce(announce_text, authority) VALUES('%s', %d)"
#define INSERT_NEW_ACCOUNT          "INSERT t_register(account, pwd, user_name, tel, authority) VALUES('%s', '%s', '%s', '%s', %d)"
#define INSERT_REPORT               "INSERT t_report (report_type, submit_person, upload_date, room_id, reason, notes, is_view) VALUES(%d, '%s', '%s', '%s', '%s', '%s', %d)"

#define DELETE_OLD_ANNOUNCE         "DELETE FROM t_announce WHERE announce_id = %d"

#define UPDATE_ADMIN_PWD            "UPDATE t_sysadmin_info SET pwd = '%s' WHERE account = 'admin'"
#define UPDATE_PERSONS_PWD          "UPDATE %s SET pwd = '%s' WHERE %s = '%s'"
///////////////////////////////////////////////////////////////////
// Message

#define WM_RECV                 WM_USER + 1111

#define WM_LOGIN                WM_USER + 2000
#define WM_LOGIN_SUCCESS        WM_USER + 2001
#define WM_LOGIN_ERROR          WM_USER + 2002

#define WM_GET_PERSON_INFO      WM_USER + 2101

#define WM_INIT_DATA            WM_USER + 2201
#define WM_INITDATA_SUCCESS     WM_USER + 2202
#define WM_INITDATA_FAILED      WM_USER + 2203

#define WM_DECLARE_ANNOUNCE     WM_USER + 2301
#define WM_DEL_ANNOUNCE         WM_USER + 2302

#define WM_ADMIN_SETUP_PWD      WM_USER + 2401

#define WM_ADD_PERSON           WM_USER + 2501
#define WM_ADD_PERSON_SUCCESS   WM_USER + 2502
#define WM_ADD_PERSON_ERROR     WM_USER + 2503

#define WM_SETUP_PWD            WM_USER + 2504
#define WM_SETUP_SERVER         WM_USER + 2505

#define WM_INSERT_REPORT            WM_USER + 2510
#define WM_INSERT_WORKLOAD_REPORT   WM_USER + 2511
#define WM_INSERT_ASSERT_DATA       WM_USER + 2512

#define WM_UPDATE_PERSON_INFO   WM_USER + 2520
#define WM_DEL_PERSON_INFO      WM_USER + 2521

#define WM_UPDATE_LIST_ITEM     WM_USER + 2522
#define WM_DEL_LIST_ITEM        WM_USER + 2523
#define WM_ADD_LIST_ITEM        WM_USER + 2524

#define WM_DEL_REGISTER         WM_USER + 2525
#define WM_DECALRE_EXAM_INFO    WM_USER + 2526
#define WM_UPDATE_CLASS_DATA    WM_USER + 2527
#define WM_UPDATE_ROOM_REQUEST  WM_USER + 2528
#define WM_UPDATE_ROOM_LIST     WM_USER + 2529
#define WM_UPDATE_STUDENT_LIST_ITEM       WM_USER + 2530
#define WM_UPDATE_EXAM_INFO               WM_USER + 2531
#define WM_UPDATE_SALARY_LIST_ITEM        WM_USER + 2532
#define WM_UPDATE_SALARY_INFO             WM_USER + 2533
#define WM_UPDATE_SCHDUEL                 WM_USER + 2534
#define WM_PERSON_EXIST_ERROR             WM_USER + 2535

// PhilosophersDinner.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "PhilosophersDinner.h"
#include <stdio.h>

#pragma warning(disable:4996)

#define MAX_LOADSTRING 100
#define PHILOSOPER_COUNT 5
#define WM_INVALIDATE    WM_USER + 1

typedef struct _tagCOMMUNICATIONOBJECT
{
    HWND hWnd;
    bool bExitApplication;
    int iPhilosopherArray[PHILOSOPER_COUNT];
    int philosopherCount;
}COMMUNICATIONOBJECT, *PCOMMUNICATIONOBJECT;

// 全局变量: 
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

TCHAR *szMappingName = _T("__SHARED_FILE_MAPPING__");
PCOMMUNICATIONOBJECT pCommObject = NULL;
TCHAR *szSemaphoreName = _T("__PD_SEMAPHORE__");

// 此代码模块中包含的函数的前向声明: 
ATOM                MyRegisterClass(HINSTANCE hInstance);
HWND                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。
    HANDLE hMapping = ::CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0, sizeof(COMMUNICATIONOBJECT), szMappingName);
    if (!hMapping) {
        return 1;
    }

    pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
    if (!pCommObject) {
        return 1;
    }

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PHILOSOPHERSDINNER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化: 
    HWND hWnd = InitInstance(hInstance, nCmdShow);
    if (!hWnd)
    {
        return FALSE;
    }

    pCommObject->bExitApplication = false;
    pCommObject->hWnd = hWnd;
    memset(pCommObject->iPhilosopherArray, 0, sizeof(pCommObject->iPhilosopherArray));
    pCommObject->philosopherCount = PHILOSOPER_COUNT;

    HANDLE hSemaphore = ::CreateSemaphore(NULL, int(PHILOSOPER_COUNT / 2), int(PHILOSOPER_COUNT / 2), szSemaphoreName);
    
    STARTUPINFOA startupInfo[PHILOSOPER_COUNT] = { {0}, {0}, {0}, {0}, {0} };
    PROCESS_INFORMATION processInfomation[PHILOSOPER_COUNT] = { { 0 },{ 0 },{ 0 },{ 0 },{ 0 } };
    HANDLE hProcesses[PHILOSOPER_COUNT];
    char szBuffer[8];
    for (int i = 0; i < PHILOSOPER_COUNT; ++i) {
        sprintf(szBuffer, "%d", i);
        if (::CreateProcessA("..\\Debug\\Philosopher.exe", szBuffer, NULL, NULL, FALSE, 0, NULL, NULL,
            &startupInfo[i], &processInfomation[i])) {
            hProcesses[i] = processInfomation[i].hProcess;
        }
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PHILOSOPHERSDINNER));

    MSG msg;

    // 主消息循环: 
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    pCommObject->bExitApplication = TRUE;
    UnmapViewOfFile(hMapping);
    WaitForMultipleObjects(PHILOSOPER_COUNT, hProcesses, TRUE, INFINITE);
    for (int i = 0; i < PHILOSOPER_COUNT; ++i) {
        CloseHandle(hProcesses[i]);
    }
    CloseHandle(hSemaphore);
    CloseHandle(hMapping);

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PHILOSOPHERSDINNER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PHILOSOPHERSDINNER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
HWND InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      200, 200, 540, 590, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return NULL;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return hWnd;
}

int Philosopher(int philosopher)
{
    return pCommObject->iPhilosopherArray[philosopher - 1];
}

void FillEllipse(HWND hWnd, HDC hDC, int iLeft, int iTop, int iRight, int iBottom, int iPass)
{
    HBRUSH hBrush = NULL;
    if (iPass) {
        hBrush = CreateSolidBrush(RGB(255, 0, 0));
    }
    else {
        hBrush = CreateSolidBrush(RGB(255, 255, 255));
    }
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, hBrush);
    Ellipse(hDC, iLeft, iTop, iRight, iBottom);
    SelectObject(hDC, hOldBrush);
    DeleteObject(hBrush);
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择: 
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_INVALIDATE: {
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            FillEllipse(hWnd, hdc, 210, 10, 310, 110, Philosopher(1));
            FillEllipse(hWnd, hdc, 410, 170, 510, 270, Philosopher(2));
            FillEllipse(hWnd, hdc, 335, 400, 435, 500, Philosopher(3));
            FillEllipse(hWnd, hdc, 80, 400, 180, 500, Philosopher(4));
            FillEllipse(hWnd, hdc, 10, 170, 110, 270, Philosopher(5));

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

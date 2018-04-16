// IPCDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>

using namespace std;

#define SYNCHRONIZING_MUTEX_NAME    _T("__TEST_MUTEX__")
#define COMUNICATION_OBJECT_NAME    _T("__FILE_MAPPING__")
#define WINDOW_CLASS_NAME           _T("__TMPWNDCLASS__")
#define BUTTON_CLOSE                100

typedef struct _tagCOMMUNICATIONOBJECT
{
    HWND hWndClient;
    BOOL bExitLoop;
    LONG lSleepTimeout;
}COMMUNICATIONOBJECT, *PCOMMUNICATIONOBJECT;

HANDLE hMapping = NULL;
PCOMMUNICATIONOBJECT pCommObject = NULL;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_COMMAND: {
        switch (LOWORD(wParam))
        {
        case BUTTON_CLOSE: {
            PostMessage(hWnd, WM_CLOSE, 0, 0);
            break;
        }
        }
        break;
    }
    case WM_DESTROY: {
        pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0);
        if (pCommObject) {
            pCommObject->bExitLoop = TRUE;
            UnmapViewOfFile(pCommObject);
        }
        PostQuitMessage(0);
        break;
    }
    default:
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return 0;
}

HWND InitializeWnd()
{
    WNDCLASSEX wndEx;
    wndEx.cbSize = sizeof(WNDCLASSEX);
    wndEx.style = CS_HREDRAW | CS_VREDRAW;
    wndEx.lpfnWndProc = WndProc;
    wndEx.cbClsExtra = 0;
    wndEx.cbWndExtra = 0;
    wndEx.hInstance = GetModuleHandle(NULL);
    wndEx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wndEx.lpszMenuName = NULL;
    wndEx.lpszClassName = WINDOW_CLASS_NAME;
    wndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndEx.hIcon = LoadIcon(wndEx.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wndEx.hIconSm = LoadIcon(wndEx.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    if (!RegisterClassEx(&wndEx)) {
        return NULL;
    }

    HWND hWnd = ::CreateWindow(wndEx.lpszClassName, _T("Interprocess communication Demo"), WS_OVERLAPPEDWINDOW,
        200, 200, 400, 300, NULL, NULL, wndEx.hInstance, NULL);
    if (!hWnd) {
        return NULL;
    }
    HWND hButton = ::CreateWindow(_T("BUTTON"), _T("Close"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,
        275, 225, 100, 25, hWnd, (HMENU)BUTTON_CLOSE, wndEx.hInstance, NULL);
    HWND hStatic = ::CreateWindow(_T("STATIC"), _T(""), WS_CHILD | WS_VISIBLE,
        10, 10, 365, 205, hWnd, NULL, wndEx.hInstance, NULL);
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    return hStatic;
}

int main()
{
    cout << "Interprocess communication demo." << endl;
    HWND hWnd = InitializeWnd();
    if (!hWnd) {
        return 1;
    }

    HANDLE hMutex = CreateMutex(NULL, FALSE, SYNCHRONIZING_MUTEX_NAME);
    if (!hMutex) {
        cout << "hMutex ： " << GetLastError() << endl;
        return 1;
    }

    hMapping = CreateFileMapping((HANDLE)-1, NULL, PAGE_READWRITE, 0, sizeof(COMMUNICATIONOBJECT), COMUNICATION_OBJECT_NAME);
    if (!hMapping) {
        cout << "hMapping ： " << GetLastError() << endl;
        return 1;
    }

    pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hMapping, FILE_MAP_WRITE, 0, 0, 0);
    if (pCommObject) {
        pCommObject->bExitLoop = FALSE;
        pCommObject->hWndClient = hWnd;
        pCommObject->lSleepTimeout = 250;
        UnmapViewOfFile(pCommObject);
    }

    STARTUPINFO startupInfoRed = { 0 };
    PROCESS_INFORMATION processInfomationRed = { 0 };
    STARTUPINFO startupInfoBlue = { 0 };
    PROCESS_INFORMATION processInfomationBlue = { 0 };

    BOOL bSuccess = FALSE;
    bSuccess = CreateProcess(_T("..\\Debug\\IPCWorker.exe"), _T("red"), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfoRed, &processInfomationRed);
    if (!bSuccess) {
        return 1;
    }

    bSuccess = CreateProcess(_T("..\\Debug\\IPCWorker.exe"), _T("blue"), NULL, NULL, FALSE, 0, NULL, NULL, &startupInfoBlue, &processInfomationBlue);
    if (!bSuccess) {
        return 1;
    }

    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnregisterClass(WINDOW_CLASS_NAME, GetModuleHandle(NULL));
    CloseHandle(hMapping);
    CloseHandle(hMutex);

    return 0;
}


// IPCWorker.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>

#define SYNCHRONIZING_MUTEX_NAME    _T("__TEST_MUTEX__")
#define COMUNICATION_OBJECT_NAME    _T("__FILE_MAPPING__")

typedef struct _tagCOMMUNICATIONOBJECT
{
    HWND hWndClient;
    BOOL bExitLoop;
    LONG lSleepTimeout;
}COMMUNICATIONOBJECT, *PCOMMUNICATIONOBJECT;

int main(int argc, _TCHAR *argv[])
{
    HBRUSH hBrush = NULL;

    if (_tcscmp(_T("blue"), argv[0]) == 0) {
        hBrush = ::CreateSolidBrush(RGB(0, 0, 255));
    }
    else {
        hBrush = ::CreateSolidBrush(RGB(255, 0, 0));
    }

    BOOL bContinueLoop = TRUE;
    PCOMMUNICATIONOBJECT pCommObject = NULL;
    HWND hWnd = NULL;
    LONG lWaitTimeout = 0;
    HDC hDC = NULL;
    RECT rectClient = { 0 };

    HANDLE hMutex = ::OpenMutex(MUTEX_ALL_ACCESS, FALSE, SYNCHRONIZING_MUTEX_NAME);
    HANDLE hMapping = ::OpenFileMapping(FILE_MAP_READ, FALSE, COMUNICATION_OBJECT_NAME);

    if (hMapping) {
        while (bContinueLoop) {
            WaitForSingleObject(hMutex, INFINITE);
            pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hMapping, FILE_MAP_READ, 0, 0, sizeof(COMMUNICATIONOBJECT));
            if (pCommObject) {
                bContinueLoop = !pCommObject->bExitLoop;
                hWnd = pCommObject->hWndClient;
                lWaitTimeout = pCommObject->lSleepTimeout;
                UnmapViewOfFile(pCommObject);

                hDC = ::GetDC(hWnd);
                if (GetClientRect(hWnd, &rectClient)) {
                    FillRect(hDC, &rectClient, hBrush);
                }
                ReleaseDC(hWnd, hDC);
                Sleep(lWaitTimeout);
            }
            ReleaseMutex(hMutex);
        }
    }
    CloseHandle(hMapping);
    CloseHandle(hMutex);
    DeleteObject(hBrush);

    return 0;
}


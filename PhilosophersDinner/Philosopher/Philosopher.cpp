// Philosopher.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>

#define PHILOSOPER_COUNT 5
#define WM_INVALIDATE    WM_USER + 1

typedef struct _tagCOMMUNICATIONOBJECT
{
    HWND hWnd;
    bool bExitApplication;
    int iPhilosopherArray[PHILOSOPER_COUNT];
    int philosopherCount;
}COMMUNICATIONOBJECT, *PCOMMUNICATIONOBJECT;

TCHAR *szMappingName = _T("__SHARED_FILE_MAPPING__");
TCHAR *szSemaphoreName = _T("__PD_SEMAPHORE__");
bool bExitApplication = false;

void Eat()
{
    Sleep(1000);
}

int _tmain(int argc, _TCHAR* argv[])
{
    HWND hConsole = GetConsoleWindow();
    ShowWindow(hConsole, SW_HIDE);

    //MessageBox(NULL, argv[0], "", 0);
    int iIndex = (int)_tcstol(argv[0], NULL, 10);
    HANDLE hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, szMappingName);
    while (!bExitApplication) {
        HANDLE hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, szSemaphoreName);
        if (hSemaphore) {
            WaitForSingleObject(hSemaphore, INFINITE);
            PCOMMUNICATIONOBJECT pCommObject = (PCOMMUNICATIONOBJECT)MapViewOfFile(hSemaphore, FILE_MAP_ALL_ACCESS,
                0, 0, sizeof(COMMUNICATIONOBJECT));
            bExitApplication = pCommObject->bExitApplication;
            int left = (iIndex + pCommObject->philosopherCount - 1) % pCommObject->philosopherCount;
            int right = (iIndex + 1) % pCommObject->philosopherCount;
            if (!pCommObject->iPhilosopherArray[left] &&
                !pCommObject->iPhilosopherArray[right]) {
                pCommObject->iPhilosopherArray[iIndex] = 1;
                Eat();
            }
            SendMessage(pCommObject->hWnd, WM_INVALIDATE, 0, 0);
            pCommObject->iPhilosopherArray[iIndex] = 0;
            UnmapViewOfFile(pCommObject);
            ReleaseSemaphore(hSemaphore, 1, NULL);
            CloseHandle(hSemaphore);
        }
    }
    CloseHandle(hMapping);

    return 0;
}


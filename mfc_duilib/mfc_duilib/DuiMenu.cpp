#include "stdafx.h"
#include "DuiMenu.h"



LRESULT CDuiMenu::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL & bHandled)
{
    Close();
    bHandled = FALSE;
    return 0;
}

void CDuiMenu::Init(HWND hWndParent, POINT ptPos)
{
    Create(hWndParent, _T("MenuWnd"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    ::ClientToScreen(hWndParent, &ptPos);
    ::SetWindowPos(*this, NULL, ptPos.x, ptPos.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

LRESULT CDuiMenu::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;
    BOOL    bHandled = TRUE;

    switch (uMsg)
    {
    case WM_KILLFOCUS:
        lRes = OnKillFocus(uMsg, wParam, lParam, bHandled);
        break;
    default:
        bHandled = FALSE;
    }

    if (bHandled || m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes))
    {
        return lRes;
    }

    return __super::HandleMessage(uMsg, wParam, lParam);
}


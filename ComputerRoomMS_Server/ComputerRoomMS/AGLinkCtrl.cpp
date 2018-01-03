// AGLinkCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "AGLinkCtrl.h"


// CAGLinkCtrl

IMPLEMENT_DYNAMIC(CAGLinkCtrl, CStatic)

CAGLinkCtrl::CAGLinkCtrl()
    : m_crText(RGB(0x00, 0x9E, 0xEB))
    , m_crHover(RGB(0x00, 0xA0, 0xE9))
    , m_crBack(RGB(0xFF, 0xFF, 0xFF))
    , m_bOverControl(FALSE)
    , m_bTrackMouseEvent(FALSE)
    , m_bHandCursor(TRUE)
    , m_bAlignCenter(FALSE)
    , m_bTransparent(FALSE)
{
}


CAGLinkCtrl::~CAGLinkCtrl()
{

}


BEGIN_MESSAGE_MAP(CAGLinkCtrl, CStatic)
    ON_WM_MOUSEMOVE()
    ON_WM_SETCURSOR()
    ON_WM_PAINT()
    ON_WM_MOUSELEAVE()
    //ON_WM_CTLCOLOR()
    //ON_WM_CTLCOLOR_REFLECT()
    //ON_WM_ERASEBKGND()
    //ON_BN_CLICKED(IDC_INVITE_LINKCTRL, OnClickedInviteLinkCtrl)
END_MESSAGE_MAP()


void CAGLinkCtrl::SetAlignCenter(BOOL bValue)
{
    m_bAlignCenter = bValue;
}

void CAGLinkCtrl::SetTransparent(BOOL bValue)
{
    m_bTransparent = bValue;
}

void CAGLinkCtrl::SetHandCursor(BOOL bValue)
{
    m_bHandCursor = bValue;
}

void CAGLinkCtrl::PreSubclassWindow()
{
    // We want to get mouse clicks via STN_CLICKED
    DWORD dwStyle = GetStyle();
    ModifyStyle(0, SS_NOTIFY, 0);

    // Check that the window text isn't empty. If it is, set it as the URL.

    CStatic::PreSubclassWindow();
}


void CAGLinkCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    if (!m_bOverControl) {
        m_bOverControl = TRUE;
        Invalidate(FALSE);
    }

    if (!m_bTrackMouseEvent) {
        TRACKMOUSEEVENT TrackMouseEvent;
        TrackMouseEvent.cbSize = sizeof(TRACKMOUSEEVENT);
        TrackMouseEvent.hwndTrack = GetSafeHwnd();
        TrackMouseEvent.dwFlags = TME_LEAVE;
        TrackMouseEvent.dwHoverTime = HOVER_DEFAULT;

        m_bTrackMouseEvent = ::TrackMouseEvent(&TrackMouseEvent);
    }

    CStatic::OnMouseMove(nFlags, point);
}

void CAGLinkCtrl::OnPaint()
{
    UINT nFormat;

    CPaintDC dc(this);

    CString strWindowText;
    GetWindowText(strWindowText);

    CRect rcClient;
    GetClientRect(&rcClient);

    CFont *lpDefaultFont = dc.SelectObject(GetFont());

    if (m_bOverControl) {
        dc.SetTextColor(m_crHover);
    }
    else {
        dc.SetTextColor(m_crText);
    }

    if (m_bAlignCenter) {
        nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE | SS_CENTERIMAGE;
        /*	dc.DrawText(strWindowText, &rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);*/
    }
    else {
        nFormat = DT_LEFT | SS_CENTERIMAGE | DT_VCENTER | DT_SINGLELINE;
        //dc.DrawText(strWindowText, &rcClient, DT_LEFT);
    }

    //dc.SetBkColor(m_crBack);

    if (m_bTransparent)
    {
        dc.SetBkMode(TRANSPARENT);
        CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
        CBrush *lpDefaultBrush = dc.SelectObject(pBrush);
        dc.DrawText(strWindowText, &rcClient, nFormat);
        dc.SelectObject(lpDefaultBrush);
        pBrush->DeleteObject();
    }
    else
    {
        dc.SetBkMode(OPAQUE);
        CBrush brush;
        brush.CreateHatchBrush(BS_SOLID, m_crBack);
        CBrush *lpDefaultBrush = dc.SelectObject(&brush);
        dc.FillSolidRect(&rcClient, m_crBack);
        dc.DrawText(strWindowText, &rcClient, nFormat);

        dc.SelectObject(lpDefaultBrush);
        brush.DeleteObject();
    }

    dc.SelectObject(lpDefaultFont);

}


BOOL CAGLinkCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if (m_bHandCursor)
    {
        HCURSOR hLinkCursor = ::LoadCursor(NULL, IDC_HAND);
        if (hLinkCursor != NULL) {
            ::SetCursor(hLinkCursor);
            return TRUE;
        }
    }
    else
    {
        HCURSOR hLinkCursor = ::LoadCursor(NULL, IDC_ARROW);
        if (hLinkCursor != NULL) {
            ::SetCursor(hLinkCursor);
            return TRUE;
        }
    }

    return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CAGLinkCtrl::OnMouseLeave()
{
    // TODO:  在此添加消息处理程序代码和/或调用默认值
    m_bTrackMouseEvent = FALSE;
    m_bOverControl = FALSE;
    Invalidate(FALSE);

    CStatic::OnMouseLeave();
}

void CAGLinkCtrl::SetTextColor(COLORREF crText, COLORREF crHover)
{
    m_crText = crText;
    m_crHover = crHover;
}

void CAGLinkCtrl::SetBackColor(COLORREF crBack)
{
    m_crBack = crBack;
}

COLORREF CAGLinkCtrl::GetTextColor() const
{
    return m_crText;
}

COLORREF CAGLinkCtrl::GetHoverColor() const
{
    return m_crHover;
}

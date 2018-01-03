#pragma once


// CButtonCtrl

class CButtonCtrl : public CButton
{
    DECLARE_DYNAMIC(CButtonCtrl)

public:
    enum {
        AGBTN_NORMAL = 0,
        AGBTN_HOVER,
        AGBTN_PUSH,
        AGBTN_DISABLE
    };

    CButtonCtrl();
    virtual ~CButtonCtrl();

    BOOL SetBackImage(UINT nIDResource, COLORREF crMask = RGB(0xFF, 0x00, 0xFF));
    void SetBackColor(COLORREF crNormal, COLORREF crHover, COLORREF crPush, COLORREF crDisable);
    void SetTextColor(COLORREF crNormal, COLORREF crHover, COLORREF crPush, COLORREF crDisable);
    void SetBorderColor(COLORREF crNormal, COLORREF crHover, COLORREF crPush, COLORREF crDisable);
    void SetTipText(CString str);

    void EnableFrameEffect(BOOL bEnable = TRUE);
    void SwitchButtonStatus(UINT nStatus = AGBTN_NORMAL) { m_nDefStatus = nStatus; };
    UINT GetButtonStatus() { return m_nDefStatus; }

protected:
    virtual void PreSubclassWindow();
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnMouseLeave();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    virtual BOOL PreTranslateMessage(MSG* pMsg);

    DECLARE_MESSAGE_MAP()

protected:
    void DrawBack(LPDRAWITEMSTRUCT lpDrawItemStruct, UINT nStat);
    void DrawText(LPDRAWITEMSTRUCT lpDrawItemStruct, UINT nStat);
    void DrawFrame(LPDRAWITEMSTRUCT lpDrawItemStruct, UINT nStat);

private:
    CImageList	m_imgBack;

    COLORREF	m_crBackNormal;
    COLORREF	m_crBackHover;
    COLORREF	m_crBackPush;
    COLORREF	m_crBackDisable;

    COLORREF	m_crTextNormal;
    COLORREF	m_crTextHover;
    COLORREF	m_crTextPush;
    COLORREF	m_crTextDisable;

    COLORREF	m_crBorderNormal;
    COLORREF	m_crBorderHover;
    COLORREF	m_crBorderPush;
    COLORREF	m_crBorderDisable;

private:
    BOOL				m_bHover;
    BOOL				m_bMouseTrack;
    BOOL				m_bFrameEffect;
    UINT				m_nDefStatus;
    CToolTipCtrl		m_toolTipCtrl;
};



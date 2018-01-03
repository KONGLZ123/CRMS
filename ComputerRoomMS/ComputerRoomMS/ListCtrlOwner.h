#pragma once
#include "afxcmn.h"
#include <map>
#include "WinThreadDatabase.h"

using namespace std;

class CListCtrlOwner :
    public CListCtrl
{
public:
    CListCtrlOwner();
    ~CListCtrlOwner();

private:
    COLORREF					    m_BkColor;
    COLORREF					    m_TextColor;
    HICON						    m_hOnlineIcon;
    CFont                           m_FontInvite;

protected:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual void MeasureItem(LPMEASUREITEMSTRUCT  lpMeasureItemStruct);
    DECLARE_MESSAGE_MAP()
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    void OnMenuClicked();

public:
    void SetRowHeigt(int nHeight);
    void SetStyle(LIST_STYLE style);
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);


private:
    CWinThreadDatabase *        m_pDbThread;
    int m_nRowHeight;
    CEdit m_EditItem;
    int m_Row;
    int m_Col;
    int m_delItem;
    LIST_STYLE m_style;

public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
};




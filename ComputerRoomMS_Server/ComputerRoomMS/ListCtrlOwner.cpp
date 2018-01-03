#include "stdafx.h"
#include "ListCtrlOwner.h"
#include "resource.h"

CListCtrlOwner::CListCtrlOwner()
{
    m_BkColor = RGB(255, 255, 255);
    m_TextColor = RGB(0, 0, 0);
}


CListCtrlOwner::~CListCtrlOwner()
{
    m_delItem = -1;
}

BEGIN_MESSAGE_MAP(CListCtrlOwner, CListCtrl)
    ON_WM_MEASUREITEM_REFLECT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_RBUTTONDOWN()
    ON_BN_CLICKED(ID_DELETE_PERSON_ITEM, OnMenuClicked)
    ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()


int SplitString(const CString str, char split, CStringArray &strArray)
{
    strArray.RemoveAll();
    CString strSource = str;
    int iIndex = 0;
    while (1)
    {
        iIndex = strSource.Find(split);
        if (iIndex >= 0) {
            strArray.Add(strSource.Left(iIndex));
            CString strTemp = strSource.Right(strSource.GetLength() - iIndex - 1);
            strSource = strTemp;
        }
        else {
            break;
        }
    }
    strArray.Add(strSource);

    return strArray.GetSize();
}

void CListCtrlOwner::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

    CRect rcBack = lpDrawItemStruct->rcItem;
    pDC->SetBkMode(TRANSPARENT);
    pDC->FillRect(rcBack, &CBrush(m_BkColor));

  
    if (lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
    {
        CString szText;
        for (int i = 0; i < GetHeaderCtrl()->GetItemCount(); i++)  //循环得到文本 
        {
            CRect rcItem;
            if (GetSubItemRect(lpDrawItemStruct->itemID, i, LVIR_LABEL, rcItem))
            {
                pDC->SetTextColor(m_TextColor);
                szText = GetItemText(lpDrawItemStruct->itemID, i);
                
                CSize size = pDC->GetTextExtent(szText);
                CRect rcTemp = rcItem;
                rcTemp.bottom = rcTemp.top + size.cy;

                CStringArray strArray;
                int nCount = SplitString(szText, '\n', strArray);
                //for (int i = 0; i < strArray.GetSize(); i++)
                if (3 == nCount)
                {
                    CString strTemp = _T("  ") + strArray.GetAt(0);
                    pDC->DrawText(strTemp, lstrlen(strTemp), &rcTemp, DT_LEFT | DT_SINGLELINE /*| DT_VCENTER*/ | DT_END_ELLIPSIS);
                    rcTemp.top = rcTemp.bottom + 2;
                    rcTemp.bottom = rcTemp.top + size.cy;
                    strTemp = _T("  ") + strArray.GetAt(1);
                    pDC->DrawText(strTemp, lstrlen(strTemp), &rcTemp, DT_LEFT | DT_SINGLELINE /*| DT_VCENTER*/ | DT_END_ELLIPSIS);
                    rcTemp.top = rcTemp.bottom + 2;
                    rcTemp.bottom = rcTemp.top + size.cy;
                    strTemp = _T("  ") + strArray.GetAt(2);
                    pDC->DrawText(strTemp, lstrlen(strTemp), &rcTemp, DT_LEFT | DT_SINGLELINE /*| DT_VCENTER*/ | DT_END_ELLIPSIS);
                    rcTemp.top = rcTemp.bottom + 2;
                    rcTemp.bottom = rcTemp.top + size.cy;
                }
                else
                {
                    szText = _T("  ") + szText;
                    pDC->DrawText(szText, lstrlen(szText), &rcItem, DT_LEFT | DT_SINGLELINE /*| DT_VCENTER*/ | DT_END_ELLIPSIS);
                }
            }
        }
    }
}


void CListCtrlOwner::MeasureItem(LPMEASUREITEMSTRUCT  lpMeasureItemStruct)
{
    lpMeasureItemStruct->itemHeight = m_nRowHeight;
    //lpMeasureItemStruct->itemWidth  = 100;
}

void CListCtrlOwner::SetRowHeigt(int nHeight)
{   
    m_nRowHeight = nHeight;  
    CRect rcWin;   
    GetWindowRect(&rcWin);  
    WINDOWPOS wp;  
    wp.hwnd = m_hWnd;  
    wp.cx = rcWin.Width();  
    wp.cy = rcWin.Height();  
    wp.flags  =  SWP_NOACTIVATE  |  SWP_NOMOVE  |  SWP_NOOWNERZORDER | SWP_NOZORDER;   
    SendMessage(WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp); 
}

void CListCtrlOwner::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (m_EditItem.m_hWnd != NULL)
    {
        m_EditItem.ShowWindow(0);
        if (m_Row != -1)
        {
            CString ItemText;
            m_EditItem.GetWindowText(ItemText);
            this->SetItemText(m_Row, m_Col, ItemText);
            //::PostMessage(GetParent()->m_hWnd, MLSM_ITEMCHANGED, (WPARAM)MAKELONG(m_Row, m_Col), (LPARAM)this->m_hWnd);
        }
    }
    m_Col = m_Row = -1;

    CListCtrl::OnLButtonDown(nFlags, point);
}


void CListCtrlOwner::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    LVHITTESTINFO hi;
    hi.pt = point;

    if (SubItemHitTest(&hi) != -1)
    {
        m_Row = hi.iItem;
        m_Col = hi.iSubItem;
        if (m_EditItem.m_hWnd == NULL)
        {
            RECT rect;
            rect.left = rect.top = 0;
            rect.bottom = 20;
            rect.right = 100;
            m_EditItem.Create(WS_CHILD | ES_LEFT | WS_BORDER | ES_AUTOHSCROLL | ES_WANTRETURN | ES_MULTILINE, rect, this, 1001);
            m_EditItem.SetFont(this->GetFont(), FALSE);
        }
        CRect rect;
        GetSubItemRect(hi.iItem, hi.iSubItem, LVIR_BOUNDS, rect);
        m_EditItem.SetWindowText(this->GetItemText(hi.iItem, hi.iSubItem));
        m_EditItem.MoveWindow(&rect, TRUE);
        m_EditItem.ShowWindow(1);
    }

    CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CListCtrlOwner::OnMenuClicked()
{
    if (-1 != m_delItem)
    {
        if (IDOK == ::MessageBox(NULL, _T("您确定要删除当前信息吗？"), _T("提示"), MB_OKCANCEL))
        {
            DeleteItem(m_delItem);
            m_delItem = -1;
        }
    }
}


BOOL CListCtrlOwner::PreTranslateMessage(MSG* pMsg)
{
    BOOL bHandledMsg = FALSE;
        
    if (pMsg->hwnd == m_EditItem.m_hWnd)
    {
        switch (pMsg->message)
        {
        case WM_KEYDOWN:
        {
            switch (pMsg->wParam)
            {
                case VK_RETURN: //回车
                    if (m_Row != -1)
                    {
                        CString ItemText;
                        m_EditItem.GetWindowText(ItemText);
                        this->SetItemText(m_Row, m_Col, ItemText);
                        //::PostMessage(GetParent()->m_hWnd, MLSM_ITEMCHANGED, (WPARAM)MAKELONG(m_Row, m_Col), (LPARAM)this->m_hWnd);
                    }
                    //break;
                case VK_ESCAPE: //ESC键
                    m_EditItem.ShowWindow(0);
                    m_Col = m_Row = -1;
                    bHandledMsg = TRUE;
                    break;
                default:
                    break;
            }
        }
        break;
        default:
            break;
        }
    }

    return CListCtrl::PreTranslateMessage(pMsg);
}


void CListCtrlOwner::OnRButtonDown(UINT nFlags, CPoint point)
{
    int nItem = GetNextItem(-1, LVNI_SELECTED);
    if (-1 != nItem)
    {
        m_delItem = nItem;
        //SetItemState(-1, LVIS_SELECTED, LVIS_SELECTED);
        CPoint pt;
        GetCursorPos(&pt);

        CMenu menu;
        menu.LoadMenu(IDR_MENU1);
        CMenu *pContextMenu = menu.GetSubMenu(0);
        pContextMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
    }

    CListCtrl::OnRButtonDown(nFlags, point);
}


void CListCtrlOwner::OnMouseHover(UINT nFlags, CPoint point)
{


    CListCtrl::OnMouseHover(nFlags, point);
}

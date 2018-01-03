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
                int nCount = (SplitString(szText, '|', strArray));
                //for (int i = 0; i < strArray.GetSize(); i++)
                if (4 == nCount)
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
                    strTemp = _T("  ") + strArray.GetAt(3);
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

void CListCtrlOwner::SetStyle(LIST_STYLE style)
{
    m_style = style;
}

void CListCtrlOwner::SetThreadDatabase(CWinThreadDatabase * pDbThread)
{
    m_pDbThread = pDbThread;
}

void CListCtrlOwner::OnLButtonDown(UINT nFlags, CPoint point)
{
    if (LIST_STYLE_PERSON_INFO == m_style)
    {
        if (m_EditItem.m_hWnd != NULL)
        {
            m_EditItem.ShowWindow(0);
            if (m_Row != -1)
            {
                CString ItemText;
                m_EditItem.GetWindowText(ItemText);

                PERSON_INFO_PART *pPersonInfo = new PERSON_INFO_PART;
                USES_CONVERSION;
                pPersonInfo->oldAuthority = atoi(W2A(this->GetItemText(m_Row, 5)));

                this->SetItemText(m_Row, m_Col, ItemText);

                pPersonInfo->account = this->GetItemText(m_Row, 1);
                pPersonInfo->pwd = this->GetItemText(m_Row, 2);
                pPersonInfo->name = this->GetItemText(m_Row, 3);
                pPersonInfo->tel = this->GetItemText(m_Row, 4);
                pPersonInfo->authority = atoi(W2A(this->GetItemText(m_Row, 5)));
                ::SendMessage(this->GetParent()->m_hWnd, WM_UPDATE_LIST_ITEM, (WPARAM)pPersonInfo, 0);
                PostThreadMessage(m_pDbThread->m_nThreadID, WM_UPDATE_PERSON_INFO, (WPARAM)pPersonInfo, 0);
            }
        }
        m_Col = m_Row = -1;
    }
    else if (LIST_STYLE_STUDENT_MANAGER == m_style) 
    {
        if (m_EditItem.m_hWnd != NULL)
        {
            m_EditItem.ShowWindow(0);
            if (m_Row != -1)
            {
                CString ItemText;
                m_EditItem.GetWindowText(ItemText);

                STUDETN_INFO *pStuInfo = new STUDETN_INFO;
                USES_CONVERSION;
                //pStuInfo->attendece_cnt = atoi(W2A(this->GetItemText(m_Row, 3)));
                //pStuInfo->attendece_score = atoi(W2A(this->GetItemText(m_Row, 4)));

                this->SetItemText(m_Row, m_Col, ItemText);

                pStuInfo->student_id = this->GetItemText(m_Row, 1);
                ::SendMessage(this->GetParent()->m_hWnd, WM_UPDATE_STUDENT_LIST_ITEM, (WPARAM)pStuInfo, 0);
                PostThreadMessage(m_pDbThread->m_nThreadID, WM_UPDATE_EXAM_INFO, (WPARAM)pStuInfo, 0);
            }
        }
        m_Col = m_Row = -1;
    }
    else if (LIST_STYLE_SALARY == m_style)
    {
        if (m_EditItem.m_hWnd != NULL)
        {
            m_EditItem.ShowWindow(0);
            if (m_Row != -1)
            {
                if (6 == m_Col)
                {
                    CString ItemText;
                    m_EditItem.GetWindowText(ItemText);
                    this->SetItemText(m_Row, m_Col, ItemText);

                    WORKLOAD_REPORT *pWorkload = new WORKLOAD_REPORT;
                    USES_CONVERSION;
                    pWorkload->strName = this->GetItemText(m_Row, 1);
                    pWorkload->strDate = this->GetItemText(m_Row, 2);
                    pWorkload->strWorkloadSalary = this->GetItemText(m_Row, 6);
                    if (this->GetItemText(m_Row, 7) == _T("是")) {
                        pWorkload->isView = 1;
                    }
                    else {
                        pWorkload->isView = 0;
                    }
                    ::SendMessage(this->GetParent()->m_hWnd, WM_UPDATE_SALARY_LIST_ITEM, (WPARAM)pWorkload, 0);
                    PostThreadMessage(m_pDbThread->m_nThreadID, WM_UPDATE_SALARY_INFO, (WPARAM)pWorkload, 0);
                }
            }
        }
        m_Col = m_Row = -1;
    }

    CListCtrl::OnLButtonDown(nFlags, point);
}


void CListCtrlOwner::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    if (LIST_STYLE_PERSON_INFO == m_style)
    {
        LVHITTESTINFO hi;
        hi.pt = point;

        if (SubItemHitTest(&hi) != -1)
        {
            m_Row = hi.iItem;
            m_Col = hi.iSubItem;

            if (0 == m_Col || 1 == m_Col)
            {
                MessageBox(_T("此列不可更改"));
                return;
            }

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
    }
    else if (LIST_STYLE_STUDENT_MANAGER == m_style)
    {
        LVHITTESTINFO hi;
        hi.pt = point;

        if (SubItemHitTest(&hi) != -1)
        {
            m_Row = hi.iItem;
            m_Col = hi.iSubItem;

            if (3 == m_Col || 4 == m_Col)
            {
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
            else
            {
                MessageBox(_T("此列不可更改"));
                return;
            }
        }
    }
    else if (LIST_STYLE_SALARY == m_style)
    {
        LVHITTESTINFO hi;
        hi.pt = point;

        if (SubItemHitTest(&hi) != -1)
        {
            m_Row = hi.iItem;
            m_Col = hi.iSubItem;

            int isView;
            if (6 == m_Col)
            {
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
            else if (7 == m_Col) {
                if (this->GetItemText(m_Row, m_Col) == _T("否")) {
                    this->SetItemText(m_Row, m_Col, _T("是"));
                    isView = 1;
                }
                else {
                    this->SetItemText(m_Row, m_Col, _T("否"));
                    isView = 0;
                }

                WORKLOAD_REPORT *pWorkload = new WORKLOAD_REPORT;
                USES_CONVERSION;
                pWorkload->strName = this->GetItemText(m_Row, 1);
                pWorkload->strDate = this->GetItemText(m_Row, 2);
                pWorkload->strWorkloadSalary = this->GetItemText(m_Row, 6);
                pWorkload->isView = isView;
                ::SendMessage(this->GetParent()->m_hWnd, WM_UPDATE_SALARY_LIST_ITEM, (WPARAM)pWorkload, 0);
                PostThreadMessage(m_pDbThread->m_nThreadID, WM_UPDATE_SALARY_INFO, (WPARAM)pWorkload, 0);
            }
            else
            {
                MessageBox(_T("此列不可更改"));
                return;
            }
        }
    }
    else if (m_style == LIST_STYLE_ROOM_REQUEST)
    {
        LVHITTESTINFO hi;
        hi.pt = point;

        if (SubItemHitTest(&hi) != -1)
        {
            m_Row = hi.iItem;
            m_Col = hi.iSubItem;

            int isView;
            if (5 == m_Col) {
                if (this->GetItemText(m_Row, m_Col) == _T("否")) {
                    this->SetItemText(m_Row, m_Col, _T("是"));
                    isView = 1;
                }
                else {
                    this->SetItemText(m_Row, m_Col, _T("否"));
                    isView = 0;
                }
                
                REPORT_DATA *pReportData = new REPORT_DATA;
                pReportData->submitPerson = this->GetItemText(m_Row, 1);
                pReportData->uploadDate = this->GetItemText(m_Row, 2);
                pReportData->isView = isView;
                ::SendMessage(this->GetParent()->m_hWnd, WM_UPDATE_ROOM_LIST, (WPARAM)pReportData, 0);
                PostThreadMessage(m_pDbThread->m_nThreadID, WM_UPDATE_ROOM_REQUEST, (WPARAM)pReportData, 0);
            }
        }
    }

    CListCtrl::OnLButtonDblClk(nFlags, point);
}

void CListCtrlOwner::OnMenuClicked()
{
    if (LIST_STYLE_PERSON_INFO == m_style)
    {
        if (-1 != m_delItem)
        {
            if (IDOK == ::MessageBox(NULL, _T("您确定要删除当前信息吗？"), _T("提示"), MB_OKCANCEL))
            {
                PERSON_INFO_PART *pPersonInfo = new PERSON_INFO_PART;
                pPersonInfo->account = this->GetItemText(m_delItem, 1);
                USES_CONVERSION;
                pPersonInfo->authority = atoi(W2A(this->GetItemText(m_delItem, 5)));
                ::SendMessage(this->GetParent()->m_hWnd, WM_DEL_LIST_ITEM, (WPARAM)pPersonInfo, 0);
                PostThreadMessage(m_pDbThread->m_nThreadID, WM_DEL_PERSON_INFO, (WPARAM)pPersonInfo, 0);

                DeleteItem(m_delItem);
                m_delItem = -1;
            }
        }
    }
}


BOOL CListCtrlOwner::PreTranslateMessage(MSG* pMsg)
{
    if (LIST_STYLE_PERSON_INFO == m_style)
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
    }


    return CListCtrl::PreTranslateMessage(pMsg);
}


void CListCtrlOwner::OnRButtonDown(UINT nFlags, CPoint point)
{
    if (LIST_STYLE_PERSON_INFO == m_style)
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
    }

    CListCtrl::OnRButtonDown(nFlags, point);
}


void CListCtrlOwner::OnMouseHover(UINT nFlags, CPoint point)
{

    CListCtrl::OnMouseHover(nFlags, point);
}

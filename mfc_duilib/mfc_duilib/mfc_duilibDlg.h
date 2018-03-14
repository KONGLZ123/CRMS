
// mfc_duilibDlg.h : 头文件
//

#pragma once

#include "DuiMenu.h"
#include "UIlib.h"
using namespace DuiLib;


#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif

class CWndUI : public CControlUI
{
public:
    CWndUI() : m_hWnd(NULL) {}


    virtual void SetInternVisible(bool bVisible = true)
    {
        __super::SetInternVisible(bVisible);
        ::ShowWindow(m_hWnd, bVisible);
    }

    virtual void SetPos(RECT rc)
    {
        __super::SetPos(rc);
        ::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
    }

    BOOL Attach(HWND hWndNew)
    {
        if (!::IsWindow(hWndNew))
        {
            return FALSE;
        }

        m_hWnd = hWndNew;
        return TRUE;
    }

    HWND Detach()
    {
        HWND hWnd = m_hWnd;
        m_hWnd = NULL;
        return hWnd;
    }

protected:
    HWND m_hWnd;
};

class CDuiFrameWnd : public WindowImplBase
{
public:
    virtual CDuiString GetSkinFolder() { return _T(""); }
    virtual CDuiString GetSkinFile() { return _T("duilib.xml"); }
    virtual LPCTSTR GetWindowClassName(void) const { return _T("MainFrame"); }

    virtual void InitWindow() {
        CActiveXUI *pActiveXUI = static_cast<CActiveXUI *>(m_PaintManager.FindControl(_T("ActiveXDemo1")));
        CProgressUI *pProgress = static_cast<CProgressUI *>(m_PaintManager.FindControl(_T("ProgressDemo2")));
        pProgress->SetValue(100);
        CSliderUI *pSlider = static_cast<CSliderUI *>(m_PaintManager.FindControl(_T("SliderDemo1")));

        CDuiString str;
        CListUI* pList = static_cast<CListUI*>(m_PaintManager.FindControl(_T("ListDemo1")));

        // 添加List列表内容，必须先Add(pListElement)，再SetText
        for (int i = 0; i < 100; i++)
        {
            CListTextElementUI* pListElement = new CListTextElementUI;
            pListElement->SetTag(i);
            pList->Add(pListElement);

            str.Format(_T("%d"), i);
            pListElement->SetText(0, str);
            pListElement->SetText(1, _T("haha"));
        }

        if (pActiveXUI) {
            IWebBrowser2 *pWebBrowser = NULL;
            pActiveXUI->SetDelayCreate(false);
            pActiveXUI->CreateControl(CLSID_WebBrowser);
            pActiveXUI->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);

            if (pWebBrowser) {
                pWebBrowser->Navigate(L"http://www.google.com/", NULL, NULL, NULL, NULL);
                pWebBrowser->Release();
            }
        }
    }
    virtual void Notify(TNotifyUI& msg) {
        if (msg.sType == _T("click")) {
            if (msg.pSender->GetName() == _T("btnMenu")) {
                POINT pt = { msg.ptMouse.x, msg.ptMouse.y };
                CDuiMenu *pMenu = new CDuiMenu(_T("menu.xml"));

                pMenu->Init(*this, pt);
                pMenu->ShowWindow(TRUE);
            }
        }

        __super::Notify(msg);
    }

    virtual CControlUI* CreateControl(LPCTSTR pstrClassName)
    {
        if (_tcsicmp(pstrClassName, _T("Wnd")) == 0)
        {
            CWndUI  *pUI = new CWndUI;
            //HWND    hWnd = CreateWindow(_T("BUTTON"), _T("win32"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 0, 0, 0, 0, m_PaintManager.GetPaintWindow(), NULL, NULL, NULL);
            //pUI->Attach(hWnd);

            //// 上面用的是win32的按钮，下面这段用MFC的按钮
            CButton *pBtn = new CButton;    // 记得释放内存
            pBtn->Create(_T("MFC"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, CRect(0, 0, 0, 0), CWnd::FromHandle(m_PaintManager.GetPaintWindow()), 0);
            pUI->Attach(*pBtn);            
            return pUI;
        }

        return NULL;
    }
};



// Cmfc_duilibDlg 对话框
class Cmfc_duilibDlg : public CDialogEx
{
// 构造
public:
	Cmfc_duilibDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_DUILIB_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedOk();

    CDuiFrameWnd m_duiFrame;
};

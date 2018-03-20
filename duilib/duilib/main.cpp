#include <windows.h>
#include <tchar.h>
#include <memory>
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

class CDuiFrameWnd : public WindowImplBase
{
public:
    virtual CDuiString GetSkinFolder() { return _T(""); }
    virtual CDuiString GetSkinFile() { return _T("duilib.xml"); }
    virtual LPCTSTR GetWindowClassName(void) const { return _T("MainFrame"); }

    virtual void InitWindow();
    virtual void Notify(TNotifyUI& msg);
};

void CDuiFrameWnd::Notify(TNotifyUI& msg)
{
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

void CDuiFrameWnd::InitWindow() 
{
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

//class CDuiFrameWnd : public CWindowWnd, public INotifyUI
//{
//public:
//    virtual LPCTSTR GetWindowClassName() const { return _T("DuiMainFrame"); };
//    virtual void Notify(TNotifyUI& msg) { 
//        if (msg.sType == _T("click")) {
//            if (msg.pSender->GetName() == _T("btnHello")) {
//                ::MessageBox(NULL, _T("我是按钮"), _T("点击了按钮"), NULL);
//            }
//        }
//    }
//
//    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
//        LRESULT lRes = 0;
//        if (uMsg == WM_CREATE) {
//            //CControlUI *pWnd = new CButtonUI;
//            //pWnd->SetName(_T("btnName"));
//            //pWnd->SetText(_T("Button"));
//            //pWnd->SetBkColor(0xFF00FF00);
//
//            m_paintManager.Init(m_hWnd);
//
//            CDialogBuilder builder;
//            CControlUI *pRoot = builder.Create(_T("duilib.xml"), (UINT)0, NULL, &m_paintManager);
//            ASSERT(pRoot && "Failed to prase XML");
//
//            m_paintManager.AttachDialog(pRoot);
//            m_paintManager.AddNotifier(this);
//
//            return lRes;
//        }
//        else if (uMsg == WM_NCACTIVATE) {
//            if (!::IsIconic(m_hWnd)) {
//                return (wParam == 0) ? TRUE : FALSE;
//            }
//        }
//        else if (uMsg == WM_NCPAINT) {
//            return 0;
//        }
//        else if (uMsg == WM_NCCALCSIZE) {
//            return 0;
//        }
//
//        if (m_paintManager.MessageHandler(uMsg, wParam, lParam, lRes)) {
//            return lRes;
//        }
//
//        return __super::HandleMessage(uMsg, wParam, lParam);
//    }
//
//protected:
//    CPaintManagerUI m_paintManager;
//};

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    CPaintManagerUI::SetInstance(hInstance);
    //CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
    HRESULT hr = ::CoInitialize(NULL);
    if (FAILED(hr))
        return 0;

    CDuiFrameWnd duiFrame;
    duiFrame.Create(NULL, _T("主框架窗口"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
    duiFrame.CenterWindow();
    duiFrame.ShowModal();

    ::CoUninitialize();
    return 0;
}
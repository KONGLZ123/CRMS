#pragma once
#include "UIlib.h"
using namespace DuiLib;

class CDuiMenu : public WindowImplBase
{
protected:
    virtual ~CDuiMenu() {};        // ˽�л����������������˶���ֻ��ͨ��new�����ɣ�������ֱ�Ӷ���������ͱ�֤��delete this�������
    CDuiString  m_strXMLPath;

public:
    explicit CDuiMenu(LPCTSTR pszXMLPath) : m_strXMLPath(pszXMLPath) {}
    virtual LPCTSTR    GetWindowClassName()const { return _T("CDuiMenu "); }
    virtual CDuiString GetSkinFolder() { return _T(""); }
    virtual CDuiString GetSkinFile() { return m_strXMLPath; }
    virtual void       OnFinalMessage(HWND hWnd) { delete this; }

    virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

    void Init(HWND hWndParent, POINT ptPos);

    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

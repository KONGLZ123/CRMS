#pragma once
#include "afxwin.h"
#include "ListCtrlOwner.h"
#include "afxcmn.h"

// CAssetsListDlg 对话框

class CAssetsListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAssetsListDlg)

public:
	CAssetsListDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAssetsListDlg();

	enum { IDD = IDD_DLG_ASSETS_LIST };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    virtual BOOL OnInitDialog();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    afx_msg void OnBnClickedButton4();
	DECLARE_MESSAGE_MAP()

public:
    void SetThreadDatabase(CWinThreadDatabase *pDbThread);
    void SetAssertDate(vector<ASSERT_DATA> & vecAssertData);

private:
    CWinThreadDatabase *    m_pDbThread;
    vector<ASSERT_DATA>     m_vecAssertData;
    CListCtrlOwner          m_listRoomAssets;
};

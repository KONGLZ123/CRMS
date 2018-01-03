#pragma once
#include "afxwin.h"
#include "ListCtrlOwner.h"
#include "afxcmn.h"

// CAssetsListDlg �Ի���

class CAssetsListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAssetsListDlg)

public:
	CAssetsListDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAssetsListDlg();

	enum { IDD = IDD_DLG_ASSETS_LIST };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
    CListCtrlOwner m_listRoomAssets;
    void SetAssertDate(vector<ASSERT_DATA> & vecAssertData);
    vector<ASSERT_DATA>     m_vecAssertData;
    virtual BOOL PreTranslateMessage(MSG* pMsg);
};

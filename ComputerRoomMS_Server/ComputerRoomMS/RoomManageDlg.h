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
    CListCtrl m_listRoomAssets;
};


// mfc_duilib.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cmfc_duilibApp: 
// �йش����ʵ�֣������ mfc_duilib.cpp
//

class Cmfc_duilibApp : public CWinApp
{
public:
	Cmfc_duilibApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cmfc_duilibApp theApp;
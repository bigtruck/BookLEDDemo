
// BookLED_Demo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBookLED_DemoApp:
// �йش����ʵ�֣������ BookLED_Demo.cpp
//

class CBookLED_DemoApp : public CWinApp
{
public:
	CBookLED_DemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CBookLED_DemoApp theApp;
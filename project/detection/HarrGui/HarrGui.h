// HarrGui.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHarrGuiApp:
// �йش����ʵ�֣������ HarrGui.cpp
//

class CHarrGuiApp : public CWinApp
{
public:
	CHarrGuiApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHarrGuiApp theApp;
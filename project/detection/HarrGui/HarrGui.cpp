// HarrGui.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "HarrGui.h"
#include "HarrGuiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHarrGuiApp

BEGIN_MESSAGE_MAP(CHarrGuiApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CHarrGuiApp 构造

CHarrGuiApp::CHarrGuiApp()
{
	// TODO: 在此处添加构造代码，
}

CHarrGuiApp theApp;

// CHarrGuiApp 初始化

BOOL CHarrGuiApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CHarrGuiDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	return FALSE;
}

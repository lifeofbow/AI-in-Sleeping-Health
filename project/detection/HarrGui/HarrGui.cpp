// HarrGui.cpp : ����Ӧ�ó��������Ϊ��
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


// CHarrGuiApp ����

CHarrGuiApp::CHarrGuiApp()
{
	// TODO: �ڴ˴���ӹ�����룬
}

CHarrGuiApp theApp;

// CHarrGuiApp ��ʼ��

BOOL CHarrGuiApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CHarrGuiDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˴����ô����ʱ�á�ȷ�������ر�
		//  �Ի���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ�á�ȡ�������ر�
		//  �Ի���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	return FALSE;
}

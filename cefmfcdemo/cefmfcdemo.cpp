
// cefmfcdemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "cefmfcdemo.h"
#include "MainFrm.h"

#include "CefDoc.h"
#include "CefView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CefMfcdDemoApp

BEGIN_MESSAGE_MAP(CefMfcdDemoApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CefMfcdDemoApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CefMfcdDemoApp construction

CefMfcdDemoApp::CefMfcdDemoApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("cefmfcdemo.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CefMfcdDemoApp object

CefMfcdDemoApp theApp;


// CefMfcdDemoApp initialization

BOOL CefMfcdDemoApp::InitInstance()
{
	m_bCEFInitialized = false;
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

   InitializeCef();

	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CefDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CefView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CefMfcdDemoApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

   UninitializeCef();

	return CWinApp::ExitInstance();
}

BOOL CefMfcdDemoApp::OnIdle(LONG lCount)
{
	if(m_bCEFInitialized)
		CefDoMessageLoopWork();
	return CWinApp::OnIdle(lCount);
}

// CefMfcdDemoApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CefMfcdDemoApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CefMfcdDemoApp message handlers


void CefMfcdDemoApp::InitializeCef()
{
   CefMainArgs mainargs(m_hInstance);

   // setup settings
   CString szCEFCache;
   CString szPath;
   INT nLen = GetTempPath(0, NULL) + 1;
   GetTempPath(nLen, szPath.GetBuffer(nLen));

   // save path
   szCEFCache.Format(_T("%scache\0\0"), szPath);

   CefSettings settings;
   CString sLog;
   sLog.Format(_T("C:/Cef/Log22.txt"));

   CString agent;
   agent.Format(_T("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:78.0) Gecko/20100101 Firefox/78.0"));

   CefString(&settings.cache_path) = szCEFCache;
   settings.no_sandbox = TRUE;
   settings.multi_threaded_message_loop = false;
   settings.log_severity = LOGSEVERITY_DEFAULT;

   CefString(&settings.user_agent) = agent;
   CefString(&settings.log_file) = sLog;
      
   m_bCEFInitialized = CefInitialize(mainargs, settings, m_app, nullptr);

}

void CefMfcdDemoApp::UninitializeCef()
{
	if (m_bCEFInitialized)
	{
		m_bCEFInitialized = FALSE;
		CefShutdown();
	}

}


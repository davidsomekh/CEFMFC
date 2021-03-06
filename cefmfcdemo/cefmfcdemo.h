
// cefmfcdemo.h : main header file for the cefmfcdemo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "include/cef_base.h"
#include "include/cef_app.h" 

// CefMfcdDemoApp:
// See cefmfcdemo.cpp for the implementation of this class
//

class CefMfcdDemoApp : public CWinApp
{
public:
	CefMfcdDemoApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
   virtual BOOL OnIdle(LONG lCount) override;

   BOOL					m_bCEFInitialized;

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

private:
   void InitializeCef();
   void UninitializeCef();

   CefRefPtr<CefApp> m_app;
};

extern CefMfcdDemoApp theApp;


// CefView.cpp : implementation of the CefView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "cefmfcdemo.h"
#endif

#include "CefDoc.h"
#include "CefView.h"
#include "CefWindowsHelpers.h"
#include "include\internal\cef_string_wrappers.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CefView

IMPLEMENT_DYNCREATE(CefView, CView)

BEGIN_MESSAGE_MAP(CefView, CView)
   ON_WM_SIZE()
END_MESSAGE_MAP()

// CefView construction/destruction

CefView::CefView()
{
	// TODO: add construction code here

}

CefView::~CefView()
{
   if(m_clientHandler != nullptr)
      m_clientHandler->DetachDelegate();
}

BOOL CefView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

void CefView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
   m_wndMain = AfxGetMainWnd();

   return CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

// CefView drawing

void CefView::OnDraw(CDC* /*pDC*/)
{
	CefDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CefView diagnostics

#ifdef _DEBUG
void CefView::AssertValid() const
{
	CView::AssertValid();
}

void CefView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CefDoc* CefView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CefDoc)));
	return (CefDoc*)m_pDocument;
}
#endif //_DEBUG


void CefView::OnBrowserCreated(CefRefPtr<CefBrowser> browser)
{
   m_browser = browser;
}

void CefView::OnBrowserClosing(CefRefPtr<CefBrowser> browser)
{
}

void CefView::OnBrowserClosed(CefRefPtr<CefBrowser> browser)
{
   if(m_browser != nullptr && 
      m_browser->GetIdentifier() == browser->GetIdentifier())
   {
      m_browser = nullptr;

      m_clientHandler->DetachDelegate();
   }
}

void CefView::OnSetAddress(std::string const & url)
{
   auto main = static_cast<CMainFrame*>(m_wndMain);
   if(main != nullptr)
   {
      auto newurl = CString {url.c_str()};
      if(newurl.Find(m_startUrl) >= 0)
         newurl = "";

      main->SetUrl(newurl);
   }
}

void CefView::OnSetTitle(std::string const & title)
{
//   ::SetWindowText(m_hWnd, CefString(title).ToWString().c_str());
}

void CefView::OnSetFullscreen(bool const fullscreen)
{
   if(m_browser != nullptr)
   {
      if(fullscreen)
      {
         CefWindowsHelpers::Maximize(m_browser);
      }
      else 
      {
         CefWindowsHelpers::Restore(m_browser);
      }
   }
}

void CefView::OnSetLoadingState(bool const isLoading,
   bool const canGoBack,
   bool const canGoForward)
{
}

void CefView::OnInitialUpdate()
{
   CView::OnInitialUpdate();

   InitStartUrl();

   auto rect = RECT{0};
   GetClientRect(&rect);

   CefWindowInfo info;
   info.SetAsChild(GetSafeHwnd(), rect);

   CefBrowserSettings browserSettings;
   browserSettings.web_security = STATE_DISABLED;

   m_clientHandler = new ClientHandler(this);
   m_clientHandler->CreateBrowser(info, browserSettings, CefString(m_startUrl));
}

void CefView::OnSize(UINT nType, int cx, int cy)
{
   CView::OnSize(nType, cx, cy);

   if(m_clientHandler != nullptr)
   {
      if(m_browser != nullptr)
      {
         auto hwnd = m_browser->GetHost()->GetWindowHandle();
         auto rect = RECT {0};
         GetClientRect(&rect);

         ::SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
      }
   }   
}

BOOL CefView::PreTranslateMessage(MSG* pMsg)
{
   if(pMsg->message == WM_KEYDOWN)
   {
      if(pMsg->wParam == VK_F5)
      {
         m_browser->Reload();
      }
   }

   return CView::PreTranslateMessage(pMsg);  
}

void CefView::Navigate(CString const & url)
{
   if(m_browser != nullptr)
   {
      auto frame = m_browser->GetMainFrame();
      if(frame != nullptr)
      {
          frame->LoadURL(CefString(url));
      }
   }
}

void CefView::NavigateBack()
{
   if(m_browser != nullptr)
   {
      m_browser->GoBack();
   }
}

void CefView::NavigateForward()
{
   if(m_browser != nullptr)
   {
      m_browser->GoForward();
   }
}

bool CefView::CanNavigateBack()
{
   return m_browser != nullptr && m_browser->CanGoBack();
}

bool CefView::CanNavigateForward()
{
   return m_browser != nullptr && m_browser->CanGoForward();
}

void CefView::InitStartUrl()
{
   TCHAR path_buffer[_MAX_PATH] = {0};
   TCHAR drive[_MAX_DRIVE] = {0};
   TCHAR dir[_MAX_DIR] = {0};
   TCHAR fname[_MAX_FNAME] = {0};
   TCHAR ext[_MAX_EXT] = {0};

   ::GetModuleFileName(NULL, path_buffer, sizeof(path_buffer));
   auto err = _tsplitpath_s(path_buffer, drive, _MAX_DRIVE, dir, _MAX_DIR, fname, _MAX_FNAME, ext, _MAX_EXT);
   if(err != 0) {}

   auto s = CString{dir};
   s += _T("html");
   err = _tmakepath_s(path_buffer, _MAX_PATH, drive, (LPCTSTR)s, _T("index"), _T("html"));
   if(err != 0) {}

   m_startUrl = CString {path_buffer};
   m_startUrl.Replace(_T('\\'),_T('/'));
   m_startUrl = CString {_T("file:///")} + m_startUrl;
}

void CefView::CloseBrowser()
{
   if(m_browser != nullptr)
   {
      ::DestroyWindow(m_browser->GetHost()->GetWindowHandle());
      //m_browser->GetHost()->CloseBrowser(true);
   }
}
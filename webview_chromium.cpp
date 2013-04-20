/////////////////////////////////////////////////////////////////////////////
// Author: Steven Lamerton
// Copyright: (c) 2013 Steven Lamerton
// Licence: wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "webview_chromium.h"
#include <wx/webview.h>
#include <wx/filesys.h>
#include <wx/msw/private.h>

#ifdef __VISUALC__
#pragma warning(push)
#pragma warning(disable:4100)
#endif

#include <include/cef_app.h>
#include <include/cef_browser.h>

#ifdef __VISUALC__
#pragma warning(pop)
#endif

extern const char wxWebViewBackendChromium[] = "wxWebViewChromium";

wxIMPLEMENT_DYNAMIC_CLASS(wxWebViewChromium, wxWebView);

CefRefPtr<ClientHandler> g_clientHandler;

bool wxWebViewChromium::Create(wxWindow* parent,
           wxWindowID id,
           const wxString& url,
           const wxPoint& pos,
           const wxSize& size,
           long style,
           const wxString& name)
{
    if (!wxControl::Create(parent, id, pos, size, style,
                           wxDefaultValidator, name))
    {
        return false;
    }

    //m_historyLoadingFromList = false;
    //m_historyEnabled = true;
    //m_historyPosition = -1;

    CefBrowserSettings browsersettings;
    CefWindowInfo info;

#ifdef __WXMSW__
    // Initialize window info to the defaults for a child window
    info.SetAsChild(GetHWND(), wxGetClientRect(this->GetHWND()));
#endif
    // Creat the new child browser window, we do this async as we use a multi
    // threaded message loop
    CefBrowserHost::CreateBrowser(info, static_cast<CefRefPtr<CefClient>>(g_clientHandler),
                                  url.ToStdString(), browsersettings);

    this->Bind(wxEVT_SIZE, &wxWebViewChromium::OnSize, this);

    return true;
}

wxWebViewChromium::~wxWebViewChromium()
{
    CefRefPtr<CefBrowser> browser = g_clientHandler->GetBrowser();
    if(browser.get()) {
        // Let the browser window know we are about to destroy it.
        browser->GetHost()->ParentWindowWillClose();
    }
}

void wxWebViewChromium::OnSize(wxSizeEvent &WXUNUSED(event))
{
    wxSize size = GetClientSize();
    wxPoint pos = GetPosition();

#ifdef __WXMSW__
    HWND handle = g_clientHandler->GetBrowser()->GetHost()->GetWindowHandle();
    HDWP hdwp = BeginDeferWindowPos(1);
    hdwp = DeferWindowPos(hdwp, handle, 
                          NULL, pos.x, pos.y,
                          size.GetWidth(), size.GetHeight(), SWP_NOZORDER);
    EndDeferWindowPos(hdwp);
#endif
}

bool wxWebViewChromium::CanGoForward() const
{ 
    if(m_historyEnabled)
        return m_historyPosition != static_cast<int>(m_historyList.size()) - 1;
    else
        return false;
}

bool wxWebViewChromium::CanGoBack() const
{ 
    if(m_historyEnabled)
        return m_historyPosition > 0;
    else
        return false;
}

void wxWebViewChromium::LoadHistoryItem(wxSharedPtr<wxWebViewHistoryItem> item)
{
    int pos = -1;
    for(unsigned int i = 0; i < m_historyList.size(); i++)
    {
        //We compare the actual pointers to find the correct item
        if(m_historyList[i].get() == item.get())
            pos = i;
    }
    wxASSERT_MSG(pos != static_cast<int>(m_historyList.size()),
                 "invalid history item");
    m_historyLoadingFromList = true;
    LoadURL(item->GetUrl());
    m_historyPosition = pos;
}

wxVector<wxSharedPtr<wxWebViewHistoryItem> > wxWebViewChromium::GetBackwardHistory()
{
    wxVector<wxSharedPtr<wxWebViewHistoryItem> > backhist;
    //As we don't have std::copy or an iterator constructor in the wxwidgets
    //native vector we construct it by hand
    for(int i = 0; i < m_historyPosition; i++)
    {
        backhist.push_back(m_historyList[i]);
    }
    return backhist;
}

wxVector<wxSharedPtr<wxWebViewHistoryItem> > wxWebViewChromium::GetForwardHistory()
{
    wxVector<wxSharedPtr<wxWebViewHistoryItem> > forwardhist;
    //As we don't have std::copy or an iterator constructor in the wxwidgets
    //native vector we construct it by hand
    for(int i = m_historyPosition + 1; i < static_cast<int>(m_historyList.size()); i++)
    {
        forwardhist.push_back(m_historyList[i]);
    }
    return forwardhist;
}

void wxWebViewChromium::GoBack()
{
    LoadHistoryItem(m_historyList[m_historyPosition - 1]);
}

void wxWebViewChromium::GoForward()
{
    LoadHistoryItem(m_historyList[m_historyPosition + 1]);
}

void wxWebViewChromium::LoadURL(const wxString& url)
{ 
   // m_browser->GetMainFrame()->LoadURL(url.ToStdString());
}

void wxWebViewChromium::ClearHistory()
{
    m_historyList.clear();
    m_historyPosition = -1;
}

void wxWebViewChromium::EnableHistory(bool enable)
{
    m_historyEnabled = enable;
}

void wxWebViewChromium::Stop()
{
 //   m_browser->StopLoad();
}

void wxWebViewChromium::Reload(wxWebViewReloadFlags flags)
{
    if(flags == wxWEBVIEW_RELOAD_NO_CACHE)
    {
    //    m_browser->ReloadIgnoreCache();
    }
    else
    {
    //    m_browser->Reload();
    }
}

wxString wxWebViewChromium::GetPageSource() const
{
    return "";
}

wxString wxWebViewChromium::GetPageText() const 
{
    return  "";
}

wxString wxWebViewChromium::GetCurrentURL() const
{
  //  return m_browser->GetMainFrame()->GetURL().ToString();
    return "";
}

wxString wxWebViewChromium::GetCurrentTitle() const
{
    return m_title;
}

void wxWebViewChromium::Print()
{
    //m_browser->GetMainFrame()->Print();
}

void wxWebViewChromium::Cut()
{
  //  m_browser->GetMainFrame()->Cut();
}

void wxWebViewChromium::Copy()
{
 //   m_browser->GetMainFrame()->Copy();
}

void wxWebViewChromium::Paste()
{
//m_browser->GetMainFrame()->Paste();
}

void wxWebViewChromium::Undo()
{
 //   m_browser->GetMainFrame()->Undo();
}

void wxWebViewChromium::Redo()
{
   // m_browser->GetMainFrame()->Redo();
}

void wxWebViewChromium::SelectAll()
{
   // m_browser->GetMainFrame()->SelectAll();
}

void wxWebViewChromium::DeleteSelection()
{
    wxString jsdelete = "if (window.getSelection) { if (window.getSelection().deleteFromDocument) { window.getSelection().deleteFromDocument(); } }";
    RunScript(jsdelete);
}

void wxWebViewChromium::ClearSelection()
{
    wxString jsclear = "if (window.getSelection) { if (window.getSelection().empty) { window.getSelection().empty(); } }";
    RunScript(jsclear);
}

void wxWebViewChromium::RunScript(const wxString& javascript)
{
   // m_browser->GetMainFrame()->ExecuteJavaScript(javascript.ToStdString(), "", 0);
}

bool wxWebViewChromium::IsBusy() const
{
    return false;
}

void wxWebViewChromium::SetEditable(bool enable)
{
    wxString mode = enable ? "\"on\"" : "\"off\"";
    RunScript("document.designMode = " + mode); 
}

void wxWebViewChromium::DoSetPage(const wxString& html, const wxString& baseUrl)
{
  //  m_browser->GetMainFrame()->LoadString(html.ToStdString(), baseUrl.ToStdString());
}

wxWebViewZoom wxWebViewChromium::GetZoom() const
{
    float zoom;
 //   float zoom = m_browser->GetHost()->GetZoomLevel();
    // arbitrary way to map float zoom to our common zoom enum
    if (zoom <= -0.75f)
    {
        return wxWEBVIEW_ZOOM_TINY;
    }
    else if (zoom > -0.75 && zoom <= -0.25)
    {
        return wxWEBVIEW_ZOOM_SMALL;
    }
    else if (zoom > -0.25 && zoom <= 0.25)
    {
        return wxWEBVIEW_ZOOM_MEDIUM;
    }
    else if (zoom > 0.25 && zoom <= 0.75)
    {
        return wxWEBVIEW_ZOOM_LARGE;
    }
    else if (zoom > 0.75)
    {
        return wxWEBVIEW_ZOOM_LARGEST;
    }

    // to shut up compilers, this can never be reached logically
    wxASSERT(false);
    return wxWEBVIEW_ZOOM_MEDIUM;
}


void wxWebViewChromium::SetZoom(wxWebViewZoom zoom)
{
    double mapzoom;
    // arbitrary way to map our common zoom enum to float zoom
    switch (zoom)
    {
        case wxWEBVIEW_ZOOM_TINY:
            mapzoom = -1.0;
            break;

        case wxWEBVIEW_ZOOM_SMALL:
            mapzoom = -0.5;
            break;

        case wxWEBVIEW_ZOOM_MEDIUM:
            mapzoom = 0.0;
            break;

        case wxWEBVIEW_ZOOM_LARGE:
            mapzoom = 0.5;
            break;

        case wxWEBVIEW_ZOOM_LARGEST:
            mapzoom = 1.0;
            break;

        default:
            wxASSERT(false);
    }
   // m_browser->GetHost()->SetZoomLevel(mapzoom);
}

void wxWebViewChromium::SetZoomType(wxWebViewZoomType type)
{
    // there is only one supported zoom type at the moment so this setter
    // does nothing beyond checking sanity
    wxASSERT(type == wxWEBVIEW_ZOOM_TYPE_LAYOUT);
}

wxWebViewZoomType wxWebViewChromium::GetZoomType() const
{
    return wxWEBVIEW_ZOOM_TYPE_LAYOUT;
}

bool wxWebViewChromium::CanSetZoomType(wxWebViewZoomType type) const
{
    return type == wxWEBVIEW_ZOOM_TYPE_LAYOUT;
}

void wxWebViewChromium::RegisterHandler(wxSharedPtr<wxWebViewHandler> handler)
{
    // We currently don't support custom scheme handlers
}

bool wxWebViewChromium::StartUp()
{
    if(!g_clientHandler)
        g_clientHandler = new ClientHandler;

    CefMainArgs args(wxGetInstance()); 
    if(CefExecuteProcess(args, NULL) >= 0)
        return false;

    CefSettings settings;
    // We use a multithreaded message loop so we don't have to integrate
    // with the wx message loop
    settings.multi_threaded_message_loop = true;

    return CefInitialize(args, settings, NULL);
}

void wxWebViewChromium::Shutdown()
{
    CefShutdown();
}

// CefLifeSpanHandler methods
bool ClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             const CefString& target_url,
                             const CefString& target_frame_name,
                             const CefPopupFeatures& popupFeatures,
                             CefWindowInfo& windowInfo,
                             CefRefPtr<CefClient>& client,
                             CefBrowserSettings& settings,
                             bool* no_javascript_access)
{
    return false;
}

void ClientHandler::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
    if(!m_browser.get())
    {
        m_browser = browser;
        m_browserId = browser->GetIdentifier();
    }
}
bool ClientHandler::DoClose(CefRefPtr<CefBrowser> browser)
{
    return false;
}

void ClientHandler::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    if(browser->GetIdentifier() == m_browserId)
    {
        m_browser = NULL;
    }
}
/////////////////////////////////////////////////////////////////////////////
// Author: Steven Lamerton
// Copyright: (c) 2013 Steven Lamerton
// Licence: wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include "webview_chromium.h"
#include <wx/webview.h>
#include <wx/filesys.h>

#ifdef __VISUALC__
#pragma warning(push)
#pragma warning(disable:4100)
#endif

#include <include/cef_version.h>

#if CEF_REVISION < 607
#include <include/cef.h>
#else
#include <include/cef_app.h>
#include <include/cef_browser.h>
#include <include/cef_client.h>
#include <include/cef_scheme.h>
#endif

#ifdef __VISUALC__
#pragma warning(pop)
#endif

extern const char wxWebViewBackendChromium[] = "wxWebViewChromium";

// ClientHandler implementation.
class ClientHandler : public CefClient,
                      public CefDisplayHandler,
                      public CefDragHandler,
                      public CefFindHandler,
                      public CefFocusHandler,
                      public CefJSDialogHandler,
                      public CefKeyboardHandler,
                      public CefLifeSpanHandler,
                      public CefLoadHandler,
                      public CefMenuHandler,
                      public CefPrintHandler,
                      public CefRequestHandler,
                      public CefV8ContextHandler

{
public:
    ClientHandler(wxWebViewChromium* webview) : m_webview(webview), 
                                                m_busyCount(0), 
                                                m_cancelLoad(false) {};
    virtual ~ClientHandler() {};

    //CefClient methods
    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler() { return this; }
    virtual CefRefPtr<CefDragHandler> GetDragHandler() { return this; }
    virtual CefRefPtr<CefFindHandler> GetFindHandler() { return this; }
    virtual CefRefPtr<CefFocusHandler> GetFocusHandler() { return this; }
    virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler() { return this; }
    virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler() { return this; }
    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() { return this; }
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() { return this; }
    virtual CefRefPtr<CefMenuHandler> GetMenuHandler() { return this; }
    virtual CefRefPtr<CefPrintHandler> GetPrintHandler() { return this; }
    virtual CefRefPtr<CefRequestHandler> GetRequestHandler() { return this; }
    virtual CefRefPtr<CefV8ContextHandler> GetV8ContextHandler() { return this; }

    //CefDisplayHandler methods
    virtual void OnAddressChange(CefRefPtr<CefBrowser>,
                                 CefRefPtr<CefFrame>,
                                 const CefString&) {}
    virtual bool OnConsoleMessage(CefRefPtr<CefBrowser>,
                                  const CefString&,
                                  const CefString&,
                                  int) { return false; }
    virtual void OnContentsSizeChange(CefRefPtr<CefBrowser>,
                                      CefRefPtr<CefFrame>, int, 
                                      int) {}
    virtual void OnNavStateChange(CefRefPtr<CefBrowser>,
                                  bool, bool) {}
    virtual void OnStatusMessage(CefRefPtr<CefBrowser>, 
                                 const CefString&, 
                                 CefDisplayHandler::StatusType) {}
    virtual void OnTitleChange(CefRefPtr<CefBrowser>, 
                               const CefString&);
    virtual bool OnTooltip(CefRefPtr<CefBrowser>, CefString&)
                          { return false; }

    //CefDragHandler methods
    virtual bool OnDragStart(CefRefPtr<CefBrowser>,
                             CefRefPtr<CefDragData>,
                             DragOperationsMask) { return false; }
    virtual bool OnDragEnter(CefRefPtr<CefBrowser>,
                             CefRefPtr<CefDragData>,
                             DragOperationsMask) { return false; }

    //CefFindHandler methods
    virtual void OnFindResult(CefRefPtr<CefBrowser>, int,
                              int, const CefRect&, 
                              int, bool) {}

    //CefFocusHandler methods
    virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser>,
                                      CefRefPtr<CefFrame>,
                                      CefRefPtr<CefDOMNode>) {}
    virtual bool OnSetFocus(CefRefPtr<CefBrowser>, 
                            CefFocusHandler::FocusSource) { return false; }
    virtual void OnTakeFocus(CefRefPtr<CefBrowser>, bool) {}

    //CefJSDialogandler methods
    virtual bool OnJSAlert(CefRefPtr<CefBrowser>, 
                           CefRefPtr<CefFrame>, 
                           const CefString&) { return false; }
    virtual bool OnJSConfirm(CefRefPtr<CefBrowser>, 
                             CefRefPtr<CefFrame>, 
                             const CefString&, bool&)
                             { return false; }
    virtual bool OnJSPrompt(CefRefPtr<CefBrowser>,
                            CefRefPtr<CefFrame>, 
                            const CefString&, 
                            const CefString&, bool&, 
                            CefString&) { return false; }

    //CefKeyboardHandler methods
    virtual bool OnKeyEvent(CefRefPtr<CefBrowser>,
                            KeyEventType, int, int, 
                            bool, bool) 
                            { return false; }

    //CefLifeSpanHandler methods
    virtual bool OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser,
                               const CefPopupFeatures& popupFeatures,
                               CefWindowInfo& windowInfo,
                               const CefString& url,
                               CefRefPtr<CefClient>& client,
                               CefBrowserSettings& settings);
    virtual void OnAfterCreated(CefRefPtr<CefBrowser>)  {}
    virtual bool DoClose(CefRefPtr<CefBrowser>)   { return false; };
    virtual void OnBeforeClose(CefRefPtr<CefBrowser>)  {}

    //CefLoadHandler methods
    virtual void OnLoadStart(CefRefPtr<CefBrowser> browser, 
                             CefRefPtr<CefFrame> frame);
    virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser, 
                           CefRefPtr<CefFrame> frame,
                           int httpStatusCode);
    virtual bool OnLoadError(CefRefPtr<CefBrowser> browser, 
                             CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                             const CefString& failedUrl, CefString& errorText);
 
    //CefMenuHandler methods
    virtual void GetMenuLabel(CefRefPtr<CefBrowser>, 
                              CefMenuHandler::MenuId, CefString&)
                              {}
    virtual bool OnBeforeMenu(CefRefPtr< CefBrowser >, 
                              const CefMenuInfo&) { return false; }
    virtual bool OnMenuAction(CefRefPtr< CefBrowser >, 
                              CefMenuHandler::MenuId) { return false; }

    //CefPrintHandler methods.
    virtual bool GetPrintHeaderFooter(CefRefPtr<CefBrowser>,
                                      CefRefPtr<CefFrame>,
                                      const CefPrintInfo&,
                                      const CefString&,
                                      const CefString&, int,
                                      int, CefString&,
                                      CefString&, CefString&,
                                      CefString&, 
                                      CefString&,
                                      CefString&) { return false; }

    virtual bool GetPrintOptions(CefRefPtr<CefBrowser>, 
                                 CefPrintOptions&)
                                 { return false; }

    //CefRequestHandler methods
    virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser>, 
                                CefRefPtr<CefFrame>, 
                                CefRefPtr<CefRequest>, 
                                CefRequestHandler::NavType, 
                                bool);

    //CefV8ContextHandler methods
    virtual void OnContextCreated(CefRefPtr<CefBrowser>,
                                  CefRefPtr<CefFrame>,
                                  CefRefPtr<CefV8Context>) {}

    virtual void OnContextReleased(CefRefPtr<CefBrowser>,
                                   CefRefPtr<CefFrame>,
                                   CefRefPtr<CefV8Context>) {}

    bool IsBusy() { return m_busyCount != 0; }

    IMPLEMENT_REFCOUNTING(ClientHandler);

private:      
    wxWebViewChromium* m_webview;
    long m_busyCount;
    //Cancel the next load as the new windw has been vetoed
    bool m_cancelLoad;
};

class CustomSchemeHandler : public CefSchemeHandler
{
public:
    CustomSchemeHandler(wxSharedPtr<wxWebViewHandler> handler) : m_handler(handler) {}

    virtual bool ProcessRequest(CefRefPtr<CefRequest> request,
                                CefRefPtr<CefSchemeHandlerCallback> callback);
    virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
                                    int64& response_length,
                                    CefString& redirectUrl);
    virtual void Cancel() {};
    virtual bool ReadResponse(void* data_out, int bytes_to_read,
                              int& bytes_read,
                              CefRefPtr<CefSchemeHandlerCallback> callback);
private:
    wxSharedPtr<wxWebViewHandler> m_handler;
    wxFSFile* m_file;
    IMPLEMENT_REFCOUNTING(CustomSchemeHandler);
};

class CustomSchemeHandlerFactory : public CefSchemeHandlerFactory
{
public:
    CustomSchemeHandlerFactory(wxSharedPtr<wxWebViewHandler> handler) : m_handler(handler) {}

    virtual CefRefPtr<CefSchemeHandler> Create(CefRefPtr<CefBrowser> WXUNUSED(browser),
                                               const CefString& WXUNUSED(scheme_name),
                                               CefRefPtr<CefRequest> WXUNUSED(request))
    {
        return new CustomSchemeHandler(m_handler);
    }

private:
    wxSharedPtr<wxWebViewHandler> m_handler;
    IMPLEMENT_REFCOUNTING(ClientSchemeHandlerFactory);
};

wxIMPLEMENT_DYNAMIC_CLASS(wxWebViewChromium, wxWebView);

wxBEGIN_EVENT_TABLE(wxWebViewChromium, wxWebView)
    EVT_TIMER(wxID_ANY, wxWebViewChromium::OnTimer)
    EVT_SIZE(wxWebViewChromium::OnSize)
wxEND_EVENT_TABLE()

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

    m_historyLoadingFromList = false;
    m_historyEnabled = true;
    m_historyPosition = -1;

    CefWindowInfo info;
    CefBrowserSettings browsersettings;
    CefSettings settings;

    CefInitialize(settings, CefRefPtr<CefApp>());

    wxSize wxsize = GetSize();
    wxPoint wxpos = GetPosition();
#ifdef __WXMSW__
    RECT rect;
    rect.top = wxpos.x;
    rect.bottom = wxsize.GetHeight();
    rect.left = wxpos.y;
    rect.right = wxsize.GetWidth();

    // Initialize window info to the defaults for a child window
    info.SetAsChild(GetHWND(), rect);
#endif
    // Creat the new child browser window
    m_browser = CefBrowser::CreateBrowserSync(info, new ClientHandler(this), url.ToStdString(), browsersettings);
    
    this->Bind(wxEVT_TIMER, &wxWebViewChromium::OnTimer, this);

    m_timer = new wxTimer(this);
    m_timer->Start(100);
    
    return true;
}

wxWebViewChromium::~wxWebViewChromium()
{
    m_timer->Stop();
    wxDELETE(m_timer);
    m_browser->ParentWindowWillClose();
}

void wxWebViewChromium::OnTimer(wxTimerEvent &event)
{
    CefDoMessageLoopWork();
}

void wxWebViewChromium::OnSize(wxSizeEvent &WXUNUSED(event))
{
    wxSize size = GetSize();
    wxPoint pos = GetPosition();

#ifdef __WXMSW__
    RECT rect;
    rect.top = pos.x;
    rect.bottom = size.GetHeight();
    rect.left = pos.y;
    rect.right = size.GetWidth();

    HDWP hdwp = BeginDeferWindowPos(1);
    hdwp = DeferWindowPos(hdwp, m_browser->GetWindowHandle(), NULL, rect.left, 
                          rect.top, rect.right - rect.left, 
                          rect.bottom - rect.top, SWP_NOZORDER);
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
    m_browser->GetMainFrame()->LoadURL(url.ToStdString());
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
    m_browser->StopLoad();
}

void wxWebViewChromium::Reload(wxWebViewReloadFlags flags)
{
    if(flags == wxWEBVIEW_RELOAD_NO_CACHE)
    {
        m_browser->ReloadIgnoreCache();
    }
    else
    {
        m_browser->Reload();
    }
}

wxString wxWebViewChromium::GetPageSource() const
{
    return m_browser->GetMainFrame()->GetSource().ToString();
}

wxString wxWebViewChromium::GetPageText() const 
{
    return m_browser->GetMainFrame()->GetText().ToString();
}

wxString wxWebViewChromium::GetCurrentURL() const
{
    return m_browser->GetMainFrame()->GetURL().ToString();
}

wxString wxWebViewChromium::GetCurrentTitle() const
{
    return m_title;
}

void wxWebViewChromium::Print()
{
    m_browser->GetMainFrame()->Print();
}

void wxWebViewChromium::Cut()
{
    m_browser->GetMainFrame()->Cut();
}

void wxWebViewChromium::Copy()
{
    m_browser->GetMainFrame()->Copy();
}

void wxWebViewChromium::Paste()
{
    m_browser->GetMainFrame()->Paste();
}

void wxWebViewChromium::Undo()
{
    m_browser->GetMainFrame()->Undo();
}

void wxWebViewChromium::Redo()
{
    m_browser->GetMainFrame()->Redo();
}

void wxWebViewChromium::SelectAll()
{
    m_browser->GetMainFrame()->SelectAll();
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
    m_browser->GetMainFrame()->ExecuteJavaScript(javascript.ToStdString(), "", 0);
}

bool wxWebViewChromium::IsBusy() const
{
    return static_cast<ClientHandler*>(m_browser->GetClient().get())->IsBusy();
}

void wxWebViewChromium::SetEditable(bool enable)
{
    wxString mode = enable ? "\"on\"" : "\"off\"";
    RunScript("document.designMode = " + mode); 
}

void wxWebViewChromium::DoSetPage(const wxString& html, const wxString& baseUrl)
{
    m_browser->GetMainFrame()->LoadString(html.ToStdString(), baseUrl.ToStdString());
}

wxWebViewZoom wxWebViewChromium::GetZoom() const
{
    float zoom = m_browser->GetZoomLevel();
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
    // arbitrary way to map our common zoom enum to float zoom
    switch (zoom)
    {
        case wxWEBVIEW_ZOOM_TINY:
            m_browser->SetZoomLevel(-1.0f);
            break;

        case wxWEBVIEW_ZOOM_SMALL:
            m_browser->SetZoomLevel(-0.5f);
            break;

        case wxWEBVIEW_ZOOM_MEDIUM:
            m_browser->SetZoomLevel(0.0f);
            break;

        case wxWEBVIEW_ZOOM_LARGE:
            m_browser->SetZoomLevel(0.5f);
            break;

        case wxWEBVIEW_ZOOM_LARGEST:
            m_browser->SetZoomLevel(1.0f);
            break;

        default:
            wxASSERT(false);
    }
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
    switch (type)
    {
        case wxWEBVIEW_ZOOM_TYPE_LAYOUT:
            return true;

        default:
            return false;
    }
}

void wxWebViewChromium::RegisterHandler(wxSharedPtr<wxWebViewHandler> handler)
{
    CefRegisterCustomScheme(handler->GetName().ToStdString(), true, false, false);
    CefRegisterSchemeHandlerFactory(handler->GetName().ToStdString(), "", 
                                    new CustomSchemeHandlerFactory(handler));
}

void wxWebViewChromium::Shutdown()
{
    CefShutdown();
}

void ClientHandler::OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
    //Check we still exist
    if(!m_webview || m_webview->IsBeingDeleted())
        return;

    m_webview->m_title = title.ToString();
    wxString target = browser->GetMainFrame()->GetName().ToString();

    wxWebViewEvent event(wxEVT_COMMAND_WEBVIEW_TITLE_CHANGED, m_webview->GetId(), "", target);
    event.SetString(title.ToString());
    event.SetEventObject(m_webview);

    m_webview->HandleWindowEvent(event);
}

bool ClientHandler::OnBeforeBrowse(CefRefPtr<CefBrowser> browser, 
                                   CefRefPtr<CefFrame> frame, 
                                   CefRefPtr<CefRequest> request, 
                                   CefRequestHandler::NavType WXUNUSED(navType), 
                                   bool WXUNUSED(isRedirect))
{
    //Check we still exist
    if(!m_webview || m_webview->IsBeingDeleted())
        return true;
    
    //If the new window event has been veted we shouldn't load the page
    //This is cancelled here because it cannot be cancelled from OnBeforePopup
    if(m_cancelLoad)
    {
        m_cancelLoad = false;
        return true;
    }

    wxString url = request->GetURL().ToString();
    wxString target = frame->GetName().ToString();

    m_busyCount++;

    wxWebViewEvent event(wxEVT_COMMAND_WEBVIEW_NAVIGATING, m_webview->GetId(), url, target);
    event.SetEventObject(m_webview);

    m_webview->HandleWindowEvent(event);

    if (!event.IsAllowed())
    {
        m_busyCount--;
        return true;
    }
    return false;
}

void ClientHandler::OnLoadStart(CefRefPtr<CefBrowser> WXUNUSED(browser), CefRefPtr<CefFrame> WXUNUSED(frame))
{
    //We actually use OnBeforeBrowse as then we can veto
}

void ClientHandler::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int WXUNUSED(httpStatusCode))
{
    //Check we still exist
    if(!m_webview || m_webview->IsBeingDeleted())
        return;

    m_busyCount--;

    wxString url = frame->GetURL().ToString();
    wxString target = frame->GetName().ToString();

    wxWebViewEvent event(wxEVT_COMMAND_WEBVIEW_NAVIGATED, m_webview->GetId(), url, target);
    event.SetEventObject(m_webview);

    m_webview->HandleWindowEvent(event);

    if(frame->IsMain())
    {
        //As we are complete we also add to the history list, but not if the
        //page is not the main page, ie it is a subframe
        if(m_webview->m_historyEnabled && !m_webview->m_historyLoadingFromList)
        {
            //If we are not at the end of the list, then erase everything
            //between us and the end before adding the new page
            if(m_webview->m_historyPosition != static_cast<int>(m_webview->m_historyList.size()) - 1)
            {
                m_webview->m_historyList.erase(m_webview->m_historyList.begin() + m_webview->m_historyPosition + 1,
                                               m_webview->m_historyList.end());
            }
            wxSharedPtr<wxWebViewHistoryItem> item(new wxWebViewHistoryItem(url, m_webview->GetCurrentTitle()));
            m_webview->m_historyList.push_back(item);
            m_webview->m_historyPosition++;
        }
        //Reset as we are done now
        m_webview->m_historyLoadingFromList = false;

        wxWebViewEvent levent(wxEVT_COMMAND_WEBVIEW_LOADED, m_webview->GetId(), url, target);
        levent.SetEventObject(m_webview);

        m_webview->HandleWindowEvent(levent);
    }
}

bool ClientHandler::OnLoadError(CefRefPtr<CefBrowser> WXUNUSED(browser), CefRefPtr<CefFrame> frame, ErrorCode errorCode,
                                const CefString& failedUrl, CefString& errorText)
{ 
    //Check we still exist
    if(!m_webview || m_webview->IsBeingDeleted())
        return false;

    //We define a macro for convenience
#define ERROR_TYPE_CASE(error, wxtype) case(error): \
                                           type = wxtype;\
                                           break

    wxWebViewNavigationError type = wxWEBVIEW_NAV_ERR_OTHER;
    switch (errorCode)
    {
        ERROR_TYPE_CASE(ERR_FAILED, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_ABORTED, wxWEBVIEW_NAV_ERR_USER_CANCELLED);
        ERROR_TYPE_CASE(ERR_INVALID_ARGUMENT, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_INVALID_HANDLE, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_FILE_NOT_FOUND, wxWEBVIEW_NAV_ERR_NOT_FOUND);
        ERROR_TYPE_CASE(ERR_TIMED_OUT, wxWEBVIEW_NAV_ERR_CONNECTION);
        ERROR_TYPE_CASE(ERR_FILE_TOO_BIG, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_UNEXPECTED, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_ACCESS_DENIED, wxWEBVIEW_NAV_ERR_AUTH);
        ERROR_TYPE_CASE(ERR_NOT_IMPLEMENTED, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_CONNECTION_CLOSED, wxWEBVIEW_NAV_ERR_CONNECTION);
        ERROR_TYPE_CASE(ERR_CONNECTION_RESET, wxWEBVIEW_NAV_ERR_CONNECTION);
        ERROR_TYPE_CASE(ERR_CONNECTION_REFUSED, wxWEBVIEW_NAV_ERR_CONNECTION);
        ERROR_TYPE_CASE(ERR_CONNECTION_ABORTED, wxWEBVIEW_NAV_ERR_CONNECTION);
        ERROR_TYPE_CASE(ERR_CONNECTION_FAILED, wxWEBVIEW_NAV_ERR_CONNECTION);
        ERROR_TYPE_CASE(ERR_NAME_NOT_RESOLVED, wxWEBVIEW_NAV_ERR_CONNECTION);
        ERROR_TYPE_CASE(ERR_INTERNET_DISCONNECTED, wxWEBVIEW_NAV_ERR_CONNECTION);
        ERROR_TYPE_CASE(ERR_SSL_PROTOCOL_ERROR, wxWEBVIEW_NAV_ERR_SECURITY);
        ERROR_TYPE_CASE(ERR_ADDRESS_INVALID, wxWEBVIEW_NAV_ERR_REQUEST);
        ERROR_TYPE_CASE(ERR_ADDRESS_UNREACHABLE, wxWEBVIEW_NAV_ERR_CONNECTION);
        ERROR_TYPE_CASE(ERR_SSL_CLIENT_AUTH_CERT_NEEDED, wxWEBVIEW_NAV_ERR_AUTH);
        ERROR_TYPE_CASE(ERR_TUNNEL_CONNECTION_FAILED, wxWEBVIEW_NAV_ERR_CONNECTION);
        ERROR_TYPE_CASE(ERR_NO_SSL_VERSIONS_ENABLED, wxWEBVIEW_NAV_ERR_SECURITY);
        ERROR_TYPE_CASE(ERR_SSL_VERSION_OR_CIPHER_MISMATCH, wxWEBVIEW_NAV_ERR_SECURITY);
        ERROR_TYPE_CASE(ERR_SSL_RENEGOTIATION_REQUESTED, wxWEBVIEW_NAV_ERR_REQUEST);
        ERROR_TYPE_CASE(ERR_CERT_COMMON_NAME_INVALID, wxWEBVIEW_NAV_ERR_CERTIFICATE);
        ERROR_TYPE_CASE(ERR_CERT_DATE_INVALID, wxWEBVIEW_NAV_ERR_CERTIFICATE);
        ERROR_TYPE_CASE(ERR_CERT_AUTHORITY_INVALID, wxWEBVIEW_NAV_ERR_CERTIFICATE);
        ERROR_TYPE_CASE(ERR_CERT_CONTAINS_ERRORS, wxWEBVIEW_NAV_ERR_CERTIFICATE);
        ERROR_TYPE_CASE(ERR_CERT_NO_REVOCATION_MECHANISM, wxWEBVIEW_NAV_ERR_CERTIFICATE);
        ERROR_TYPE_CASE(ERR_CERT_UNABLE_TO_CHECK_REVOCATION, wxWEBVIEW_NAV_ERR_CERTIFICATE);
        ERROR_TYPE_CASE(ERR_CERT_REVOKED, wxWEBVIEW_NAV_ERR_CERTIFICATE);
        ERROR_TYPE_CASE(ERR_CERT_INVALID, wxWEBVIEW_NAV_ERR_CERTIFICATE);
        ERROR_TYPE_CASE(ERR_CERT_END, wxWEBVIEW_NAV_ERR_CERTIFICATE);
        ERROR_TYPE_CASE(ERR_INVALID_URL, wxWEBVIEW_NAV_ERR_REQUEST);
        ERROR_TYPE_CASE(ERR_DISALLOWED_URL_SCHEME, wxWEBVIEW_NAV_ERR_REQUEST);
        ERROR_TYPE_CASE(ERR_UNKNOWN_URL_SCHEME, wxWEBVIEW_NAV_ERR_REQUEST);
        ERROR_TYPE_CASE(ERR_TOO_MANY_REDIRECTS, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_UNSAFE_REDIRECT, wxWEBVIEW_NAV_ERR_SECURITY);
        ERROR_TYPE_CASE(ERR_UNSAFE_PORT, wxWEBVIEW_NAV_ERR_SECURITY);
        ERROR_TYPE_CASE(ERR_INVALID_RESPONSE, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_INVALID_CHUNKED_ENCODING, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_METHOD_NOT_SUPPORTED, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_UNEXPECTED_PROXY_AUTH, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_EMPTY_RESPONSE, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_RESPONSE_HEADERS_TOO_BIG, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_CACHE_MISS, wxWEBVIEW_NAV_ERR_OTHER);
        ERROR_TYPE_CASE(ERR_INSECURE_RESPONSE, wxWEBVIEW_NAV_ERR_SECURITY);
    }

    wxString url = failedUrl.ToString();
    wxString target = frame->GetName().ToString();
    wxWebViewEvent event(wxEVT_COMMAND_WEBVIEW_ERROR, m_webview->GetId(), url, target);
    event.SetEventObject(m_webview);
    event.SetInt(type);
    event.SetString(errorText.ToString());
     
    m_webview->HandleWindowEvent(event);

    return false;
}

bool ClientHandler::OnBeforePopup(CefRefPtr<CefBrowser> WXUNUSED(parentBrowser),
                                  const CefPopupFeatures& WXUNUSED(popupFeatures),
                                  CefWindowInfo& WXUNUSED(windowInfo),
                                  const CefString& url,
                                  CefRefPtr<CefClient>& WXUNUSED(client),
                                  CefBrowserSettings& WXUNUSED(settings))
{
    //Check we still exist
    if(!m_webview || m_webview->IsBeingDeleted())
        return true;

    wxWebViewEvent event(wxEVT_COMMAND_WEBVIEW_NEWWINDOW, m_webview->GetId(), url.ToString(), "");
    event.SetEventObject(m_webview);
    m_webview->HandleWindowEvent(event);

    if(!event.IsAllowed())
    {
        m_cancelLoad = true;
    }

    //We return true because we do not want a popup to be shown
    return true;
}

bool CustomSchemeHandler::ProcessRequest(CefRefPtr<CefRequest> request,
                                         CefRefPtr<CefSchemeHandlerCallback> callback)
{
    wxString url = request->GetURL().ToString();

    m_file = m_handler->GetFile(url);

    if(m_file)
    {
        callback->HeadersAvailable();
        return true;
    }
    return false;
}

void CustomSchemeHandler::GetResponseHeaders(CefRefPtr<CefResponse> response,
                                             int64& response_length,
                                             CefString& WXUNUSED(redirectUrl))
{
    response->SetMimeType(m_file->GetMimeType().ToStdString());
    response->SetStatus(200);

    response_length = m_file->GetStream()->GetLength();
}

bool CustomSchemeHandler::ReadResponse(void* data_out, int bytes_to_read,
                                       int& bytes_read,
                                       CefRefPtr<CefSchemeHandlerCallback> WXUNUSED(callback))
{
    /*wxStreamError err = */m_file->GetStream()->Read(data_out, bytes_to_read).GetLastError();
    bytes_read = m_file->GetStream()->LastRead();

    return true;
}

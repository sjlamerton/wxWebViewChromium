/////////////////////////////////////////////////////////////////////////////
// Author: Steven Lamerton
// Copyright: (c) 2013 Steven Lamerton
// Licence: wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_WEBVIEWCHROMIUM_H_
#define _WX_WEBVIEWCHROMIUM_H_

#ifdef __VISUALC__
#pragma warning(push)
#pragma warning(disable:4100)
#endif

#include <include/cef_version.h>

#if CEF_REVISION < 607
#include <include/cef.h>
#else
#include <include/cef_browser.h>
#endif

#ifdef __VISUALC__
#pragma warning(pop)
#endif

#include <wx/control.h>
#include <wx/webview.h>
#include <wx/sharedptr.h>
#include <wx/vector.h>
#include <wx/webview.h>
#include <wx/timer.h>

extern const char wxWebViewBackendChromium[];

class WXDLLIMPEXP_WEBVIEW wxWebViewChromium : public wxWebView
{
public:

    wxWebViewChromium() {}

    wxWebViewChromium(wxWindow* parent,
           wxWindowID id,
           const wxString& url = wxWebViewDefaultURLStr,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = 0,
           const wxString& name = wxWebViewNameStr)
   {
       Create(parent, id, url, pos, size, style, name);
   }

    ~wxWebViewChromium() {
        m_timer->Stop();
        wxDELETE(m_timer);
        m_browser->ParentWindowWillClose();
    }

    void OnTimer(wxTimerEvent &event);
    void OnSize(wxSizeEvent &event);

    bool Create(wxWindow* parent,
           wxWindowID id,
           const wxString& url = wxWebViewDefaultURLStr,
           const wxPoint& pos = wxDefaultPosition,
           const wxSize& size = wxDefaultSize,
           long style = 0,
           const wxString& name = wxWebViewNameStr);

    virtual void LoadURL(const wxString& url);
    virtual void LoadHistoryItem(wxSharedPtr<wxWebViewHistoryItem> item);
    virtual wxVector<wxSharedPtr<wxWebViewHistoryItem> > GetBackwardHistory();
    virtual wxVector<wxSharedPtr<wxWebViewHistoryItem> > GetForwardHistory();

    virtual bool CanGoForward() const;
    virtual bool CanGoBack() const;
    virtual void GoBack();
    virtual void GoForward();
    virtual void ClearHistory();
    virtual void EnableHistory(bool enable = true);
    virtual void Stop();
    virtual void Reload(wxWebViewReloadFlags flags = wxWEBVIEW_RELOAD_DEFAULT);

    virtual wxString GetPageSource() const;
    virtual wxString GetPageText() const;

    virtual bool IsBusy() const;
    virtual wxString GetCurrentURL() const;
    virtual wxString GetCurrentTitle() const;

    virtual void SetZoomType(wxWebViewZoomType type);
    virtual wxWebViewZoomType GetZoomType() const;
    virtual bool CanSetZoomType(wxWebViewZoomType type) const;

    virtual void Print();

    virtual wxWebViewZoom GetZoom() const;
    virtual void SetZoom(wxWebViewZoom zoom);

    virtual void* GetNativeBackend() const { return m_browser; }

    virtual long Find(const wxString& text, int flags = wxWEBVIEW_FIND_DEFAULT) { return wxNOT_FOUND; }

    //Clipboard functions
    virtual bool CanCut() const { return true; }
    virtual bool CanCopy() const { return true; }
    virtual bool CanPaste() const { return true; }
    virtual void Cut();
    virtual void Copy();
    virtual void Paste();

    //Undo / redo functionality
    virtual bool CanUndo() const { return true; }
    virtual bool CanRedo() const { return true; }
    virtual void Undo();
    virtual void Redo();

    //Editing functions
    virtual void SetEditable(bool enable = true);
    virtual bool IsEditable() const { return false; }

    //Selection
    virtual void SelectAll();
    virtual bool HasSelection() const { return false; }
    virtual void DeleteSelection();
    virtual wxString GetSelectedText() const { return ""; }
    virtual wxString GetSelectedSource() const { return ""; }
    virtual void ClearSelection();

    virtual void RunScript(const wxString& javascript);

    //Virtual Filesystem Support
    virtual void RegisterHandler(wxSharedPtr<wxWebViewHandler> handler);

    static void Shutdown();

protected:
    virtual void DoSetPage(const wxString& html, const wxString& baseUrl);

private:
    CefRefPtr<CefBrowser> m_browser;

    //History related variables, we currently use our own implementation
    wxVector<wxSharedPtr<wxWebViewHistoryItem> > m_historyList;
    int m_historyPosition;
    bool m_historyLoadingFromList;
    bool m_historyEnabled;

    //We need to store the title ourselves
    wxString m_title;

    //We also friend ClientHandler so it can access the history
    friend class ClientHandler;

    //Our timer handles the CEF event loop
    wxTimer *m_timer;
  
    wxDECLARE_DYNAMIC_CLASS(wxWebViewChromium);
    wxDECLARE_EVENT_TABLE();
};

class WXDLLIMPEXP_WEBVIEW wxWebViewFactoryChromium : public wxWebViewFactory
{
public:
    virtual wxWebView* Create() { return new wxWebViewChromium; }
    virtual wxWebView* Create(wxWindow* parent,
                              wxWindowID id,
                              const wxString& url = wxWebViewDefaultURLStr,
                              const wxPoint& pos = wxDefaultPosition,
                              const wxSize& size = wxDefaultSize,
                              long style = 0,
                              const wxString& name = wxWebViewNameStr)
    { return new wxWebViewChromium(parent, id, url, pos, size, style, name); }
};

#endif // _WX_WEBVIEWCHROMIUM_H_

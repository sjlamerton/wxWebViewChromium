/////////////////////////////////////////////////////////////////////////////
// Author: Steven Lamerton
// Copyright: (c) 2013 Steven Lamerton
// Licence: wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/sharedptr.h>
#include <wx/webview.h>

#include "simple.h"
#include "../webview_chromium.h"

bool SimpleApp::OnInit()
{
    if(!wxWebViewChromium::StartUp())
        return false;

    SimpleFrame *frame = new SimpleFrame();
    frame->Show(true);

    return true;
}

int SimpleApp::OnExit()
{
    wxWebViewChromium::Shutdown();
    return wxApp::OnExit();
}

SimpleFrame::SimpleFrame() : wxFrame(NULL, wxID_ANY, "wxWebViewChromium")
{
    wxWebView::RegisterFactory(wxWebViewBackendChromium, wxSharedPtr<wxWebViewFactory>
                                                                     (new wxWebViewFactoryChromium));
    wxWebView* webview = wxWebView::New(this, wxID_ANY, "http://www.bbc.co.uk/",
                                        wxDefaultPosition, wxDefaultSize, wxWebViewBackendChromium);
}

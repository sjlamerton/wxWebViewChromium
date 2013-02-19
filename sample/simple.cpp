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
    timer = new wxTimer(this);
    this->Bind(wxEVT_TIMER, &SimpleFrame::OnTimer, this);

    wxWebView::RegisterFactory(wxWebViewBackendChromium, wxSharedPtr<wxWebViewFactory>
                                                                     (new wxWebViewFactoryChromium));
    webview = wxWebView::New(this, wxID_ANY, "http://www.blick.ch/",
                             wxDefaultPosition, wxDefaultSize, wxWebViewBackendChromium);

    //timer->Start(2000);
}

void SimpleFrame::OnTimer(wxTimerEvent &event)
{
    //wxDELETE(webview);
}

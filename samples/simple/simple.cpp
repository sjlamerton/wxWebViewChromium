/////////////////////////////////////////////////////////////////////////////
// Author: Steven Lamerton
// Copyright: (c) 2013 Steven Lamerton
// Licence: wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/sharedptr.h>
#include <wx/webview.h>
#include <wx/button.h>
#include <wx/sizer.h>

#include "simple.h"
#include "../../webview_chromium.h"

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

    wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);

    m_webview = wxWebView::New(this, wxID_ANY, "http://www.bbc.co.uk/",
                               wxDefaultPosition, wxDefaultSize, wxWebViewBackendChromium);

    wxButton* btn = new wxButton(this, wxID_ANY, "Load page");

    topsizer->Add(m_webview, 1, wxEXPAND);
    topsizer->Add(btn);

    this->SetSizer(topsizer);
    this->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SimpleFrame::OnButton, this);
}

void SimpleFrame::OnButton(wxCommandEvent &evt)
{
     m_webview->LoadURL(wxString("http://wxwidgets.org").ToStdString());
}

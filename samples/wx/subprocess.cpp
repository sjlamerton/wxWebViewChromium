/////////////////////////////////////////////////////////////////////////////
// Author: Steven Lamerton
// Copyright: (c) 2013 Steven Lamerton
// Licence: wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/app.h>
#include "../../webview_chromium.h"

class SubprocessApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnRun();
};

IMPLEMENT_APP(SubprocessApp)

bool SubprocessApp::OnInit()
{
    return true;
}

int SubprocessApp::OnRun()
{
    return wxWebViewChromium::StartUpSubprocess();
}

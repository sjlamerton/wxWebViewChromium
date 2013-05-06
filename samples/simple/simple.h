#ifndef _SIMPLE_H_
#define _SIMPLE_H_

#include <wx/app.h>
#include <wx/frame.h>

class SimpleApp : public wxApp
{
public:
    virtual bool OnInit();
    virtual int OnExit();
};

class SimpleFrame : public wxFrame
{
public:
    SimpleFrame();
    void OnButton(wxCommandEvent &evt);

private:
    wxWebView *m_webview;
};

IMPLEMENT_APP(SimpleApp)

#endif

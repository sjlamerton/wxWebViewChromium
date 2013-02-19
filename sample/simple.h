#ifndef _SIMPLE_H_
#define _SIMPLE_H_

#include <wx/app.h>
#include <wx/frame.h>
#include <wx/timer.h>

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

    void OnTimer(wxTimerEvent &event);
private:
    wxTimer *timer;
    wxWebView *webview;
};

IMPLEMENT_APP(SimpleApp)

#endif

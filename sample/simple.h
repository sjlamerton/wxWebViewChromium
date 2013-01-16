#ifndef _SIMPLE_H_
#define _SIMPLE_H_

#include <wx/app.h>
#include <wx/frame.h>

class SimpleApp : public wxApp
{
public:
    virtual bool OnInit();
};

class SimpleFrame : public wxFrame
{
public:
    SimpleFrame();
};

IMPLEMENT_APP(SimpleApp)

#endif

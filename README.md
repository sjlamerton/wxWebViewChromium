wxWebViewChromium
=================

wxWebViewChromium is a Chromium backend for wxWebView using the Chromium
Embedded Framework. For more detailed information, please see the project
[wiki][1].

Requirements
------------

* [wxWidgets][2]: r73369 / 2.9.5  or greater
* [Chromium Embedded Framework][3]:  1.963.439, 1.1025.607, 1.1180.832, 
   1.1364.1123, 3.1364.1188, 3.1453.1255, 3.1547.1412 or 3.1650.1562.

Compiling
---------

The backend is made up of five files, a source and header file for CEF1
and CEF3 and a selection header. Simply compile these alongside your
project and add the path to your Chromiumium Embedded Framework
install to your include directory list and link to `libcef_dll_wrapper`
and `libcef`.

Using
-----

To use wxWebViewChromium first register the backend with wxWidgets

    wxWebView::RegisterFactory(wxWebViewBackendChromium, 
                               wxSharedPtr<wxWebViewFactory>
                               (new wxWebViewFactoryChromium));

It can then be used in a standard `wxWebView::New` call

    wxWebView* webview = wxWebView::New(this, wxID_ANY,
                                        "http://www.wxwidgets.org/",
                                        wxDefaultPosition, wxDefaultSize,
                                        wxWebViewBackendChromium);

[1]: https://github.com/steve-lamerton/wxWebViewChromium/wiki
[2]: http://www.wxwidgets.org
[3]: http://code.google.com/p/chromiumembedded/


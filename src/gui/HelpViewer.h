#ifndef HELPVIEWER_H
#define HELPVIEWER_H

#include <QWebView>

class HelpViewer : public QWebView
{
public:
    HelpViewer(QWidget* parent=0, const char* name="HelpViewer");
    ~HelpViewer();
};

#endif // HELPVIEWER_H

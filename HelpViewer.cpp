#include <QUrl>

#include "HelpViewer.h"
#include "globalDefs.h"

HelpViewer::HelpViewer(QWidget *parent, const char *name):QWebView(parent)
{
    setObjectName(name);
    load(QUrl("doc/index.htm"));
}

HelpViewer::~HelpViewer()
{

}

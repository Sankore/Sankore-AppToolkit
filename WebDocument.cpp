#include "WebDocument.h"

WebDocument::WebDocument(const QString& docName, const char *name, QWidget *parent):DocumentWidget(docName, name, parent)
{
    mPath = docName;
    mpWidget = new QWebView(this);
    mpLayout->addWidget(mpWidget);

    dynamic_cast<QWebView*>(mpWidget)->load(docName);
}

WebDocument::~WebDocument()
{

}

void WebDocument::save()
{
    // Nothing to do
}

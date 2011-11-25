#ifndef WEBDOCUMENT_H
#define WEBDOCUMENT_H

#include <QWebView>

#include "DocumentWidget.h"

class WebDocument : public DocumentWidget
{
public:
    WebDocument(const QString& docName, const char* name="TextDocument", QWidget* parent=0);
    ~WebDocument();

protected:
    void save();
};


#endif // WEBDOCUMENT_H

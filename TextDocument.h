#ifndef TEXTDOCUMENT_H
#define TEXTDOCUMENT_H

#include <QTextEdit>

#include "DocumentWidget.h"

class TextDocument : public DocumentWidget
{
public:
    TextDocument(const QString& docName, const char* name="TextDocument", QWidget* parent=0);
    ~TextDocument();

protected:
    void save();
};

#endif // TEXTDOCUMENT_H

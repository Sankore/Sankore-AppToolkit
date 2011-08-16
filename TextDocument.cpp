#include <QFile>
#include <QDebug>

#include "TextDocument.h"

TextDocument::TextDocument(const QString& docName, const char *name, QWidget *parent):DocumentWidget(docName, name, parent)
{
    mpWidget = new QTextEdit(this);
    mpLayout->addWidget(mpWidget);

    QFile f(docName);
    if(f.exists())
    {
        if(f.open(QIODevice::ReadOnly))
        {
            QByteArray fileContent = f.readAll();

            dynamic_cast<QTextEdit*>(mpWidget)->setText(QString(fileContent));

            f.close();
        }
        else
        {
            qDebug() << "Error: cannot open file " << docName;
        }
    }
    else
    {
        qDebug() << "Error: file " << docName << "doesn't exist!";
    }
}

TextDocument::~TextDocument()
{

}

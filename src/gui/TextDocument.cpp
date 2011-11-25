#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "TextDocument.h"
#include "core/globalDefs.h"

TextDocument::TextDocument(const QString& docName, const char *name, QWidget *parent):DocumentWidget(docName, name, parent)
{
    mPath = docName;
    mpWidget = new QTextEdit(this);
    dynamic_cast<QTextEdit*>(mpWidget)->setTabStopWidth(40);
    mpLayout->addWidget(mpWidget);
    setHighlighter();

    QFile f(docName);
    if(f.exists())
    {
        if(f.open(QIODevice::ReadOnly))
        {
            QByteArray fileContent = f.readAll();
            dynamic_cast<QTextEdit*>(mpWidget)->document()->setPlainText(QString(fileContent));
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

    connect(dynamic_cast<QTextEdit*>(mpWidget), SIGNAL(textChanged()), this, SIGNAL(modified()));
}

TextDocument::~TextDocument()
{

}

void TextDocument::save()
{
    QFile f(mPath);
    if(f.exists())
    {
        if(f.open(QIODevice::WriteOnly))
        {
            f.write(dynamic_cast<QTextEdit*>(mpWidget)->toPlainText().toAscii());
            f.close();
        }
        else
        {
            qDebug() << "Error : cannot write on file " << mPath;
        }
    }
    else
    {
        qDebug() << "Error : file " << mPath << " doesn't exist!";
    }
}

void TextDocument::setHighlighter()
{
    QFileInfo fi(mPath);

    QString extension = fi.suffix();
    if("js" == extension)
    {
        mpHL = new JSSyntaxHighlighter(mpWidget);
    }
    else if("xml" == extension || "htm" == extension || "html" == extension || "xhtml" == extension || "xhtm" == extension)
    {
        mpHL = new XMLSyntaxHighlighter(mpWidget);
    }
}

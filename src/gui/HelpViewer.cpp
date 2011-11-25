#include <QUrl>
#include <QApplication>

#include "HelpViewer.h"
#include "core/globalDefs.h"

HelpViewer::HelpViewer(QWidget *parent, const char *name):QWebView(parent)
{
    setObjectName(name);

    QString qsDocFiles;
    QString qsDocFilename = "doc/index.htm";

#ifdef Q_WS_MAC
    qsDocFiles = QString("%0/../Resources/%1").arg(QApplication::applicationDirPath()).arg(qsDocFilename);
#else
    qsDocFiles = qsDocFilename;
#endif

    load(QUrl(qsDocFiles));
}

HelpViewer::~HelpViewer()
{

}

#include <QFileInfo>
#include <QTime>

#include "WidgetManager.h"
#include "globalDefs.h"

WidgetManager::WidgetManager(QObject* parent):QObject(parent)
{

}

WidgetManager::~WidgetManager()
{

}

void WidgetManager::setWidget(const QString& path)
{
    mPath = path;
    mIndex = QUrl::fromLocalFile(path + "/index.html");

    QFileInfo fi(path);
    mName = fi.fileName();
}

void WidgetManager::setCurrentFrame(QWebFrame *frame)
{
    mpFrame = frame;
    attachObject();
    connect(mpFrame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachObject()));
}

QString WidgetManager::path()
{
    return mPath;
}

QUrl WidgetManager::indexUrl()
{
    return mIndex;
}

QString WidgetManager::widgetName()
{
    return mName;
}

void WidgetManager::attachObject()
{
    mpFrame->addToJavaScriptWindowObject(QString("W3CInteractiveWidgetApi"), this);
}

void WidgetManager::returnStatus(QString command, QString status)
{
    QString logString = QTime::currentTime().toString("hh:mm:ss.zzz") + "  :   command: " + command + "   status:  " + status;
    emit (logFromJS(logString));
}

void WidgetManager::evaluateJS(const QString &cmd)
{
    // Send the command to the javascript
    mpFrame->evaluateJavaScript(cmd);
}

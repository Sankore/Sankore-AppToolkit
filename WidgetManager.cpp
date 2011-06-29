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
    mpApi = new UBWidgetUniboardAPI(this);
    connect(mpApi, SIGNAL(functionCalled(QString)), this, SIGNAL(logFromJS(QString)));
    mpFrame = frame;
    connect(mpFrame, SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(attachObject()));

    attachObject();

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
    mpFrame->addToJavaScriptWindowObject("sankore", mpApi);
}

void WidgetManager::returnStatus(QString command, QString status)
{
    QString logString = command + "   status:  " + status;
    emit (logFromJS(logString));
}

void WidgetManager::evaluateJS(const QString &cmd)
{
    // Send the command to the javascript
    mpFrame->evaluateJavaScript(cmd);
}

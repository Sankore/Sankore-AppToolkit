#include <QFileInfo>
#include <QTime>
#include <QDomDocument>
#include <QDebug>

#include "WidgetManager.h"
#include "core/globalDefs.h"

WidgetManager::WidgetManager(QObject* parent):QObject(parent)
{

}

WidgetManager::~WidgetManager()
{

}

void WidgetManager::setWidget(const QString& path)
{
    mPath = path;
    QString qsWidgetName;

    QString qsConfigPath = QString("%0/config.xml").arg(path);

    if(QFile::exists(qsConfigPath))
    {
        QFile f(qsConfigPath);
        if(f.open(QIODevice::ReadOnly))
        {
            QDomDocument domDoc;
            domDoc.setContent(QString(f.readAll()));
            QDomElement root = domDoc.documentElement();

            QDomNode node = root.firstChild();
            while(!node.isNull())
            {
                if(node.toElement().tagName() == "content")
                {
                    QDomAttr srcAttr = node.toElement().attributeNode("src");
                    qsWidgetName = srcAttr.value();
                    break;
                }
                node = node.nextSibling();
            }
            f.close();
        }
    }

    mIndex = QUrl::fromLocalFile(QString("%0/%1").arg(path).arg(qsWidgetName));

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

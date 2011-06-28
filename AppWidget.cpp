#include <QDebug>

#include "AppWidget.h"
#include "globalDefs.h"

AppWidget::AppWidget(QWidget *parent, const char *name):QWidget(parent)
  , mpSplitter(NULL)
  , mpWebView(NULL)
  , mpLayout(NULL)
  , mpSelectors(NULL)
  , mpQtToJS(NULL)
  , mpJSToQt(NULL)
  , mpSelectorLayout(NULL)
  , mpLogger(NULL)
  , mpSettings(NULL)
{
    setObjectName(name);
    mpSplitter = new QSplitter(this);
    mpLayout = new QVBoxLayout();
    setLayout(mpLayout);
    mpLayout->addWidget(mpSplitter);
    mpSplitter->setOrientation(Qt::Vertical);

    mpWebView = new QWebView(this);
    mpWebView->setStyleSheet(QString("background-color:white;"));
    mpWebView->load(QUrl("http://www.google.ch"));
    mpSplitter->addWidget(mpWebView);

    mpSelectors = new QWidget(this);
    mpSelectorLayout = new QHBoxLayout();
    mpSelectors->setLayout(mpSelectorLayout);
    mpQtToJS = new MessageSelector("QT -> JS");
    mpJSToQt = new MessageSelector("JS -> QT");
    mpSelectorLayout->addWidget(mpQtToJS);
    mpSelectorLayout->addWidget(mpJSToQt);

    populateMessages(mpQtToJS, "msg.config");

    mpSplitter->addWidget(mpSelectors);

    mpLogger = new LogWidget(this);
    mpSplitter->addWidget(mpLogger);

    connect(mpQtToJS, SIGNAL(sendMessage(int, const QString&)), this, SLOT(onMessageSentToJS(int, const QString&)));
    connect(mpJSToQt, SIGNAL(sendMessage(int, const QString&)), this, SLOT(onMessageSentToQt(int, const QString&)));
}

AppWidget::~AppWidget()
{
    DELETEPTR(mpSettings);
    DELETEPTR(mpLogger);
    DELETEPTR(mpQtToJS);
    DELETEPTR(mpJSToQt);
    DELETEPTR(mpSelectorLayout);
    DELETEPTR(mpSelectors);
    DELETEPTR(mpSplitter);
    DELETEPTR(mpWebView);
    DELETEPTR(mpLayout);
}

void AppWidget::populateMessages(MessageSelector *pSelector, const QString& filename)
{
    if(NULL == mpSettings)
    {
        // Load the settings file
        mpSettings = new QSettings(filename, QSettings::IniFormat);

        // Qt --> JS
        mpSettings->beginGroup("QtToJSLabels");
        QStringList qslKeys = mpSettings->childKeys();

        QStringList qslQtToJSLabels;
        foreach(QString eachLabel, qslKeys)
        {
            qslQtToJSLabels << mpSettings->value(eachLabel).toString();
        }
        mpSettings->endGroup();

        mpQtToJS->populateMessages(qslQtToJSLabels);

    }
}

void AppWidget::onMessageSentToJS(int msgID, const QString& label)
{
    QString key = QString("op%0Msg").arg(msgID);
    mpSettings->beginGroup("QtToJSMessages");

    // Send the message to the JS

    mpSettings->endGroup();

    // Log the message
    QString logMsg = QString(tr("%0 sent to the widget")).arg(label);
    mpLogger->LogMessage(logMsg, eMsgDirection_QtToJS);
}

void AppWidget::onMessageSentToQt(int msgID, const QString& label)
{
    // TODO: complete this class later...
    Q_UNUSED(msgID);
}

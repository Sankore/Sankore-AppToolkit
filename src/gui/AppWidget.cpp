#include <QDebug>
#include <QUrl>
#include <QWebPage>
#include <QApplication>

#include "AppWidget.h"
#include "core/globalDefs.h"

AppWidget::AppWidget(QWidget *parent, const char *name):QWidget(parent)
  , mpSplitter(NULL)
  , mpWebView(NULL)
  , mpLayout(NULL)
  , mpSelectors(NULL)
  , mpQtToJS(NULL)
  //, mpJSToQt(NULL)
  , mpSelectorLayout(NULL)
  , mpLogger(NULL)
  , mpSettings(NULL)
  , mpWebSettings(NULL)
  , mpRefreshLayout(NULL)
  , mpRefreshButton(NULL)
{
    setObjectName(name);
    mpSplitter = new QSplitter(this);
    mpLayout = new QVBoxLayout();
    setLayout(mpLayout);

    mpRefreshLayout = new QHBoxLayout();
    mpRefreshButton = new QPushButton(tr("Refresh"), this);
    mpRefreshLayout->addWidget(mpRefreshButton, 0);
    mpRefreshLayout->addStretch(1);
    mpLayout->addLayout(mpRefreshLayout);

    mpLayout->addWidget(mpSplitter);
    mpSplitter->setOrientation(Qt::Vertical);

    mpWebView = new QWebView(this);
    mpWebView->setStyleSheet(QString("background-color:white;"));
    mpWebView->load(QUrl(""));
    setWebSettings();
    mpSplitter->addWidget(mpWebView);

    mpSelectors = new QWidget(this);
    mpSelectorLayout = new QHBoxLayout();
    mpSelectors->setLayout(mpSelectorLayout);
    mpQtToJS = new MessageSelector("QT -> JS");
    //mpJSToQt = new MessageSelector("JS -> QT");
    mpSelectorLayout->addWidget(mpQtToJS);
    //mpSelectorLayout->addWidget(mpJSToQt);

    QString qsMsgFiles;
    QString qsMsgFilename = "msg.config";

#ifdef Q_WS_MAC
    qsMsgFiles = QString("%0/../Resources/%1").arg(QApplication::applicationDirPath()).arg(qsMsgFilename);
#else
    qsMsgFiles = qsMsgFilename;
#endif

    populateMessages(qsMsgFiles);

    mpSplitter->addWidget(mpSelectors);

    mpLogger = new LogWidget(this);
    mpSplitter->addWidget(mpLogger);

    connect(mpQtToJS, SIGNAL(sendMessage(int, const QString&)), this, SLOT(onMessageSentToJS(int, const QString&)));
    //connect(mpJSToQt, SIGNAL(sendMessage(int, const QString&)), this, SLOT(onMessageSentToQt(int, const QString&)));
    connect(this, SIGNAL(widgetLoaded()), mpQtToJS, SLOT(onLoaded()));
    //connect(this, SIGNAL(widgetLoaded()), mpJSToQt, SLOT(onLoaded()));
    connect(mpRefreshButton, SIGNAL(clicked()), this, SLOT(onRefreshButtonClicked()));
}

AppWidget::~AppWidget()
{
    DELETEPTR(mpRefreshButton);
    DELETEPTR(mpRefreshLayout);
    DELETEPTR(mpSettings);
    DELETEPTR(mpLogger);
    DELETEPTR(mpQtToJS);
    //DELETEPTR(mpJSToQt);
    DELETEPTR(mpSelectorLayout);
    DELETEPTR(mpSelectors);
    DELETEPTR(mpSplitter);
    DELETEPTR(mpLayout);
}

void AppWidget::populateMessages(const QString& filename)
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

void AppWidget::setWidgetManager(WidgetManager *mgr)
{
    mpWidgetManager = mgr;
    connect(mpWidgetManager, SIGNAL(logFromJS(QString)), this, SLOT(onMessageReceivedFromJS(QString)));
}

void AppWidget::refreshWidget()
{
    mpWebView->load(mpWidgetManager->indexUrl());
    mpWidgetManager->setCurrentFrame(mpWebView->page()->mainFrame());
}

void AppWidget::evaluateJS(const QString &cmd)
{
    mpWidgetManager->evaluateJS(cmd);
}

void AppWidget::setWebSettings()
{
    mpWebSettings = QWebSettings::globalSettings();
    mpWebSettings->setAttribute(QWebSettings::JavaEnabled, true);
    mpWebSettings->setAttribute(QWebSettings::PluginsEnabled, true);
    mpWebSettings->setAttribute(QWebSettings::JavaEnabled, true);
    mpWebSettings->setAttribute(QWebSettings::LocalStorageDatabaseEnabled, true);
    mpWebSettings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);
    mpWebSettings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    mpWebSettings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, true);
    mpWebSettings->setAttribute(QWebSettings::DnsPrefetchEnabled, true);


}

void AppWidget::onMessageSentToJS(int msgID, const QString& label)
{
    // Log the message
    QString logMsg = QString(tr("Sending %0 to the widget")).arg(label);
    mpLogger->LogMessage(logMsg, eMsgDirection_QtToJS);

    if("Reload" == label)
    {
        refreshWidget();
    }
    else
    {
        QString key = QString("op%0Msg").arg(msgID);
        mpSettings->beginGroup("QtToJSMessages");

        // Send the message to the JS
        evaluateJS(mpSettings->value(key).toString());

        mpSettings->endGroup();
    }
}

void AppWidget::onMessageSentToQt(int msgID, const QString& label)
{
    // TODO: complete this class later...
    Q_UNUSED(msgID);
    Q_UNUSED(label);
}

void AppWidget::onWidgetLoaded()
{
    refreshWidget();

    QString qsLogMsg = QString(tr("Widget %0 loaded")).arg(mpWidgetManager->widgetName());
    mpLogger->LogMessage(qsLogMsg, eMsgDirection_None);
    emit widgetLoaded();
}

void AppWidget::onMessageReceivedFromJS(const QString &msg)
{
    mpLogger->LogMessage(msg, eMsgDirection_JSToQt);
}

void AppWidget::onRefreshButtonClicked()
{
    if(NULL != mpWebView)
    {
        mpWebView->reload();
    }
}

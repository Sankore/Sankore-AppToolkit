#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QWebView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QSettings>
#include <QWebSettings>

#include "MessageSelector.h"
#include "LogWidget.h"
#include "WidgetManager.h"

class AppWidget : public QWidget
{
    Q_OBJECT
public:
    AppWidget(QWidget* parent=0, const char* name="AppWidget");
    ~AppWidget();
    void setWidgetManager(WidgetManager* mgr);

signals:
    void logMessage(const QString& msg);
    void widgetLoaded();

public slots:
    void onWidgetLoaded();

private slots:
    void onMessageSentToJS(int msgID, const QString& label);
    void onMessageSentToQt(int msgID, const QString& label);
    void onMessageReceivedFromJS(const QString& msg);
    void onRefreshButtonClicked();

private:
    void populateMessages(const QString& filename);
    void refreshWidget();
    void evaluateJS(const QString& cmd);
    void setWebSettings();

    QSplitter* mpSplitter;
    QWebView* mpWebView;
    QVBoxLayout* mpLayout;
    QWidget* mpSelectors;
    MessageSelector* mpQtToJS;
    MessageSelector* mpJSToQt;
    QHBoxLayout* mpSelectorLayout;
    LogWidget* mpLogger;
    QSettings* mpSettings;
    WidgetManager* mpWidgetManager;
    QWebSettings* mpWebSettings;
    QHBoxLayout* mpRefreshLayout;
    QPushButton* mpRefreshButton;
};

#endif // APPWIDGET_H

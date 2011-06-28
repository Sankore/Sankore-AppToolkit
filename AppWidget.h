#ifndef APPWIDGET_H
#define APPWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QWebView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFile>
#include <QSettings>

#include "MessageSelector.h"
#include "LogWidget.h"

class AppWidget : public QWidget
{
    Q_OBJECT
public:
    AppWidget(QWidget* parent=0, const char* name="AppWidget");
    ~AppWidget();

signals:
    void logMessage(const QString& msg);

private slots:
    void onMessageSentToJS(int msgID, const QString& label);
    void onMessageSentToQt(int msgID, const QString& label);

private:
    void populateMessages(MessageSelector* pSelector, const QString& filename);

    QSplitter* mpSplitter;
    QWebView* mpWebView;
    QVBoxLayout* mpLayout;
    QWidget* mpSelectors;
    MessageSelector* mpQtToJS;
    MessageSelector* mpJSToQt;
    QHBoxLayout* mpSelectorLayout;
    LogWidget* mpLogger;
    QSettings* mpSettings;
};

#endif // APPWIDGET_H

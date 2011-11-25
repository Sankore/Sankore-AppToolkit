#ifndef WIDGETMANAGER_H
#define WIDGETMANAGER_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QWebFrame>

#include "api/UBWidgetUniboardAPI.h"

class WidgetManager : public QObject
{
    Q_OBJECT
public:
    WidgetManager(QObject* parent=0);
    ~WidgetManager();

    void setWidget(const QString& path);
    void setCurrentFrame(QWebFrame* frame);
    void evaluateJS(const QString& cmd);
    QString path();
    QUrl indexUrl();
    QString widgetName();

signals:
    void javaScriptWindowObjectCleared();
    void logFromJS(QString log);

public slots:
    void returnStatus(QString command, QString status);

private slots:
    void attachObject();

private:
    QString mPath;
    QUrl mIndex;
    QString mName;
    QWebFrame* mpFrame;
    UBWidgetUniboardAPI* mpApi;
};

#endif // WIDGETMANAGER_H

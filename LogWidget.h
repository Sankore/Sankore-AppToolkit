#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QTreeWidget>

typedef enum
{
    eMsgDirection_None,
    eMsgDirection_QtToJS,
    eMsgDirection_JSToQt
}eMsgDirection;

class LogWidget : public QTreeWidget
{
public:
    LogWidget(QWidget* parent=0, const char* name="LogWidget");
    ~LogWidget();

    void LogMessage(const QString& msg, eMsgDirection direction);
};

#endif // LOGWIDGET_H

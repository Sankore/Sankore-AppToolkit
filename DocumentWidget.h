#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>

class DocumentWidget : public QWidget
{
public:
    DocumentWidget(const QString& docName, const char* name="DocumentWidget", QWidget* parent=0);
    ~DocumentWidget();

    QWidget* widget();
    QString name();

protected:
    QWidget* mpWidget;
    QString mName;
    QVBoxLayout* mpLayout;
};

#endif // DOCUMENTWIDGET_H

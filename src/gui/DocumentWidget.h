#ifndef DOCUMENTWIDGET_H
#define DOCUMENTWIDGET_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>

class DocumentWidget : public QWidget
{
    Q_OBJECT
public:
    DocumentWidget(const QString& docName, const char* name="DocumentWidget", QWidget* parent=0);
    ~DocumentWidget();

    QWidget* widget();
    QString name();

    virtual void save();

signals:
    void modified();

protected:
    QWidget* mpWidget;
    QString mName;
    QString mPath;
    QVBoxLayout* mpLayout;
};

#endif // DOCUMENTWIDGET_H

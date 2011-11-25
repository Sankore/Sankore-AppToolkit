#ifndef DOCUMENTTAB_H
#define DOCUMENTTAB_H

#include <QTabWidget>
#include <QMap>
#include <QTextEdit>

#include "DocumentWidget.h"

typedef enum
{
    eDocType_Text,
    eDocType_Any
}eDocType;

class DocumentTab : public QTabWidget
{
    Q_OBJECT

public:
    DocumentTab(QWidget* parent=0, const char* name="DocumentTab");
    ~DocumentTab();

    void addPage(const QString& path);
    void removePage(const QString& name);
    void clear();
    void saveCurrentDoc();

public slots:
    void onFileDoubleClicked(const QString& path);
    void onFileRenamed(const QString& oldPath, const QString& newPath);
    void onFileDeleted(const QString& path);

private slots:
    void onTabCloseRequest(int index);
    void onDocModified();

private:
    eDocType getDocType(const QString& docName);

    QMap<QString, DocumentWidget*> mWidgets;
};

#endif // DOCUMENTTAB_H

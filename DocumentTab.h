#ifndef DOCUMENTTAB_H
#define DOCUMENTTAB_H

#include <QTabWidget>
#include <QMap>
#include <QTextEdit>

class DocumentTab : public QTabWidget
{
    Q_OBJECT

public:
    DocumentTab(QWidget* parent=0, const char* name="DocumentTab");
    ~DocumentTab();

    void addPage(const QString& name, QTextEdit* textEditor);
    void removePage(const QString& name);

private:
    QMap<QString, QTextEdit*> mpPages;
};

#endif // DOCUMENTTAB_H

#ifndef PROJECTTREE_H
#define PROJECTTREE_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDir>

class ProjectTree : public QTreeWidget
{
    Q_OBJECT
public:
    ProjectTree(QWidget* parent=0, const char* name="ProjectTree");
    ~ProjectTree();
    void setProjectPath(const QString& path);

signals:
    void fileDoubleClicked(const QString& path);

private slots:
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
    void getContent(QDir* pParentDir, QTreeWidgetItem* pParentItem);

    QTreeWidgetItem* pItem;
};

#endif // PROJECTTREE_H

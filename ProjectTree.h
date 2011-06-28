#ifndef PROJECTTREE_H
#define PROJECTTREE_H

#include <QTreeWidget>

class ProjectTree : public QTreeWidget
{
    Q_OBJECT
public:
    ProjectTree(QWidget* parent=0, const char* name="ProjectTree");
    ~ProjectTree();
};

#endif // PROJECTTREE_H

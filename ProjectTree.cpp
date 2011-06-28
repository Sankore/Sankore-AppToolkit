#include "ProjectTree.h"
#include "globalDefs.h"

ProjectTree::ProjectTree(QWidget *parent, const char *name):QTreeWidget(parent)
{
    setObjectName(name);
}

ProjectTree::~ProjectTree()
{

}

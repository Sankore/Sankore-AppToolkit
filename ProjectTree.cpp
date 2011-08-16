#include <QDebug>
#include <QFileInfo>
#include <QUrl>

#include "ProjectTree.h"
#include "globalDefs.h"

ProjectTree::ProjectTree(QWidget *parent, const char *name):QTreeWidget(parent)
{
    setObjectName(name);
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*,int)));
}

ProjectTree::~ProjectTree()
{

}

void ProjectTree::setProjectPath(const QString &path)
{
    // Clear the tree
    this->clear();
    QTreeWidgetItem* pRoot = this->invisibleRootItem();

    // Get the current widget
    QDir projectDir(path);
    QFileInfo fi(path);

    pItem = new QTreeWidgetItem(this);
    pItem->setText(0, fi.baseName());
    pItem->setIcon(0, QIcon(":resources/folder.png"));
    pItem->setData(0, Qt::UserRole, QVariant(path));
    pRoot->addChild(pItem);
    getContent(&projectDir, pItem);
}

void ProjectTree::getContent(QDir *pParentDir, QTreeWidgetItem* pParentItem)
{
    if(NULL != pParentDir && NULL != pParentItem)
    {
        QStringList qslFiles = pParentDir->entryList();
        for(int i = 0; i < qslFiles.size(); i++)
        {
            if(qslFiles.at(i) != "." && qslFiles.at(i) != "..")
            {
                QString qsPath = QString("%0/%1").arg(pParentDir->path()).arg(qslFiles.at(i));
                pItem = new QTreeWidgetItem();
                pItem->setText(0, qslFiles.at(i));
                pItem->setData(0, Qt::UserRole, QVariant(QUrl(qsPath).toLocalFile()));
                pParentItem->addChild(pItem);
                QString itemPath = QString("%0/%1").arg(pParentDir->path()).arg(qslFiles.at(i));
                QFileInfo fi(itemPath);
                if(fi.isDir())
                {
                    pItem->setIcon(0, QIcon(":resources/folder.png"));
                    QDir crntDir(itemPath);
                    getContent(&crntDir, pItem);
                }
                else
                {
                    pItem->setIcon(0, QIcon(":resources/file.png"));
                }
            }
        }
    }
}

void ProjectTree::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    // Here we get the path of the file and send it with the signal
    QString itemPath = item->data(column, Qt::UserRole).toString();
    QFileInfo fi(itemPath);
    if(fi.isFile())
    {
        emit fileDoubleClicked(itemPath);
    }
}

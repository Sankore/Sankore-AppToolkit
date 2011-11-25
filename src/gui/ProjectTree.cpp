#include <QDebug>
#include <QFileInfo>
#include <QUrl>
#include <QCursor>
#include <QMessageBox>
#include <QHeaderView>

#include "ProjectTree.h"
#include "core/globalDefs.h"

ProjectTree::ProjectTree(QWidget *parent, const char *name):QTreeWidget(parent)
  , pItem(NULL)
  , mpContextMenu(NULL)
  , mpActionNew(NULL)
  , mpActionRename(NULL)
  , mpActionDelete(NULL)
{
    setObjectName(name);
    connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*,int)));

    header()->hide();

    mpContextMenu = new QMenu(this);
    mpActionNew = mpContextMenu->addAction(tr("New File"));
    mpActionRename = mpContextMenu->addAction(tr("Rename"));
    mpActionDelete = mpContextMenu->addAction(tr("Delete"));

    connect(mpActionNew, SIGNAL(triggered()), this, SLOT(onNewClicked()));
    connect(mpActionRename, SIGNAL(triggered()), this, SLOT(onRenameClicked()));
    connect(mpActionDelete, SIGNAL(triggered()), this, SLOT(onDeleteClicked()));
}

ProjectTree::~ProjectTree()
{
    DELETEPTR(mpActionDelete);
    DELETEPTR(mpActionNew);
    DELETEPTR(mpActionRename);
    DELETEPTR(pItem);
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
                pItem->setData(0, Qt::UserRole, QVariant(QUrl::fromLocalFile(qsPath)));
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
    QString itemPath = QUrl(item->data(column, Qt::UserRole).toString()).toLocalFile();
    QFileInfo fi(itemPath);
    if(fi.isFile())
    {
        emit fileDoubleClicked(itemPath);
    }
}

void ProjectTree::contextMenuEvent(QContextMenuEvent *ev)
{
    Q_UNUSED(ev);
    QPoint mousePos = QCursor::pos();
    QTreeWidgetItem* pCrntItem = currentItem();
    if(NULL != pCrntItem)
    {
        // Disable some menu items in case of a folder
        QString qsPath = pCrntItem->data(0, Qt::UserRole).toString();
        QFileInfo fi(qsPath);
        if(fi.isDir())
        {
            mpActionRename->setEnabled(false);
            mpActionDelete->setEnabled(false);
        }
        else
        {
            mpActionRename->setEnabled(true);
            mpActionDelete->setEnabled(true);
        }

        // The context menu should appear only if an item is selected
        // Get the mouse position and set the geometry of the menu to it
        mpContextMenu->setGeometry(mousePos.x(), mousePos.y(), mpContextMenu->width(), 70);
        mpContextMenu->show();
    }
}

void ProjectTree::onNewClicked()
{
    FileManagementDlg dlg(eDialogType_NewFile);
    if(QDialog::Accepted == dlg.exec())
    {
        QTreeWidgetItem* pCrntItem = currentItem();
        if(NULL != pCrntItem)
        {
            QTreeWidgetItem* pParentItem = NULL;
            QString qsNewPath;
            QString qsPath = pCrntItem->data(0, Qt::UserRole).toString();
            QFileInfo fi(qsPath);
            if(fi.isDir())
            {
                pParentItem = pCrntItem;
                qsNewPath = QString("%0%1%2").arg(qsPath).arg(QDir::separator()).arg(dlg.name());
            }
            else
            {
                pParentItem = pCrntItem->parent();
                qsNewPath = QString("%0%1%2").arg(fi.path()).arg(QDir::separator()).arg(dlg.name());
            }
            QFile newFile(qsNewPath);
            if(newFile.exists())
            {
                QMessageBox::critical(parentWidget(), tr("Error"), tr("You cannot create this file because it already exists."));
            }
            else
            {
                // Create the file
                if(newFile.open(QIODevice::WriteOnly))
                {
                    newFile.close();
                }

                // Add the item to the tree
                pItem = new QTreeWidgetItem(pParentItem);
                pItem->setText(0, dlg.name());
                pItem->setIcon(0, QIcon(":resources/file.png"));
                pItem->setData(0, Qt::UserRole, QVariant(qsNewPath));
            }
        }
    }
}

void ProjectTree::onRenameClicked()
{
    FileManagementDlg dlg(eDialogType_Rename);
    if(QDialog::Accepted == dlg.exec())
    {
        QTreeWidgetItem* pCrntItem = currentItem();
        QTreeWidgetItem* pParentItem = pCrntItem->parent();
        if(NULL != pCrntItem)
        {
            QString qsNewPath;
            QString qsPath = pCrntItem->data(0, Qt::UserRole).toString();
            QFileInfo fi(qsPath);
            if(fi.isDir())
            {
                // It should never happen because a folder cannot be renamed for this version.
            }
            else
            {
                qsNewPath = QString("%0%1%2").arg(fi.path()).arg(QDir::separator()).arg(dlg.name());
                if(!QFile::exists(qsNewPath))
                {
                    QFile f(qsPath);
                    f.rename(qsNewPath);
                    pParentItem->removeChild(pCrntItem);
                    pItem = new QTreeWidgetItem(pParentItem);
                    pItem->setText(0, dlg.name());
                    pItem->setIcon(0, QIcon(":resources/file.png"));
                    pItem->setData(0, Qt::UserRole, QVariant(qsNewPath));

                    // Notify that this file has been renamed
                    emit fileRenamed(qsPath, qsNewPath);
                    setCurrentItem(pItem);
                }
                else
                {
                    QMessageBox::critical(parentWidget(), tr("Error"), tr("You cannot rename this file with this name because it already exists."));
                }
            }
        }
    }
}

void ProjectTree::onDeleteClicked()
{
    FileManagementDlg dlg(eDialogType_Delete);
    if(QDialog::Accepted == dlg.exec())
    {
        QTreeWidgetItem* pCrntItem = currentItem();
        QString qsPath = pCrntItem->data(0, Qt::UserRole).toString();
        if(NULL != pCrntItem)
        {
            QTreeWidgetItem* pParentItem = pCrntItem->parent();
            if(NULL != pParentItem)
            {
                // Remove the item from the tree
                pParentItem->removeChild(pCrntItem);

                // Remove the item from the file system
                QFile f(qsPath);
                if(f.remove())
                {
                    // Notify its destruction
                    emit fileDeleted(qsPath);
                }
            }
        }
    }
}

FileManagementDlg::FileManagementDlg(eDialogType type, const char *name, QWidget *parent) : QDialog(parent)
    , mpLayout(NULL)
    , mpLabel(NULL)
    , mpLineEdit(NULL)
    , mpBttn(NULL)
    , mpHLayout(NULL)
{
    setObjectName(name);
    setFixedSize(300, 100);
    mType = type;
    mpLayout = new QVBoxLayout();
    setLayout(mpLayout);

    switch(type)
    {
        case eDialogType_NewFile:
            setWindowTitle(tr("Add New File"));
            mpHLayout = new QHBoxLayout();
            mpLabel = new QLabel(tr("File name:"), this);
            mpLineEdit = new QLineEdit(this);
            mpHLayout->addWidget(mpLabel, 0);
            mpHLayout->addWidget(mpLineEdit, 1);
            mpLayout->addLayout(mpHLayout, 0);
        break;

        case eDialogType_Rename:
            setWindowTitle(tr("Rename File"));
            mpHLayout = new QHBoxLayout();
            mpLabel = new QLabel(tr("File name:"), this);
            mpLineEdit = new QLineEdit(this);
            mpHLayout->addWidget(mpLabel, 0);
            mpHLayout->addWidget(mpLineEdit, 1);
            mpLayout->addLayout(mpHLayout, 0);
        break;

        case eDialogType_Delete:
            setWindowTitle(tr("Delete"));
            mpLabel = new QLabel(tr("The selected item will be deleted."), this);
            mpLayout->addWidget(mpLabel, 0);
        break;

        case eDialogType_NewFolder:
            // Maybe this case will be handled in a futur development...
        break;
    }

    mpBttn = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this);
    mpLayout->addWidget(mpBttn, 0);

    connect(mpBttn, SIGNAL(accepted()), this, SLOT(validate()));
    connect(mpBttn, SIGNAL(rejected()), this, SLOT(reject()));
}

FileManagementDlg::~FileManagementDlg()
{
    DELETEPTR(mpLabel);
    DELETEPTR(mpLineEdit);
    DELETEPTR(mpBttn);
    DELETEPTR(mpHLayout);
    DELETEPTR(mpLayout);
}

void FileManagementDlg::validate()
{
    switch(mType)
    {
        case eDialogType_NewFile:
        case eDialogType_NewFolder:
        case eDialogType_Rename:
            if("" == mpLineEdit->text())
            {
                // Some infos are missing
                QMessageBox::warning(this, tr("Missing information"), tr("Please enter a correct value!"));

                return;
            }
        break;
        case eDialogType_Delete:
        break;
    }
    accept();
}

QString FileManagementDlg::name()
{
    QString qsRet = "";
    if(NULL != mpLineEdit)
    {
        qsRet = mpLineEdit->text();
    }
    return qsRet;
}

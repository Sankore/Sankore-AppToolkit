#ifndef PROJECTTREE_H
#define PROJECTTREE_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDir>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QHBoxLayout>

typedef enum
{
    eDialogType_NewFile,
    eDialogType_NewFolder,
    eDialogType_Rename,
    eDialogType_Delete
}eDialogType;

class FileManagementDlg : public QDialog
{
    Q_OBJECT
public:
    FileManagementDlg(eDialogType type, const char* name="FileManagementDlg", QWidget* parent=0);
    ~FileManagementDlg();
    QString name();

private slots:
    void validate();

private:
    eDialogType mType;
    QVBoxLayout* mpLayout;
    QLabel* mpLabel;
    QLineEdit* mpLineEdit;
    QDialogButtonBox* mpBttn;
    QHBoxLayout* mpHLayout;
};

class ProjectTree : public QTreeWidget
{
    Q_OBJECT
public:
    ProjectTree(QWidget* parent=0, const char* name="ProjectTree");
    ~ProjectTree();
    void setProjectPath(const QString& path);

signals:
    void fileDoubleClicked(const QString& path);
    void fileRenamed(const QString& oldPath, const QString& newPath);
    void fileDeleted(const QString& path);

protected:
    void contextMenuEvent(QContextMenuEvent* ev);

private slots:
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);
    void onNewClicked();
    void onRenameClicked();
    void onDeleteClicked();

private:
    void getContent(QDir* pParentDir, QTreeWidgetItem* pParentItem);

    QTreeWidgetItem* pItem;
    QMenu* mpContextMenu;
    QAction* mpActionNew;
    QAction* mpActionRename;
    QAction* mpActionDelete;
};

#endif // PROJECTTREE_H

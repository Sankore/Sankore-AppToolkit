#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QMenu>
#include <QMenuBar>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>

#include "ProjectTree.h"
#include "DocumentTab.h"
#include "AppWidget.h"
#include "WidgetManager.h"
#include "HelpViewer.h"

#define ADD_TREE    1
#define ADD_EDITOR  1
#define ADD_VIEWER  1
#define ADD_HELP    1

#define PROJECT_TEMPLATE    "sample.wgt"

class NewProjectDlg : public QDialog
{
    Q_OBJECT
public:
    NewProjectDlg(const char* name="NewProjectDlg", QWidget* parent=0);
    ~NewProjectDlg();
    QString path();

private slots:
    void onBrowseClicked();
    void validateFields();
    void onOkClicked();

private:
    void copyFolder(QString sourceFolder, QString destFolder);

    QLabel* mpNameLb;
    QLabel* mpLocationLb;
    QLineEdit* mpName;
    QLineEdit* mpLocation;
    QPushButton* mpBrowseBttn;
    QDialogButtonBox* mpDlgBttn;
    QVBoxLayout* mpLayout;
    QHBoxLayout* mpNameLayout;
    QHBoxLayout* mpLocationLayout;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    WidgetManager* widgetManager();

signals:
    void widgetLoaded();


private slots:
    void onFileClose();
    void onFileOpen();
    void onHelpShow();
    void onHelpHide();
    void onFileSave();
    void onFileNewProject();

private:
    void createMenuBar();

    QSplitter* mpSplitter;
    ProjectTree* mpTree;
    DocumentTab* mpTabs;
    AppWidget* mpAppWidget;
    QMenuBar* mpMenuBar;
    QMenu* mpMenuFile;
    WidgetManager* mpWidgetManager;
    HelpViewer* mpHelpViewer;
    QMenu* mpMenuHelp;
};

#endif // MAINWINDOW_H

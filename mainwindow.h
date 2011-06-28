#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QMenu>
#include <QMenuBar>

#include "ProjectTree.h"
#include "DocumentTab.h"
#include "AppWidget.h"
#include "WidgetManager.h"

//#define ADD_TREE    1
//#define ADD_EDITOR  1
#define ADD_VIEWER  1

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

private:
    void createMenuBar();

    QSplitter* mpSplitter;
    ProjectTree* mpTree;
    DocumentTab* mpTabs;
    AppWidget* mpAppWidget;
    QMenuBar* mpMenuBar;
    QMenu* mpMenuFile;
    WidgetManager* mpWidgetManager;
};

#endif // MAINWINDOW_H

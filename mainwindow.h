#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>

#include "ProjectTree.h"
#include "DocumentTab.h"
#include "AppWidget.h"

//#define ADD_TREE    1
//#define ADD_EDITOR  1
#define ADD_VIEWER  1

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QSplitter* mpSplitter;
    ProjectTree* mpTree;
    DocumentTab* mpTabs;
    AppWidget* mpAppWidget;
};

#endif // MAINWINDOW_H

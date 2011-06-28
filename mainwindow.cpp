#include "mainwindow.h"
#include "globalDefs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mpSplitter(NULL)
    , mpTree(NULL)
    , mpTabs(NULL)
    , mpAppWidget(NULL)
{
    setWindowTitle(tr("Sankore Application Toolkit"));
    resize(800, 600);
    mpSplitter = new QSplitter(this);
    setCentralWidget(mpSplitter);

#ifdef ADD_TREE
    mpTree = new ProjectTree(this);
    mpSplitter->addWidget(mpTree);
#endif
#ifdef ADD_EDITOR
    mpTabs = new DocumentTab(this);
    mpSplitter->addWidget(mpTabs);
#endif
#ifdef ADD_VIEWER
    mpAppWidget = new AppWidget(this);
    mpSplitter->addWidget(mpAppWidget);
#endif
}

MainWindow::~MainWindow()
{
    DELETEPTR(mpAppWidget);
    DELETEPTR(mpTabs);
    DELETEPTR(mpSplitter);
    DELETEPTR(mpTree);
}

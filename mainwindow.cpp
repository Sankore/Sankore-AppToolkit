#include <QAction>
#include <QApplication>
#include <QFileDialog>

#include "mainwindow.h"
#include "globalDefs.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , mpSplitter(NULL)
    , mpTree(NULL)
    , mpTabs(NULL)
    , mpAppWidget(NULL)
    , mpMenuBar(NULL)
    , mpMenuFile(NULL)
    , mpWidgetManager(NULL)
{
    setWindowTitle(tr("Sankore Application Toolkit"));
    resize(800, 600);
    mpSplitter = new QSplitter(this);
    setCentralWidget(mpSplitter);

    mpWidgetManager = new WidgetManager();

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
    mpAppWidget->setWidgetManager(mpWidgetManager);
    mpSplitter->addWidget(mpAppWidget);
    connect(this, SIGNAL(widgetLoaded()), mpAppWidget, SLOT(onWidgetLoaded()));
#endif

    createMenuBar();
}

MainWindow::~MainWindow()
{
    DELETEPTR(mpMenuFile);
    DELETEPTR(mpMenuBar);
    DELETEPTR(mpAppWidget);
    DELETEPTR(mpTabs);
    DELETEPTR(mpSplitter);
    DELETEPTR(mpTree);
    DELETEPTR(mpWidgetManager);
}

void MainWindow::createMenuBar()
{
    mpMenuBar = new QMenuBar(this);
    setMenuBar(mpMenuBar);

    mpMenuFile = new QMenu(tr("File"), this);
    mpMenuBar->addMenu(mpMenuFile);

    QAction* pAction = mpMenuFile->addAction(tr("Open"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onFileOpen()));

    pAction = mpMenuFile->addAction(tr("Close"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onFileClose()));
}

WidgetManager* MainWindow::widgetManager()
{
    return mpWidgetManager;
}

void MainWindow::onFileClose()
{
    QApplication::quit();
}

void MainWindow::onFileOpen()
{
    QString widgetPath = QFileDialog::getExistingDirectory( this, tr("Open widget"), "", QFileDialog::ShowDirsOnly);
    mpWidgetManager->setWidget(widgetPath);

    emit widgetLoaded();
}

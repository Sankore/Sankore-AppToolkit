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
    , mpHelpViewer(NULL)
{
    setWindowTitle(tr("Sankore Application Toolkit"));
    setWindowIcon(QIcon(":resources/uniboard.png"));
    resize(800, 600);
    mpSplitter = new QSplitter(this);
    setCentralWidget(mpSplitter);

    mpWidgetManager = new WidgetManager();

#ifdef ADD_TREE
    mpTree = new ProjectTree(this);
    mpSplitter->addWidget(mpTree);
    mpSplitter->setStretchFactor(mpSplitter->count()-1, 0);
#endif
#ifdef ADD_EDITOR
    mpTabs = new DocumentTab(this);
    mpSplitter->addWidget(mpTabs);
    mpSplitter->setStretchFactor(mpSplitter->count()-1, 1);
#endif

#ifdef ADD_VIEWER
    mpAppWidget = new AppWidget(this);
    mpAppWidget->setWidgetManager(mpWidgetManager);
    mpSplitter->addWidget(mpAppWidget);
    connect(this, SIGNAL(widgetLoaded()), mpAppWidget, SLOT(onWidgetLoaded()));
    mpSplitter->setStretchFactor(mpSplitter->count()-1, 0);
#endif
#ifdef ADD_HELP
    mpHelpViewer = new HelpViewer(this);
    mpSplitter->addWidget(mpHelpViewer);
    mpHelpViewer->hide();
    mpSplitter->setStretchFactor(mpSplitter->count()-1, 0);
#endif
    createMenuBar();

#if defined ADD_TREE && defined ADD_EDITOR
    connect(mpTree, SIGNAL(fileDoubleClicked(QString)), mpTabs, SLOT(onFileDoubleClicked(QString)));
#endif
}

MainWindow::~MainWindow()
{
    DELETEPTR(mpMenuHelp);
    DELETEPTR(mpHelpViewer);
    DELETEPTR(mpMenuFile);
    DELETEPTR(mpMenuBar);
    DELETEPTR(mpAppWidget);
    DELETEPTR(mpTabs);
    DELETEPTR(mpTree);
    DELETEPTR(mpSplitter);
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

#ifdef ADD_EDITOR
    mpMenuFile->addSeparator();
    pAction = mpMenuFile->addAction(tr("Save"));
    pAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onFileSave()));
    mpMenuFile->addSeparator();
#endif

    pAction = mpMenuFile->addAction(tr("Close"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onFileClose()));

#ifdef ADD_HELP
    mpMenuHelp = new QMenu(tr("Help"), this);
    mpMenuBar->addMenu(mpMenuHelp);
    pAction = mpMenuHelp->addAction(tr("Show Help"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onHelpShow()));

    pAction = mpMenuHelp->addAction(tr("Hide Help"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onHelpHide()));
#endif
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
    QString qsDefaultPath;

#ifdef Q_WS_MAC
    qsDefaultPath = QString("%0/../Resources/").arg(QApplication::applicationDirPath());
#else
    qsDefaultPath = QApplication::applicationDirPath();
#endif

    QString widgetPath = QFileDialog::getExistingDirectory( this, tr("Open widget"), qsDefaultPath, QFileDialog::ShowDirsOnly);

    if("" != widgetPath)
    {
#ifdef ADD_EDITOR
        mpTabs->clear();
#endif

#ifdef ADD_TREE
        mpTree->setProjectPath(widgetPath);
#endif
        mpWidgetManager->setWidget(widgetPath);

        emit widgetLoaded();
    }
}

void MainWindow::onHelpShow()
{
    mpHelpViewer->show();
}

void MainWindow::onHelpHide()
{
    mpHelpViewer->hide();
}

void MainWindow::onFileSave()
{
#ifdef ADD_EDITOR
    mpTabs->saveCurrentDoc();
#endif
}


#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QDebug>

#include "mainwindow.h"
#include "core/globalDefs.h"

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
    connect(mpTree, SIGNAL(fileRenamed(QString,QString)), mpTabs, SLOT(onFileRenamed(QString,QString)));
    connect(mpTree, SIGNAL(fileDeleted(QString)), mpTabs, SLOT(onFileDeleted(QString)));
#endif
}

MainWindow::~MainWindow()
{
    DELETEPTR(mpMenuHelp);
    DELETEPTR(mpHelpViewer);
    DELETEPTR(mpMenuFile);
//    DELETEPTR(mpMenuTools);
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

    QAction* pAction = mpMenuFile->addAction(tr("New Project"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onFileNewProject()));

    pAction = mpMenuFile->addAction(tr("Open"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onFileOpen()));

#ifdef ADD_EDITOR
    mpMenuFile->addSeparator();
    pAction = mpMenuFile->addAction(tr("Save"));
    pAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onFileSave()));
    mpMenuFile->addSeparator();
#endif

    pAction = mpMenuFile->addAction(tr("Quit"));
    connect(pAction, SIGNAL(triggered()), this, SLOT(onFileQuit()));

//    mpMenuTools = new QMenu(tr("Tools"), this);
//    mpMenuBar->addMenu(mpMenuTools);
//    pAction = mpMenuTools->addAction(tr("Options..."));
//    connect(pAction, SIGNAL(triggered()), this, SLOT(onToolsOption()));

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

void MainWindow::onFileQuit()
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

void MainWindow::onFileNewProject()
{
    NewProjectDlg dlg;
    if(QDialog::Accepted == dlg.exec())
    {
        // Close the current project if it exists (see ::onFileOpen())
        mpTabs->clear();

        mpTree->setProjectPath(dlg.path());

        mpWidgetManager->setWidget(dlg.path());

        emit widgetLoaded();
    }
}

NewProjectDlg::NewProjectDlg(const char *name, QWidget *parent):QDialog(parent)
    , mpNameLb(NULL)
    , mpLocationLb(NULL)
    , mpName(NULL)
    , mpLocation(NULL)
    , mpBrowseBttn(NULL)
    , mpDlgBttn(NULL)
    , mpLayout(NULL)
    , mpNameLayout(NULL)
    , mpLocationLayout(NULL)
{
    setObjectName(name);
    setFixedHeight(150);
    setWindowTitle(tr("Create a new project"));
    // Build the GUI
    mpLayout = new QVBoxLayout();
    setLayout(mpLayout);

    // Name
    mpNameLayout = new QHBoxLayout();
    mpNameLb = new QLabel(tr("Name: "), this);
    mpName = new QLineEdit(this);
    mpNameLayout->addWidget(mpNameLb, 0);
    mpNameLayout->addWidget(mpName, 1);
    mpLayout->addLayout(mpNameLayout);

    // Location
    mpLocationLayout = new QHBoxLayout();
    mpLocationLb = new QLabel(tr("Location: "), this);
    mpLocation = new QLineEdit(this);
    mpBrowseBttn = new QPushButton(tr("Browse"), this);
    mpLocationLayout->addWidget(mpLocationLb, 0);
    mpLocationLayout->addWidget(mpLocation, 1);
    mpLocationLayout->addWidget(mpBrowseBttn, 0);
    mpLayout->addLayout(mpLocationLayout);

    // Buttons
    mpDlgBttn = new QDialogButtonBox(QDialogButtonBox::Cancel | QDialogButtonBox::Ok, Qt::Horizontal, this);
    mpDlgBttn->button(QDialogButtonBox::Ok)->setEnabled(false);
    mpLayout->addWidget(mpDlgBttn);

    // Connect signal/slots
    connect(mpDlgBttn, SIGNAL(accepted()), this, SLOT(onOkClicked()));
    connect(mpDlgBttn, SIGNAL(rejected()), this, SLOT(reject()));
    connect(mpBrowseBttn, SIGNAL(clicked()), this, SLOT(onBrowseClicked()));
    connect(mpName, SIGNAL(textChanged(QString)), this, SLOT(validateFields()));
    connect(mpLocation, SIGNAL(textChanged(QString)), this, SLOT(validateFields()));
}

NewProjectDlg::~NewProjectDlg()
{
    DELETEPTR(mpNameLb);
    DELETEPTR(mpLocationLb);
    DELETEPTR(mpName);
    DELETEPTR(mpLocation);
    DELETEPTR(mpBrowseBttn);
    DELETEPTR(mpDlgBttn);
    DELETEPTR(mpNameLayout);
    DELETEPTR(mpLocationLayout);
    DELETEPTR(mpLayout);
}

void NewProjectDlg::onBrowseClicked()
{
    QString widgetPath = QFileDialog::getExistingDirectory( this, tr("Select New Project Location"), "", QFileDialog::ShowDirsOnly);

    if("" != widgetPath)
    {
        mpLocation->setText(widgetPath);
    }
}

void NewProjectDlg::validateFields()
{
    if("" == mpName->text() || "" == mpLocation->text())
    {
        mpDlgBttn->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
    else
    {
        mpDlgBttn->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

QString NewProjectDlg::path()
{
    return QString("%0%1%2.wgt").arg(mpLocation->text()).arg(QDir::separator()).arg(mpName->text());
}

void NewProjectDlg::onOkClicked()
{
    // Copy the folder here
#ifdef Q_WS_MACX
    QString qsSampleWidget = QString("%0/../Resources/%1").arg(QApplication::applicationDirPath()).arg(PROJECT_TEMPLATE);
    copyFolder(qsSampleWidget, path());
#else
    QString qsSampleWidget = QString("%0%1%2").arg(QApplication::applicationDirPath()).arg(QDir::separator()).arg(PROJECT_TEMPLATE);

    qDebug() << qsSampleWidget;

    copyFolder(qsSampleWidget, path());
#endif
    // Accept the dialog
    accept();
}

void NewProjectDlg::copyFolder(QString sourceFolder, QString destFolder)
{
    QDir sourceDir(sourceFolder);
    if(!sourceDir.exists())
    {
        return;
    }

    QDir destDir(destFolder);
    if(!destDir.exists())
    {
        destDir.mkdir(destFolder);
    }

    QStringList files = sourceDir.entryList(QDir::Files);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + QDir::separator() + files[i];
        QString destName = destFolder + QDir::separator() + files[i];
        QFile::copy(srcName, destName);
    }

    files.clear();
    files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + QDir::separator() + files[i];
        QString destName = destFolder + QDir::separator() + files[i];
        copyFolder(srcName, destName);
    }
}

void MainWindow::onToolsOption()
{
    mOptionsDlg.show();
}

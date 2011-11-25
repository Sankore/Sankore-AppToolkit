#include <QApplication>
#include <QTextCodec>
#include <QDebug>

#include "gui/mainwindow.h"
#include "globalDefs.h"

void messageHandler(QtMsgType type, const char* msg)
{
    switch(type){
    case QtDebugMsg:
        qDebug() << "[Debug] " << msg;
        break;
    case QtWarningMsg:
        qDebug() << "[Warning] " << msg;
        break;
    case QtCriticalMsg:
        qDebug() << "[Critical] " << msg;
        break;
    case QtFatalMsg:
        qDebug() << "[Fatal] " << msg;
        exit(1);
        break;
    }
}

int main(int argc, char *argv[])
{
    // Install the message handler
    //qInstallMsgHandler(messageHandler);

    // Create the application
    QApplication a(argc, argv);
    a.setApplicationVersion(QString("%0").arg(APP_VERSION));
    a.setApplicationName(APP_NAME);
    a.setOrganizationName(APP_ORG);
    a.setOrganizationDomain(APP_DOMAIN);

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    // Launch the GUI
    MainWindow w;
    w.showMaximized();

    return a.exec();
}

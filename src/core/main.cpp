#include <QApplication>
#include <QTextCodec>
#include <QDebug>

#include "gui/mainwindow.h"
#include "globalDefs.h"
#include "AppSettings.h"

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

void initSettings()
{
    if(!AppSettings::settings()->contains(QString("%0/%1").arg(SETTINGSGROUP_EDITOR).arg(SETTING_TABSIZE))){
        AppSettings::settings()->setValue(QString("%0/%1").arg(SETTINGSGROUP_EDITOR).arg(SETTING_TABSIZE), QVariant(4));
    }
    if(!AppSettings::settings()->contains(QString("%0/%1").arg(SETTINGSGROUP_EDITOR).arg(SETTING_FONT))){
        AppSettings::settings()->setValue(QString("%0/%1").arg(SETTINGSGROUP_EDITOR).arg(SETTING_FONT), QVariant("Arial"));
    }
    if(!AppSettings::settings()->contains(QString("%0/%1").arg(SETTINGSGROUP_EDITOR).arg(SETTING_FONTSIZE))){
        AppSettings::settings()->setValue(QString("%0/%1").arg(SETTINGSGROUP_EDITOR).arg(SETTING_FONTSIZE), QVariant(10));
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

    initSettings();

    // Launch the GUI
    MainWindow w;
    w.showMaximized();

    return a.exec();
}

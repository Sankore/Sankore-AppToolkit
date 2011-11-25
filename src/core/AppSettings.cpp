#include "AppSettings.h"
#include "globalDefs.h"

static AppSettings* mpInstance = NULL;

AppSettings::AppSettings(const char *name):QSettings(APP_ORG, APP_NAME)
{
    setObjectName(name);
}

AppSettings::~AppSettings()
{
    DELETEPTR(mpInstance);
}

AppSettings* AppSettings::settings()
{
    if(NULL == mpInstance){
        mpInstance = new AppSettings();
    }
    return mpInstance;
}

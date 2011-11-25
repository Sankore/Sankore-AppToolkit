#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QSettings>

#define SETTINGSGROUP_EDITOR    "Editor"
#define SETTING_TABSIZE         "TabSize"
#define SETTING_FONT            "Font"
#define SETTING_FONTSIZE        "FontSize"

class AppSettings : public QSettings
{
public:
    AppSettings(const char* name="AppSettings");
    ~AppSettings();

    static AppSettings* settings();
};

#endif // APPSETTINGS_H

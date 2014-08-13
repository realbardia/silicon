#ifndef PERCONF_H
#define PERCONF_H

#include <QApplication>
#include <QList>
#include <QDir>
#include <QFile>
#include <QProcess>
#include <limits.h>

#include <sdatabase.h>
#include "sdefaults.h"


class perConf : public SDataBase
{
public:
    perConf();
    perConf(const QString & path);
    ~perConf();

    void setPath(const QString & path);
    void checkFileConf(const QString & path);

    QString readOption(const int & type);
    void setOption(const int & type,const QString & stt);
    void setOptionDefault(const int & type);
    QString findHeadStr(const int & type);

    enum type {GENERAL
                  ,WIDTH
                  ,HEIGHT
                  ,REMOTE_ARGUMENT
                  ,ACTIVE_PLUGINS
                  ,DEACTIVE_PLUGINS
                  ,TEMP_ADDRESS

              ,APPEARANCE
                  ,CURRENT_THEME
                  ,MENU_ANIMATION
                  ,SEARCHBAR_ANIMATION
                  ,SEARCHING_ANIMATION
                  ,TABBAR_ANIMATION
                  ,TOOLBAR_ANIMATION
                  ,VIEW_MODE
                  ,GRAPHICS_SYSTEM
                  ,WIDGET_STYLE
                  ,GRADIANT_BACK
                  ,COLORIZED_MENU
                  ,LANGUAGE

              ,COLORS
                  ,WINDOW
                  ,WINDOW_TEXT
                  ,BASE
                  ,BASE_TEXT
                  ,BUTTON_TEXT
                  ,BUTTON
                  ,BRIGHT_TEXT
                  ,DARK
                  ,HIGHLIGHT
                  ,HIGHLIGHT_TEXT
                  ,LIGHT
                  ,LINK
                  ,LINK_VISITED
                  ,MID
                  ,MIDLIGHT
                  ,SHADOW

              ,APPLICATIONS
                  ,FAVORITES
                  ,SAPPS_WINDOW
                  ,SAPPS_WINDOW_TEXT
                  ,SAPPS_BASE
                  ,SAPPS_BASE_TEXT
                  ,SAPPS_BUTTON_TEXT
                  ,SAPPS_BUTTON
                  ,SAPPS_BRIGHT_TEXT
                  ,SAPPS_DARK
                  ,SAPPS_HIGHLIGHT
                  ,SAPPS_HIGHLIGHT_TEXT
                  ,SAPPS_LIGHT
                  ,SAPPS_LINK
                  ,SAPPS_LINK_VISITED
                  ,SAPPS_MID
                  ,SAPPS_MIDLIGHT
                  ,SAPPS_SHADOW

              ,BEFORE

              ,END};

private:
    void setup();
    void setupHeads();


    enum UbuntuTheme{
        Ambiance,
        Radiance,
        Adwaita,
        Other
    };

    UbuntuTheme ubuntuTheme();

private:
    SDefaults defaults;
    QList<QString> optionStr;
    QList<QString> defaultsStr;
};

#endif // PERCONF_H

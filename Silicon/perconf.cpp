#include "perconf.h"

#include <SDesktopFunctions>
#include <QtSystemInfo/QtSystemInfo.h>

PerConf::PerConf() : SDataBase()
{
    setPath( defaults.Personal_Configs_file );
}

PerConf::PerConf(const QString & path) : SDataBase()
{
    setPath(path);
}

PerConf::~PerConf()
{
}

void PerConf::setPath(const QString & path)
{
    checkFileConf(path);
    setDataBase(path);
    setup();
}

void PerConf::checkFileConf(const QString & path)
{
    if(QFile::exists( defaults.Share_Configs_file ) && !QFile::exists(path))
    {
        QString app("cp");
        QStringList arguments;
        arguments<< defaults.Share_Configs_file << path;

        QProcess *pr=new QProcess();
        pr->start(app,arguments);
        pr->waitForFinished(INT_MAX);
        delete pr;
    }
}

void PerConf::setup()
{
    optionStr.clear();
    defaultsStr.clear();
    for(int i=0;i<PerConf::END;i++)
    {
        optionStr<<"";
        defaultsStr<<"";
    }

    QtSystemInfo sysInfo;

    QString desktop;
#ifdef Q_OS_LINUX
    switch( static_cast<int>(SDesktopFunctions::desktopType()) )
    {
    case SDesktopFunctions::KDE :
        desktop = "kde";
        break;

    case SDesktopFunctions::Gnome :
        desktop = "gnome";
        break;

    case SDesktopFunctions::Ubuntu :
        desktop = "ubuntu" ;
        break;

    case SDesktopFunctions::Xfce :
        desktop = "xfce" ;
        break;

    case SDesktopFunctions::Unknown :
        desktop = "unknown" ;
        break;
    }
#else
#ifdef Q_OS_WIN32
    desktop = "win32";
#else
#ifdef Q_OS_MAC
    desktop = "mac";
#endif
#endif
#endif


    optionStr.replace(PerConf::GENERAL,"General_" + sysInfo.systemName().toLower() + "_" + desktop );
    optionStr.replace(PerConf::WIDTH,"width");
    optionStr.replace(PerConf::HEIGHT,"height");
    optionStr.replace(PerConf::REMOTE_ARGUMENT,"remote_argument");
    optionStr.replace(PerConf::ACTIVE_PLUGINS,"active_plugins");
    optionStr.replace(PerConf::DEACTIVE_PLUGINS,"deactive_plugins");
    optionStr.replace(PerConf::TEMP_ADDRESS,"temp_address");

    optionStr.replace(PerConf::APPEARANCE,"Appearance_" + sysInfo.systemName().toLower() + "_" + desktop );
    optionStr.replace(PerConf::CURRENT_THEME,"theme");
    optionStr.replace(PerConf::MENU_ANIMATION,"menu_animation");
    optionStr.replace(PerConf::SEARCHBAR_ANIMATION,"searchbar_animation");
    optionStr.replace(PerConf::SEARCHING_ANIMATION,"searching_animation");
    optionStr.replace(PerConf::TABBAR_ANIMATION,"tabbar_animation");
    optionStr.replace(PerConf::TOOLBAR_ANIMATION,"toolbar_animation");
    optionStr.replace(PerConf::VIEW_MODE,"view_mode");
    optionStr.replace(PerConf::GRAPHICS_SYSTEM,"graphics_system");
    optionStr.replace(PerConf::WIDGET_STYLE,"widget_style");
    optionStr.replace(PerConf::GRADIANT_BACK,"gradiant_background");
    optionStr.replace(PerConf::COLORIZED_MENU,"colorized_menu");
    optionStr.replace(PerConf::LANGUAGE,"language");

    optionStr.replace(PerConf::COLORS,"Colors");
    optionStr.replace(PerConf::WINDOW,"Window");
    optionStr.replace(PerConf::WINDOW_TEXT,"Window_text");
    optionStr.replace(PerConf::BASE,"Base");
    optionStr.replace(PerConf::BASE_TEXT,"Base_text");
    optionStr.replace(PerConf::BUTTON_TEXT,"Button_text");
    optionStr.replace(PerConf::BUTTON,"Button");
    optionStr.replace(PerConf::BRIGHT_TEXT,"Bright_text");
    optionStr.replace(PerConf::DARK,"Dark");
    optionStr.replace(PerConf::HIGHLIGHT,"Highlight");
    optionStr.replace(PerConf::HIGHLIGHT_TEXT,"Highlight_text");
    optionStr.replace(PerConf::LIGHT,"Light");
    optionStr.replace(PerConf::LINK,"Link");
    optionStr.replace(PerConf::LINK_VISITED,"Link_visited");
    optionStr.replace(PerConf::MID,"Mid");
    optionStr.replace(PerConf::MIDLIGHT,"Midlight");
    optionStr.replace(PerConf::SHADOW,"Shadow");

    optionStr.replace(PerConf::APPLICATIONS,"Applications");
    optionStr.replace(PerConf::FAVORITES,"Favorites");
    optionStr.replace(PerConf::SAPPS_WINDOW,"Window");
    optionStr.replace(PerConf::SAPPS_WINDOW_TEXT,"Window_text");
    optionStr.replace(PerConf::SAPPS_BASE,"Base");
    optionStr.replace(PerConf::SAPPS_BASE_TEXT,"Base_text");
    optionStr.replace(PerConf::SAPPS_BUTTON_TEXT,"Button_text");
    optionStr.replace(PerConf::SAPPS_BUTTON,"Button");
    optionStr.replace(PerConf::SAPPS_BRIGHT_TEXT,"Bright_text");
    optionStr.replace(PerConf::SAPPS_DARK,"Dark");
    optionStr.replace(PerConf::SAPPS_HIGHLIGHT,"Highlight");
    optionStr.replace(PerConf::SAPPS_HIGHLIGHT_TEXT,"Highlight_text");
    optionStr.replace(PerConf::SAPPS_LIGHT,"Light");
    optionStr.replace(PerConf::SAPPS_LINK,"Link");
    optionStr.replace(PerConf::SAPPS_LINK_VISITED,"Link_visited");
    optionStr.replace(PerConf::SAPPS_MID,"Mid");
    optionStr.replace(PerConf::SAPPS_MIDLIGHT,"Midlight");
    optionStr.replace(PerConf::SAPPS_SHADOW,"Shadow");

    optionStr.replace(PerConf::BEFORE,"Before");



    defaultsStr.replace(PerConf::GENERAL,"");
    defaultsStr.replace(PerConf::WIDTH,"1000");
    defaultsStr.replace(PerConf::HEIGHT,"573");
    defaultsStr.replace(PerConf::REMOTE_ARGUMENT,"");
    defaultsStr.replace(PerConf::ACTIVE_PLUGINS,"");
    defaultsStr.replace(PerConf::DEACTIVE_PLUGINS,"");
    defaultsStr.replace(PerConf::TEMP_ADDRESS,QDir::tempPath());

    defaultsStr.replace(PerConf::APPEARANCE,"");

#ifdef Q_OS_LINUX

    // If system is Ubuntu 10.04 or Greater, Fedora 15 or ...
    if(
            SDesktopFunctions::desktopType() == SDesktopFunctions::Ubuntu &&
            sysInfo.systemName().toLower() == "ubuntu"                    &&
            sysInfo.systemVersion().toDouble() >= 10.04
      )
    {
        switch( static_cast<int>(ubuntuTheme()) )
        {
        case PerConf::Ambiance :
            defaultsStr.replace(PerConf::CURRENT_THEME,"Ambiance-Dream");
            break;

        case PerConf::Radiance :
            defaultsStr.replace(PerConf::CURRENT_THEME,"Radiance-Dream");
            break;

        case PerConf::Other :
            defaultsStr.replace(PerConf::CURRENT_THEME,"Dream");
            break;
        }
    }
    else if(
            SDesktopFunctions::desktopType() == SDesktopFunctions::Gnome
      )
        defaultsStr.replace(PerConf::CURRENT_THEME,"Adwaita-Dream");
    else
#endif
        defaultsStr.replace(PerConf::CURRENT_THEME,"Dream");
    // End if
    defaultsStr.replace(PerConf::MENU_ANIMATION,"false");
    defaultsStr.replace(PerConf::SEARCHBAR_ANIMATION,"true");
    defaultsStr.replace(PerConf::SEARCHING_ANIMATION,"true");
    defaultsStr.replace(PerConf::TABBAR_ANIMATION,"true");
    defaultsStr.replace(PerConf::TOOLBAR_ANIMATION,"true");
    defaultsStr.replace(PerConf::VIEW_MODE,"list");
    defaultsStr.replace(PerConf::GRAPHICS_SYSTEM,"raster");
    defaultsStr.replace(PerConf::WIDGET_STYLE,"");
#ifdef Q_OS_WIN32
    defaultsStr.replace(perConf::GRADIANT_BACK,"true");
#else
    defaultsStr.replace(PerConf::GRADIANT_BACK,"false");
#endif
    defaultsStr.replace(PerConf::COLORIZED_MENU,"false");
    defaultsStr.replace(PerConf::LANGUAGE,"english");

    defaultsStr.replace(PerConf::COLORS,"");
    defaultsStr.replace(PerConf::WINDOW,"");
    defaultsStr.replace(PerConf::WINDOW_TEXT,"");
    defaultsStr.replace(PerConf::BASE,"");
    defaultsStr.replace(PerConf::BASE_TEXT,"");
    defaultsStr.replace(PerConf::BUTTON_TEXT,"");
    defaultsStr.replace(PerConf::BUTTON,"");
    defaultsStr.replace(PerConf::BRIGHT_TEXT,"");
    defaultsStr.replace(PerConf::DARK,"");
    defaultsStr.replace(PerConf::HIGHLIGHT,"");
    defaultsStr.replace(PerConf::HIGHLIGHT_TEXT,"");
    defaultsStr.replace(PerConf::LIGHT,"");
    defaultsStr.replace(PerConf::LINK,"");
    defaultsStr.replace(PerConf::LINK_VISITED,"");
    defaultsStr.replace(PerConf::MID,"");
    defaultsStr.replace(PerConf::MIDLIGHT,"");
    defaultsStr.replace(PerConf::SHADOW,"");

    defaultsStr.replace(PerConf::APPLICATIONS,"");
    defaultsStr.replace(PerConf::FAVORITES,"DataDisc,AudioDisc,ScriptRunner,Mounter,CopyDisc,Tagarg");
    defaultsStr.replace(PerConf::SAPPS_WINDOW,"");
    defaultsStr.replace(PerConf::SAPPS_WINDOW_TEXT,"");
    defaultsStr.replace(PerConf::SAPPS_BASE,"");
    defaultsStr.replace(PerConf::SAPPS_BASE_TEXT,"");
    defaultsStr.replace(PerConf::SAPPS_BUTTON_TEXT,"");
    defaultsStr.replace(PerConf::SAPPS_BUTTON,"");
    defaultsStr.replace(PerConf::SAPPS_BRIGHT_TEXT,"");
    defaultsStr.replace(PerConf::SAPPS_DARK,"");
    defaultsStr.replace(PerConf::SAPPS_HIGHLIGHT,"");
    defaultsStr.replace(PerConf::SAPPS_HIGHLIGHT_TEXT,"");
    defaultsStr.replace(PerConf::SAPPS_LIGHT,"");
    defaultsStr.replace(PerConf::SAPPS_LINK,"");
    defaultsStr.replace(PerConf::SAPPS_LINK_VISITED,"");
    defaultsStr.replace(PerConf::SAPPS_MID,"");
    defaultsStr.replace(PerConf::SAPPS_MIDLIGHT,"");
    defaultsStr.replace(PerConf::SAPPS_SHADOW,"");

    defaultsStr.replace(PerConf::BEFORE,"");


    this->setupHeads();
}

void PerConf::setupHeads()
{
    this->addHead(optionStr.at(PerConf::GENERAL));
    this->addHead(optionStr.at(PerConf::APPEARANCE));
    this->addHead(optionStr.at(PerConf::COLORS));
    this->addHead(optionStr.at(PerConf::APPLICATIONS));
    this->addHead(optionStr.at(PerConf::BEFORE));
}


void PerConf::setOptionDefault(const int & type)
{
    QString headStr=this->findHeadStr(type);
    QString childStr=optionStr.at(type);

    if(!this->checkChildExist(headStr,childStr))
        this->addChild(headStr,childStr);

    this->set(headStr,childStr,defaultsStr.at(type));
}

void PerConf::setOption(const int & type,const QString & stt)
{
    QString headStr=this->findHeadStr(type);
    QString childStr=optionStr.at(type);

    if(!this->checkChildExist(headStr,childStr))
        this->addChild(headStr,childStr);

    this->set(headStr,childStr,stt);
}

QString PerConf::readOption(const int & type)
{
    QString headStr=this->findHeadStr(type);
    QString childStr=optionStr.at(type);

    if(!this->checkChildExist(headStr,childStr))
        this->setOptionDefault(type);

    return this->read(headStr,childStr);
}

QString PerConf::findHeadStr(const int & type)
{
    if(this->BEFORE<type)
        return optionStr.at(this->BEFORE);

    else if(this->APPLICATIONS<type)
        return optionStr.at(this->APPLICATIONS);

    else if(this->COLORS<type)
        return optionStr.at(this->COLORS);

    else if(this->APPEARANCE<type)
        return optionStr.at(this->APPEARANCE);

    else //if(this->GENERAL<type)
        return optionStr.at(this->GENERAL);

}

PerConf::UbuntuTheme PerConf::ubuntuTheme()
{
    QString path = QDir::homePath() + "/.gconf/desktop/gnome/interface/%gconf.xml";
    QFile   file( path );

    if( !file.exists() || !file.open(QFile::ReadOnly) )
        return PerConf::Ambiance;

    QString str = file.readAll();
    if( str.contains( "<stringvalue>Ambiance</stringvalue>" , Qt::CaseInsensitive ) )
        return PerConf::Ambiance;
    if( str.contains( "<stringvalue>Radiance</stringvalue>" , Qt::CaseInsensitive ) )
        return PerConf::Radiance;

    return PerConf::Other;
}

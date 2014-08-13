#include "perconf.h"

#include <SDesktopFunctions>
#include <QtSystemInfo/QtSystemInfo.h>

perConf::perConf() : SDataBase()
{
    setPath( defaults.Personal_Configs_file );
}

perConf::perConf(const QString & path) : SDataBase()
{
    setPath(path);
}

perConf::~perConf()
{
}

void perConf::setPath(const QString & path)
{
    checkFileConf(path);
    setDataBase(path);
    setup();
}

void perConf::checkFileConf(const QString & path)
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

void perConf::setup()
{
    optionStr.clear();
    defaultsStr.clear();
    for(int i=0;i<perConf::END;i++)
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


    optionStr.replace(perConf::GENERAL,"General_" + sysInfo.systemName().toLower() + "_" + desktop );
    optionStr.replace(perConf::WIDTH,"width");
    optionStr.replace(perConf::HEIGHT,"height");
    optionStr.replace(perConf::REMOTE_ARGUMENT,"remote_argument");
    optionStr.replace(perConf::ACTIVE_PLUGINS,"active_plugins");
    optionStr.replace(perConf::DEACTIVE_PLUGINS,"deactive_plugins");
    optionStr.replace(perConf::TEMP_ADDRESS,"temp_address");

    optionStr.replace(perConf::APPEARANCE,"Appearance_" + sysInfo.systemName().toLower() + "_" + desktop );
    optionStr.replace(perConf::CURRENT_THEME,"theme");
    optionStr.replace(perConf::MENU_ANIMATION,"menu_animation");
    optionStr.replace(perConf::SEARCHBAR_ANIMATION,"searchbar_animation");
    optionStr.replace(perConf::SEARCHING_ANIMATION,"searching_animation");
    optionStr.replace(perConf::TABBAR_ANIMATION,"tabbar_animation");
    optionStr.replace(perConf::TOOLBAR_ANIMATION,"toolbar_animation");
    optionStr.replace(perConf::VIEW_MODE,"view_mode");
    optionStr.replace(perConf::GRAPHICS_SYSTEM,"graphics_system");
    optionStr.replace(perConf::WIDGET_STYLE,"widget_style");
    optionStr.replace(perConf::GRADIANT_BACK,"gradiant_background");
    optionStr.replace(perConf::COLORIZED_MENU,"colorized_menu");
    optionStr.replace(perConf::LANGUAGE,"language");

    optionStr.replace(perConf::COLORS,"Colors");
    optionStr.replace(perConf::WINDOW,"Window");
    optionStr.replace(perConf::WINDOW_TEXT,"Window_text");
    optionStr.replace(perConf::BASE,"Base");
    optionStr.replace(perConf::BASE_TEXT,"Base_text");
    optionStr.replace(perConf::BUTTON_TEXT,"Button_text");
    optionStr.replace(perConf::BUTTON,"Button");
    optionStr.replace(perConf::BRIGHT_TEXT,"Bright_text");
    optionStr.replace(perConf::DARK,"Dark");
    optionStr.replace(perConf::HIGHLIGHT,"Highlight");
    optionStr.replace(perConf::HIGHLIGHT_TEXT,"Highlight_text");
    optionStr.replace(perConf::LIGHT,"Light");
    optionStr.replace(perConf::LINK,"Link");
    optionStr.replace(perConf::LINK_VISITED,"Link_visited");
    optionStr.replace(perConf::MID,"Mid");
    optionStr.replace(perConf::MIDLIGHT,"Midlight");
    optionStr.replace(perConf::SHADOW,"Shadow");

    optionStr.replace(perConf::APPLICATIONS,"Applications");
    optionStr.replace(perConf::FAVORITES,"Favorites");
    optionStr.replace(perConf::SAPPS_WINDOW,"Window");
    optionStr.replace(perConf::SAPPS_WINDOW_TEXT,"Window_text");
    optionStr.replace(perConf::SAPPS_BASE,"Base");
    optionStr.replace(perConf::SAPPS_BASE_TEXT,"Base_text");
    optionStr.replace(perConf::SAPPS_BUTTON_TEXT,"Button_text");
    optionStr.replace(perConf::SAPPS_BUTTON,"Button");
    optionStr.replace(perConf::SAPPS_BRIGHT_TEXT,"Bright_text");
    optionStr.replace(perConf::SAPPS_DARK,"Dark");
    optionStr.replace(perConf::SAPPS_HIGHLIGHT,"Highlight");
    optionStr.replace(perConf::SAPPS_HIGHLIGHT_TEXT,"Highlight_text");
    optionStr.replace(perConf::SAPPS_LIGHT,"Light");
    optionStr.replace(perConf::SAPPS_LINK,"Link");
    optionStr.replace(perConf::SAPPS_LINK_VISITED,"Link_visited");
    optionStr.replace(perConf::SAPPS_MID,"Mid");
    optionStr.replace(perConf::SAPPS_MIDLIGHT,"Midlight");
    optionStr.replace(perConf::SAPPS_SHADOW,"Shadow");

    optionStr.replace(perConf::BEFORE,"Before");



    defaultsStr.replace(perConf::GENERAL,"");
    defaultsStr.replace(perConf::WIDTH,"1000");
    defaultsStr.replace(perConf::HEIGHT,"573");
    defaultsStr.replace(perConf::REMOTE_ARGUMENT,"");
    defaultsStr.replace(perConf::ACTIVE_PLUGINS,"");
    defaultsStr.replace(perConf::DEACTIVE_PLUGINS,"");
    defaultsStr.replace(perConf::TEMP_ADDRESS,QDir::tempPath());

    defaultsStr.replace(perConf::APPEARANCE,"");

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
        case perConf::Ambiance :
            defaultsStr.replace(perConf::CURRENT_THEME,"AmbianceDream");
            break;

        case perConf::Radiance :
            defaultsStr.replace(perConf::CURRENT_THEME,"RadianceDream");
            break;

        case perConf::Other :
            defaultsStr.replace(perConf::CURRENT_THEME,"Dream");
            break;
        }
    }
    else if(
            SDesktopFunctions::desktopType() == SDesktopFunctions::Gnome
      )
        defaultsStr.replace(perConf::CURRENT_THEME,"AdwaitaDream");
    else
#endif
        defaultsStr.replace(perConf::CURRENT_THEME,"Dream");
    // End if
    defaultsStr.replace(perConf::MENU_ANIMATION,"false");
    defaultsStr.replace(perConf::SEARCHBAR_ANIMATION,"true");
    defaultsStr.replace(perConf::SEARCHING_ANIMATION,"true");
    defaultsStr.replace(perConf::TABBAR_ANIMATION,"true");
    defaultsStr.replace(perConf::TOOLBAR_ANIMATION,"true");
    defaultsStr.replace(perConf::VIEW_MODE,"list");
    defaultsStr.replace(perConf::GRAPHICS_SYSTEM,"raster");
    defaultsStr.replace(perConf::WIDGET_STYLE,"");
#ifdef Q_OS_WIN32
    defaultsStr.replace(perConf::GRADIANT_BACK,"true");
#else
    defaultsStr.replace(perConf::GRADIANT_BACK,"false");
#endif
    defaultsStr.replace(perConf::COLORIZED_MENU,"false");
    defaultsStr.replace(perConf::LANGUAGE,"english");

    defaultsStr.replace(perConf::COLORS,"");
    defaultsStr.replace(perConf::WINDOW,"");
    defaultsStr.replace(perConf::WINDOW_TEXT,"");
    defaultsStr.replace(perConf::BASE,"");
    defaultsStr.replace(perConf::BASE_TEXT,"");
    defaultsStr.replace(perConf::BUTTON_TEXT,"");
    defaultsStr.replace(perConf::BUTTON,"");
    defaultsStr.replace(perConf::BRIGHT_TEXT,"");
    defaultsStr.replace(perConf::DARK,"");
    defaultsStr.replace(perConf::HIGHLIGHT,"");
    defaultsStr.replace(perConf::HIGHLIGHT_TEXT,"");
    defaultsStr.replace(perConf::LIGHT,"");
    defaultsStr.replace(perConf::LINK,"");
    defaultsStr.replace(perConf::LINK_VISITED,"");
    defaultsStr.replace(perConf::MID,"");
    defaultsStr.replace(perConf::MIDLIGHT,"");
    defaultsStr.replace(perConf::SHADOW,"");

    defaultsStr.replace(perConf::APPLICATIONS,"");
    defaultsStr.replace(perConf::FAVORITES,"DataDisc,AudioDisc,ScriptRunner,Mounter,CopyDisc,Tagarg");
    defaultsStr.replace(perConf::SAPPS_WINDOW,"");
    defaultsStr.replace(perConf::SAPPS_WINDOW_TEXT,"");
    defaultsStr.replace(perConf::SAPPS_BASE,"");
    defaultsStr.replace(perConf::SAPPS_BASE_TEXT,"");
    defaultsStr.replace(perConf::SAPPS_BUTTON_TEXT,"");
    defaultsStr.replace(perConf::SAPPS_BUTTON,"");
    defaultsStr.replace(perConf::SAPPS_BRIGHT_TEXT,"");
    defaultsStr.replace(perConf::SAPPS_DARK,"");
    defaultsStr.replace(perConf::SAPPS_HIGHLIGHT,"");
    defaultsStr.replace(perConf::SAPPS_HIGHLIGHT_TEXT,"");
    defaultsStr.replace(perConf::SAPPS_LIGHT,"");
    defaultsStr.replace(perConf::SAPPS_LINK,"");
    defaultsStr.replace(perConf::SAPPS_LINK_VISITED,"");
    defaultsStr.replace(perConf::SAPPS_MID,"");
    defaultsStr.replace(perConf::SAPPS_MIDLIGHT,"");
    defaultsStr.replace(perConf::SAPPS_SHADOW,"");

    defaultsStr.replace(perConf::BEFORE,"");


    this->setupHeads();
}

void perConf::setupHeads()
{
    this->addHead(optionStr.at(perConf::GENERAL));
    this->addHead(optionStr.at(perConf::APPEARANCE));
    this->addHead(optionStr.at(perConf::COLORS));
    this->addHead(optionStr.at(perConf::APPLICATIONS));
    this->addHead(optionStr.at(perConf::BEFORE));
}


void perConf::setOptionDefault(const int & type)
{
    QString headStr=this->findHeadStr(type);
    QString childStr=optionStr.at(type);

    if(!this->checkChildExist(headStr,childStr))
        this->addChild(headStr,childStr);

    this->set(headStr,childStr,defaultsStr.at(type));
}

void perConf::setOption(const int & type,const QString & stt)
{
    QString headStr=this->findHeadStr(type);
    QString childStr=optionStr.at(type);

    if(!this->checkChildExist(headStr,childStr))
        this->addChild(headStr,childStr);

    this->set(headStr,childStr,stt);
}

QString perConf::readOption(const int & type)
{
    QString headStr=this->findHeadStr(type);
    QString childStr=optionStr.at(type);

    if(!this->checkChildExist(headStr,childStr))
        this->setOptionDefault(type);

    return this->read(headStr,childStr);
}

QString perConf::findHeadStr(const int & type)
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

perConf::UbuntuTheme perConf::ubuntuTheme()
{
    QString path = QDir::homePath() + "/.gconf/desktop/gnome/interface/%gconf.xml";
    QFile   file( path );

    if( !file.exists() || !file.open(QFile::ReadOnly) )
        return perConf::Ambiance;

    QString str = file.readAll();
    if( str.contains( "<stringvalue>Ambiance</stringvalue>" , Qt::CaseInsensitive ) )
        return perConf::Ambiance;
    if( str.contains( "<stringvalue>Radiance</stringvalue>" , Qt::CaseInsensitive ) )
        return perConf::Radiance;

    return perConf::Other;
}

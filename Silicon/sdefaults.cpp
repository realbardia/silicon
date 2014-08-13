#include "sdefaults.h"

#include <SDesktopFunctions>
#include <SMasterIcons>

SDefaults::SDefaults()
{
    QDir current = QDir( SDesktopFunctions::binaryPath() );
        current.cdUp();
        current.cdUp();

    Home_Path = QDir( QDir::homePath()+"/.config/silicon" ).path();
    Share_Path = QDir( current.path() + "/share/silicon" ).path();

    Personal_Theme_Directory_Path = QDir( Home_Path+"/themes" ).path();
    Personal_Icons_Directory_Path = QDir( Home_Path+"/icons" ).path();
    Share_Theme_Directory_Path = QDir( Share_Path+"/themes" ).path();
    Share_Icons_Directory_Path = QDir( current.path() + "/share/icons" ).path();
    Default_Theme = QDir( "Dream" ).path();

    if ( QDir( current.path() + "/lib32" ).exists() && sizeof(void*) == 4 )
        Silicon_Libs_Directory = QDir( current.path() + "/lib32" ).path();
    else if( QDir( current.path() + "/lib64" ).exists() && sizeof(void*) == 8 )
        Silicon_Libs_Directory = QDir( current.path() + "/lib64" ).path();
    else
        Silicon_Libs_Directory = QDir( current.path() + "/lib" ).path();

    Silicon_Application_Directory = QDir( Silicon_Libs_Directory +"/silicon/apps" ).path();
    Silicon_Plugins_Directory = QDir( Silicon_Libs_Directory +"/silicon/plugins" ).path();

    Personal_Configs_file = QDir( Home_Path+"/config" ).path();
    Share_Configs_file = QDir( Share_Path+"/config" ).path();

    Default_Theme_Name = "theme.css";

    Desktop_Environment = SDesktopFunctions::findEnvVariable( "DESKTOP_SESSION" ) ;

    this->makeDirectorys();
}

QIcon SDefaults::catIcons( SAboutData::Type type )
{
    switch( static_cast<int>(type) )
    {
    case SAboutData::DVD :
        return SMasterIcons::icon( QSize(48,48) , "media-optical-dvd.png" );
        break;

    case SAboutData::CD :
        return SMasterIcons::icon( QSize(48,48) , "media-optical.png" );
        break;

    case SAboutData::BluRay :
        return SMasterIcons::icon( QSize(48,48) , "media-optical-blu-ray.png" );
        break;

    case SAboutData::Image :
        return SMasterIcons::icon( QSize(48,48) , "application-x-cd-image.png" );
        break;

    case SAboutData::Photo :
        return SMasterIcons::icon( QSize(48,48) , "image-x-generic.png" );
        break;

    case SAboutData::Sound :
        return SMasterIcons::icon( QSize(48,48) , "audio-x-generic.png" );
        break;

    case SAboutData::Video :
        return SMasterIcons::icon( QSize(48,48) , "applications-multimedia.png" );
        break;

    case SAboutData::Tools :
        return SMasterIcons::icon( QSize(48,48) , "preferences-system.png" );
        break;

    case SAboutData::Other :
        return SMasterIcons::icon( QSize(48,48) , "arrow-right.png" );
        break;
    }

    return QIcon();
}

QString SDefaults::catGroup( SAboutData::Type type )
{
    switch( static_cast<int>(type) )
    {
    case SAboutData::DVD :
        return QObject::tr("DVD");
        break;

    case SAboutData::CD :
        return QObject::tr("CD");
        break;

    case SAboutData::BluRay :
        return QObject::tr("Bluray");
        break;

    case SAboutData::Image :
        return QObject::tr("Image");
        break;

    case SAboutData::Photo :
        return QObject::tr("Picture");
        break;

    case SAboutData::Sound :
        return QObject::tr("Sound");
        break;

    case SAboutData::Video :
        return QObject::tr("Video");
        break;

    case SAboutData::Tools :
        return QObject::tr("Tools");
        break;

    case SAboutData::Other :
        return QObject::tr("Other");
        break;
    }

    return QObject::tr("Unknown");
}

void SDefaults::makeDirectorys()
{
    QDir().mkpath( Home_Path );
    QDir().mkpath( Share_Path );
    QDir().mkpath( Personal_Theme_Directory_Path );
}

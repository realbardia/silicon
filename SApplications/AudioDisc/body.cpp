#include "audiodisc.h"

#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QPixmap>

#include <SAboutData>
#include <Silicon>

SAboutData about_obj;

extern "C" int application( const QVariantList & args )       // Required Function <-----
{
    AudioDisc *app = new AudioDisc( about_obj );
        app->addRuntimeArgs( args );

    return 0;
}

extern "C" SAboutData about()           // Required Function <-----
{
    SAuthorInfo bardia;
        bardia.setFirstName( "Bardia" );
        bardia.setLastName( "Daneshvar" );
        bardia.setNickName( "Bardiax" );
        bardia.setAuthorIcon( QIcon(":/files/Pixs/authors/bardia-icon.jpg") );
        bardia.setAvatarPixmap( QPixmap(":/files/Pixs/authors/bardia-av.jpg") );
        bardia.setBornDate( QDate( 1990 , 9 , 18 ) );
        bardia.setHomePage( "http://www.HyperRay.net" );
        bardia.setBlog( "http://Bardiax.wordpress.com" );
        bardia.setMail( "bardia.daneshvar@gmail.com" );
        bardia.setAIM( "bardiax" );
        bardia.setGTalkIM( "bardia.daneshvar" );
        bardia.setMSN( "realbardia@live.com" );
        bardia.setSkypeIM( "bardiax" );
        bardia.setYahooIM( "bard1ax" );
        bardia.setRollList( QStringList()<<"Maintainer"<<"Developer" );
        bardia.setProgramingLanguagesList( QStringList()<<"C"<<"C++"<<"C#"<<"Qt"<<"HTML"<<"Css"<<"JavaScript" );
        bardia.setProjectsLists( QStringList() );

    about_obj.setName( "AudioDisc" );
    about_obj.setTranslatedName( QObject::tr("Audio Disc") );
    about_obj.setVersion( "0.8" );
    about_obj.setType( QList<SAboutData::Type>()<< SAboutData::CD << SAboutData::DVD << SAboutData::Sound );
    about_obj.setIcon( QIcon( ":/AudioDisc/icons/audio-disc.png" ) );
    about_obj.setMultiRun( true );
    about_obj.setPixmap( QPixmap(":/AudioDisc/icons/pic.png") );
    about_obj.setLicense( Silicon::GPLLicense() );
    about_obj.setAuthors( QList<SAuthorInfo>()<<bardia );
    about_obj.setDescription( QString() + " <b>" + QObject::tr("Description :") + "</b><br /> "
                              + QObject::tr("AudioDisc Application create Audio discs on CDs ,DVDs or Blu-Rays.") + "<br /><br /> "
                              + QObject::tr("To burn an audio disc, this application convert your files to wav format using Silicon Converter System and then burning tracks on the disc") + "<br /><br /> * "
                              + QObject::tr("You can choose your favorite audio burner from plugin system.") + "<br /><br />"
                              + " <b>" + QObject::tr("Version :") + "</b> 0.8 <br /><br />"
                              + " <b>" + QObject::tr("License :") + "</b> GPL V3 <br /><br />"
                              + " <b>" + QObject::tr("Authors :") + "</b><br />"
                              + " -> Bardia Daneshvar");

    about_obj.setCommandMap( "file1 file2 ...\t\t\tAdd to list for burning.\n\n"
                             "dev=[device address]\t\tset destination burn device to address\n\n"
                             "speed=[number]\t\tset speed to number\n\n"
                             "count=[number]\t\tset burn/create count to number\n\n"
                             "scan=[name]\t\t\tset scaned name to save in the database\n\n"
                             "wait=[true/false]\t\tif true, wait until send wait=false command and then start process.\n\n"
                             "volume=[name]\t\tset volume name to name\n\n"
                             "app_id=[app_id_string]\t\tset application id to app_id_string\n\n"
                             "biblio=[biblio_str]\t\tset biblio of the disc to biblio_str\n\n"
                             "abstract=[abstract_str]\t\tset abstract to abstract_str\n\n"
                             "copyright=[copyright_str]\t\tset CopyRight to copyright_str\n\n"
                             "system_id=[sys_id_str]\t\tset system_id to sys_id_str\n\n"
                             "publisher=[publisher_str]\t\tset publisher name to publisher_str\n\n"
                             "little-endian=[true/false]\t\tSet little endian burning or not.\n\n"
                             "-tao\t\t\tSet track at once\n\n"
                             "-dao\t\t\tSet disc at once\n\n"
                             "-eject\t\t\teject disc after burning operation complited\n\n"
                             "-dummy\t\t\tset burning type to dummy burn\n\n" );

    return about_obj;
}

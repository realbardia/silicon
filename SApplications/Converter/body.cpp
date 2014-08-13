#include "converter.h"

#include <QString>
#include <QStringList>
#include <QIcon>
#include <QPixmap>

#include <SAboutData>

SAboutData about_obj;

extern "C" int application( const QVariantList & args )       // Required Function <-----
{
    Converter *app = new Converter( about_obj );
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
        about_obj.setAuthors( QList<SAuthorInfo>()<<bardia );

    about_obj.setName( "Converter" );
    about_obj.setTranslatedName( QObject::tr("Converter") );
    about_obj.setVersion( "0.9" );
    about_obj.setType( QList<SAboutData::Type>()<< SAboutData::Tools );
    about_obj.setIcon( QIcon(":/Converter/icons/icon.png") );
    about_obj.setMultiRun( true );
    about_obj.setPixmap( QPixmap(":/Converter/icons/pic.png") );
    about_obj.setLicense( Silicon::LesserGPLLicense() );
    about_obj.setAuthors( QList<SAuthorInfo>()<<bardia );
    about_obj.setDescription( QString() + " <b>" + QObject::tr("Description :") + "</b><br /> "
                              + QObject::tr("Converter Application convert your files to another supported file formats using silicon converter system.") + "<br /><br /> "
                              + QObject::tr("Silicon Converter system is plugin base system, Means you can add supported file format, using plugin.") + "<br /><br />"
                              + " <b>" + QObject::tr("Version :") + "</b> 0.9 <br /><br />"
                              + " <b>" + QObject::tr("License :") + "</b> GPL V3 <br /><br />"
                              + " <b>" + QObject::tr("Authors :") + "</b><br />"
                              + " -> Bardia Daneshvar");

    about_obj.setCommandMap( "src=[file address]\t\tset source file to \"file address\"\n\n"
                             "dst=[file address]\t\tset destination converted file to \"file address\"\n\n"
                             "src-type=[type]\t\tset source type to \"type\"\n\n"
                             "dst-type=[type]\t\tset destination type to \"type\"\n\n"
                             "wait=[true/false]\t\tif true, wait until send wait=false command and then start process.\n\n"
                             "-silent\t\t\tDont show Ui and start burn process in the application startup.\n\n" );

    return about_obj;
}

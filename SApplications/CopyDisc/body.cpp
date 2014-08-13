#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QPixmap>
#include <QTimer>

#include <SAboutData>

#include "copydisc.h"

SAboutData about_obj;

extern "C" int application( const QVariantList & args )       // Required Function <-----
{
    CopyDisc *app = new CopyDisc( about_obj );
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

    about_obj.setName( "CopyDisc" );
    about_obj.setTranslatedName( QObject::tr("Copy Disc") );
    about_obj.setVersion( "0.9" );
    about_obj.setType( QList<SAboutData::Type>()<< SAboutData::CD << SAboutData::DVD );
    about_obj.setIcon(  QIcon(":/Copy-Disc/pics/icon.png") );
    about_obj.setPrivateStructure( false );
    about_obj.setMultiRun( true );
    about_obj.setPixmap( QPixmap(":/Copy-Disc/pics/pic.png") );
    about_obj.setLicense( Silicon::GPLLicense() );
    about_obj.setAuthors( QList<SAuthorInfo>()<<bardia );
    about_obj.setDescription( QString() + " <b>" + QObject::tr("Description :") + "</b><br /> "
                              + QObject::tr("CopyDisc Application can copy a Disc to another Disc.") + "<br /><br /> "
                              + QObject::tr("To Copy a Disc to another Disc without onfly system, Application autamatically using another applications like ImageBurner and DiscImaging.") + "<br /><br /> * "
                              + QObject::tr("If You want Burn an image to disc or Copy a Disc to Your hard, Using ImageBurner or DiscImaging.") + "<br /> * "
                              + QObject::tr("Also You can choose your favorite copy engine from plugin system.") + "<br /><br />"
                              + " <b>" + QObject::tr("Version :") + "</b> 0.9 <br /><br />"
                              + " <b>" + QObject::tr("License :") + "</b> GPL V3 <br /><br />"
                              + " <b>" + QObject::tr("Authors :") + "</b><br />"
                              + " -> Bardia Daneshvar");

    about_obj.setCommandMap( "dst=[device address]\t\tset destination burn device to address\n\n"
                             "src=[device address]\t\tset source device to address\n\n"
                             "speed=[number]\t\tset speed to number\n\n"
                             "count=[number]\t\tset burn/create count to number\n\n"
                             "scan=[name]\t\t\tset scaned name to save in the database\n\n"
                             "wait=[true/false]\t\tif true, wait until send wait=false command and then start process.\n\n"
                             "onfly=[true/false]\t\tonFly or non-onFly burn\n\n"
                             "-eject\t\t\teject disc after burning operation complited\n\n"
                             "-dummy\t\t\tset burning type to dummy burn\n\n"
                             "-silent\t\t\tDont show Ui and start burn process in the application startup.\n\n" );

    return about_obj;
}

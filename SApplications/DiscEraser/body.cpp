#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QPixmap>
#include <QTimer>

#include <SAboutData>

#include "disceraser.h"

SAboutData about_obj;

extern "C" int application( const QVariantList & args )       // Required Function <-----
{
    DiscEraser *app = new DiscEraser( about_obj );
        app->addRuntimeArgs( args );

    return 0;
}

extern "C" SAboutData about()           // Required Function <-----
{
    SAuthorInfo bardia;
        bardia.setFirstName( "Bardia" );
        bardia.setLastName( "Daneshvar" );
        bardia.setNickName( "realbardia" );
        bardia.setAuthorIcon( QIcon(":/files/Pixs/authors/bardia-icon.jpg") );
        bardia.setAvatarPixmap( QPixmap(":/files/Pixs/authors/bardia-av.jpg") );
        bardia.setBornDate( QDate( 1990 , 9 , 18 ) );
        bardia.setHomePage( "http://www.HyperRay.net" );
        bardia.setBlog( "http://realbardia.com" );
        bardia.setMail( "bardia.daneshvar@gmail.com" );
        bardia.setAIM( "bardiax" );
        bardia.setGTalkIM( "bardia.daneshvar" );
        bardia.setMSN( "realbardia@live.com" );
        bardia.setSkypeIM( "bardiax" );
        bardia.setYahooIM( "bard1ax" );
        bardia.setRollList( QStringList()<<"Maintainer"<<"Developer" );
        bardia.setProgramingLanguagesList( QStringList()<<"C"<<"C++"<<"C#"<<"Qt"<<"HTML"<<"Css"<<"JavaScript" );
        bardia.setProjectsLists( QStringList() );

    about_obj.setName( "DiscEraser" );
    about_obj.setOrganizer( "Sialan Labs" );
    about_obj.setCopyRight( "Sialan Labs" );
    about_obj.setTranslatedName( QObject::tr("Disc Eraser") );
    about_obj.setVersion( "0.9" );
    about_obj.setType( QList<SAboutData::Type>()<< SAboutData::CD << SAboutData::DVD );
    about_obj.setIcon(  QIcon(":/DiscEraser/pics/icon.png") );
    about_obj.setCompatibleFileTypes( QStringList()<<"iso"<<"nrg"<<"mdf"<<"img" );
    about_obj.setMultiRun( true );
    about_obj.setPrivateStructure( false );
    about_obj.setPixmap( QPixmap(":/DiscEraser/pics/pic.png") );
    about_obj.setLicense( Silicon::GPLLicense() );
    about_obj.setAuthors( QList<SAuthorInfo>()<<bardia );
    about_obj.setDescription( QString() + " <b>" + QObject::tr("Description:") + "</b><br /> "
                              + QObject::tr("DiscEraser Application erase your Rewritable discs.") + "<br /><br /> * "
                              + QObject::tr("You can choose your favorite disc eraser engine from plugin system.") + "<br /><br />"
                              + " <b>" + QObject::tr("Version :") + "</b> 0.9 <br /><br />"
                              + " <b>" + QObject::tr("License :") + "</b> GPL V3 <br /><br />"
                              + " <b>" + QObject::tr("Authors :") + "</b><br />"
                              + " -> Bardia Daneshvar");

    about_obj.setCommandMap( "dev=[device address]\t\tset destination burn device to address\n\n"
                             "-eject\t\t\teject disc after burning operation complited\n\n"
                             "-force\t\t\tThis forces to format dvd as new while wiping any existing data.\n\n"
                             "-lead-out\t\t\tThis relocates the lead-out next to outermost written sector.\n\n"
                             "-silent\t\t\tDont show Ui and start burn process in the application startup.\n\n" );

    return about_obj;
}

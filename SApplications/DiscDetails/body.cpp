#include <QString>
#include <QStringList>
#include <QIcon>
#include <QPixmap>

#include <SAboutData>
#include <SApplication>
#include <SPage>
#include <SDialog>

#include "discdetails.h"

SAboutData about_obj;

extern "C" int application()       // Required Function <-----
{
    SApplication *app  = new SApplication( about_obj );
    DiscDetails  *page = new DiscDetails( app );

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
        about_obj.setAuthors( QList<SAuthorInfo>()<<bardia );

    about_obj.setName( "DiscDetails" );
    about_obj.setOrganizer( "Sialan Labs" );
    about_obj.setCopyRight( "Sialan Labs" );
    about_obj.setTranslatedName( QObject::tr("Disc Details") );
    about_obj.setVersion( "0.6" );
    about_obj.setType( QList<SAboutData::Type>()<< SAboutData::CD << SAboutData::DVD );
    about_obj.setIcon( QIcon(":/plugins/DiscDetails/Pics/icon.png") );
    about_obj.setMultiRun( true );
    about_obj.setPixmap( QPixmap(":/plugins/DiscDetails/Pics/pic.png") );
    about_obj.setLicense( Silicon::LesserGPLLicense() );
    about_obj.setAuthors( QList<SAuthorInfo>()<<bardia );
    about_obj.setDescription( QString() + " <b>" + QObject::tr("Description :") + "</b><br /> "
                              + QObject::tr("DiscDetails Application showing your disc, image or database disc details.") + "<br /><br /> "
                              + QObject::tr("It's using silicon device notifier system that is a pluginable system. Means You can Change Silicon Device Notifier system engine system to hal, udisks or ... using plugins.") + "<br /><br />"
                              + " <b>" + QObject::tr("Version :") + "</b> 0.6 <br /><br />"
                              + " <b>" + QObject::tr("License :") + "</b> GPL V3 <br /><br />"
                              + " <b>" + QObject::tr("Authors :") + "</b><br />"
                              + " -> Bardia Daneshvar");

    return about_obj;
}

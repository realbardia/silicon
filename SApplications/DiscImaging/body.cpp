#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QPixmap>
#include <QTimer>

#include <SAboutData>

#include "discimaging.h"

SAboutData about_obj;

extern "C" int application( const QVariantList & args )       // Required Function <-----
{
    DiscImaging *app = new DiscImaging( about_obj );
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

    about_obj.setName( "DiscImaging" );
    about_obj.setOrganizer( "Sialan Labs" );
    about_obj.setCopyRight( "Sialan Labs" );
    about_obj.setTranslatedName( QObject::tr("Disc Imaging") );
    about_obj.setVersion( "0.9" );
    about_obj.setType( QList<SAboutData::Type>()<< SAboutData::CD << SAboutData::DVD << SAboutData::Image );
    about_obj.setIcon(  QIcon(":/DiscImaging/pics/icon.png") );
    about_obj.setPrivateStructure( false );
    about_obj.setMultiRun( true );
    about_obj.setPixmap( QPixmap(":/DiscImaging/pics/pic.png") );
    about_obj.setLicense( Silicon::GPLLicense() );
    about_obj.setAuthors( QList<SAuthorInfo>()<<bardia );
    about_obj.setDescription( QString() + " <b>" + QObject::tr("Description :") + "</b><br /> "
                              + QObject::tr("DiscImaging Application can make an image from your discs.") + "<br /><br /> * "
                              + QObject::tr("If You want Burn an image to disc or Copy a Disc to another Disc, Using ImageBurner or CopyDisc.") + "<br /> * "
                              + QObject::tr("Also You can choose your favorite disc imaging engine from plugin system.") + "<br /><br />"
                              + " <b>" + QObject::tr("Version :") + "</b> 0.9 <br /><br />"
                              + " <b>" + QObject::tr("License :") + "</b> GPL V3 <br /><br />"
                              + " <b>" + QObject::tr("Authors :") + "</b><br />"
                              + " -> Bardia Daneshvar");

    about_obj.setCommandMap( "dev=[device address]\t\tset destination burn device to address\n\n"
                             "file=[file address]\t\tset source file address to address\n\n"
                             "sectors=[from-to]\t\tset copy sectors from \"from\" sector to \"to\" sector.\n\n"
                             "count=[number]\t\tset burn/create count to number\n\n"
                             "wait=[true/false]\t\tif true, wait until send wait=false command and then start process.\n\n"
                             "-eject\t\t\teject disc after burning operation complited\n\n"
                             "-silent\t\t\tDont show Ui and start burn process in the application startup.\n\n" );

    return about_obj;
}

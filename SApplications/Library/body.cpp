#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QPixmap>

#include <SAboutData>
#include <SAuthorInfo>
#include <SApplication>
#include <SPage>

#include "libraryui.h"

SAboutData about_obj;

extern "C" int application( const QVariantList & args )       // Required Function <-----
{
    SApplication *app = new SApplication( about_obj );
    LibraryUI *page = new LibraryUI( about_obj.name() , app );
        page->addRuntimeArgs( args );

    QObject::connect( app , SIGNAL(runtimeArguments(QVariantList)) , page , SLOT(addRuntimeArgs(QVariantList)) );

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

    about_obj.setName( "Library" );
    about_obj.setTranslatedName( QObject::tr("Library") );
    about_obj.setVersion( "1.0.0" );
    about_obj.setType( QList<SAboutData::Type>()<< SAboutData::Image );
    about_obj.setCompatibleFileTypes( QStringList()<<"iso"<<"nrg"<<"mdf"<<"img" );
    about_obj.setIcon( QIcon(":/library/pics/icon.png") );
    about_obj.setMultiRun( false );
    about_obj.setPixmap( QPixmap(":/library/pics/pic.png") );
    about_obj.setLicense( Silicon::GPLLicense() );
    about_obj.setAuthors( QList<SAuthorInfo>()<<bardia );
    about_obj.setDescription( QString() + " <b>" + QObject::tr("Description :") + "</b><br /> "
                              + QObject::tr("Library Application manages your iso images in classic way.") + "<br /><br /> * "
                              + QObject::tr("We will remove this application after silicon database system maked ready.") + "<br /><br />"
                              + " <b>" + QObject::tr("Version :") + "</b> 1.0.0 <br /><br />"
                              + " <b>" + QObject::tr("License :") + "</b> GPL V3 <br /><br />"
                              + " <b>" + QObject::tr("Authors :") + "</b><br />"
                              + " -> Bardia Daneshvar");

    return about_obj;
}

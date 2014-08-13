#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QPixmap>
#include <QTimer>

#include <SAboutData>

#include "discscanner.h"

SAboutData about_obj;

extern "C" int application( const QVariantList & args )       // Required Function <-----
{
    DiscScanner *app = new DiscScanner( about_obj );
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

    about_obj.setName( "DiscScanner" );
    about_obj.setTranslatedName( QObject::tr("Disc Scanner") );
    about_obj.setVersion( "0.4" );
    about_obj.setType( QList<SAboutData::Type>()<< SAboutData::Tools );
    about_obj.setIcon( QIcon(":/DiscScanner/pics/icon.png") );
    about_obj.setMultiRun( true );
    about_obj.setPixmap( QPixmap(":/DiscScanner/pics/pic.png") );
    about_obj.setLicense( Silicon::LesserGPLLicense() );
    about_obj.setDescription( QString() + " <b>" + QObject::tr("Description:") + "</b><br /> "
                              + "<b>" + QObject::tr("Under Construction") + "</b><br /> "
                              + QObject::tr("DiscScanner Application copy information and datas of your discs and images to the Silicon DataBase.") + "<br /><br /> "
                              + QObject::tr("Silicon DataBase system keep usefull datas of your discs to database to help you easy finding and managing your Audios, Videos, Pictures and Datas.") + "<br /><br />"
                              + QObject::tr("It's under construction and will be ready soon.") + "<br /><br />"
                              + " <b>" + QObject::tr("Version :") + "</b> 0.4 <br /><br />"
                              + " <b>" + QObject::tr("License :") + "</b> GPL V3 <br /><br />"
                              + " <b>" + QObject::tr("Authors :") + "</b><br />"
                              + " -> Bardia Daneshvar");

    return about_obj;
}

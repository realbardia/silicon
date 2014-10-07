#include "scriptrunner.h"

#include <QString>
#include <QStringList>
#include <QIcon>
#include <QPixmap>

#include <SAboutData>

SAboutData about_obj;

extern "C" int application( const QVariantList & args )       // Required Function <-----
{
    ScriptRunner *app = new ScriptRunner( about_obj );
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
        about_obj.setAuthors( QList<SAuthorInfo>()<<bardia );

    about_obj.setName( "ScriptRunner" );
    about_obj.setOrganizer( "Sialan Labs" );
    about_obj.setCopyRight( "Sialan Labs" );
    about_obj.setTranslatedName( QObject::tr("Script Runner") );
    about_obj.setVersion( "0.2" );
    about_obj.setType( QList<SAboutData::Type>()<< SAboutData::Tools );
    about_obj.setIcon( QIcon(":/ScriptRunner/icons/icon.png") );
    about_obj.setMultiRun( true );
    about_obj.setPixmap( QPixmap(":/ScriptRunner/icons/pic.png") );
    about_obj.setLicense( Silicon::LesserGPLLicense() );
    about_obj.setDescription( QString() + " <b>" + QObject::tr("Description :") + "</b><br /> "
                              + "<b>" + QObject::tr("Under Construction") + "</b><br /> "
                              + QObject::tr("ScriptRunner Appliction help you to write very script and run thats.") + "<br /><br /> "
                              + QObject::tr("It's under construction and support simple commands currently.") + "<br /><br /> "
                              + " <b>" + QObject::tr("Version :") + "</b> 0.2 <br /><br />"
                              + " <b>" + QObject::tr("License :") + "</b> GPL V3 <br /><br />"
                              + " <b>" + QObject::tr("Authors :") + "</b><br />"
                              + " -> Bardia Daneshvar");

    return about_obj;
}

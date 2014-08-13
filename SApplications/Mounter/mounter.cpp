#include <QWidget>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QDir>
#include <QPixmap>
#include <QHBoxLayout>

#include <SAboutData>
#include <SApplication>
#include <SPage>
#include <Silicon>

#include "managergui.h"
#include "configure.h"
#include "perconf.h"

QString dir = Silicon::config() + "/Silicon Mounter";
QString path = dir + "/config";

MounterConf *perconf = new MounterConf( path );
ConfigureWidget *confUI = new ConfigureWidget( perconf );
SAboutData about_obj;

extern "C" int application( const QVariantList & args )
{
    if( !QDir().exists( dir ) )
        QDir().mkpath( dir );

    SApplication *app = new SApplication( about_obj );
    managerGUI *main = new managerGUI( about_obj.name() , app , path , perconf );
        main->setConfigure( confUI );
        main->addRuntimeArgs( args );

    QObject::connect( app , SIGNAL(runtimeArguments(QVariantList)) , main , SLOT(addRuntimeArgs(QVariantList)) );

    return 0;
}

extern "C" QWidget *configure()         // Optional Function <-----
{
    if( !QDir().exists( dir ) )
        QDir().mkpath( dir );

    return confUI;
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

    about_obj.setName( "Mounter" );
    about_obj.setTranslatedName( QObject::tr("Image Mounter") );
    about_obj.setVersion( "1.5.0" );
    about_obj.setType( QList<SAboutData::Type>()<< SAboutData::Image );
    about_obj.setCompatibleFileTypes( QStringList()<<"iso"<<"mdf"<<"nrg"<<"img" );
    about_obj.setIcon( QIcon(":/mounter/files/mounted.png") );
    about_obj.setMultiRun( false );
    about_obj.setPixmap( QPixmap(":/mounter/files/pic.png") );
    about_obj.setAuthors( QList<SAuthorInfo>()<<bardia );
    about_obj.setLicense( Silicon::GPLLicense() );
    about_obj.setBinaryDepends( QStringList()<< "fuseiso" );
    about_obj.setDescription( QString() + " <b>" + QObject::tr("Description :") + "</b><br /> "
                              + QObject::tr("Mounter Application helps you to Mount/Unmount your images easily.") + "<br /><br /> "
                              + QObject::tr("Mount/Unmount your disc images using drag and drop quickly.") + "<br /> * "
                              + QObject::tr("You can choose your favorite image mounter engine from plugin system.") + "<br /><br />"
                              + " <b>" + QObject::tr("Version :") + "</b> 1.5.0 <br /><br />"
                              + " <b>" + QObject::tr("License :") + "</b> GPL V3 <br /><br />"
                              + " <b>" + QObject::tr("Authors :") + "</b><br />"
                              + " -> Bardia Daneshvar");

    about_obj.setCommandMap( "file1 file2 ...\t\t\t Mount/Unmount files." );

    return about_obj;
}

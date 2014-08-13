#include <QWidget>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QPixmap>
#include <QTimer>

#include <SAboutData>
#include <SApplication>
#include <SPage>
#include <SDialog>

SAboutData about_obj;

extern "C" int application()       // Required Function <-----
{
    SApplication *sapp = new SApplication( about_obj );
    SPage *page = new SPage( "Bardia" , sapp );
    SDialog *dialog = new SDialog( page , 337 , 73 );

    QTimer::singleShot( 2500 , dialog , SLOT(hide()) );

    return 0;
}

extern "C" QWidget *configure()         // Optional Function <-----
{
    QWidget *widget = new QWidget();
    QPushButton *button = new QPushButton("Test Configure");
        button->setParent( widget );

    return widget;
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

    about_obj.setName( "Sample" );
    about_obj.setTranslatedName( QObject::tr("Sample") );
    about_obj.setVersion( "1.0.0" );
    about_obj.setType( QList<SAboutData::Type>()<< SAboutData::Video << SAboutData::Sound );
    about_obj.setIcon( QIcon(":/sample/sample-icon.png") );
    about_obj.setMultiRun( false );
    about_obj.setPixmap( QPixmap(":/sample/pix.png") );
    about_obj.setLicense( Silicon::LesserGPLLicense() );
    about_obj.setDescription( QString() + " <b>" + QObject::tr("Description") + " :</b><br />" +
                          QObject::tr(" It's a Silicon Sample Application. ;)") );

    return about_obj;
}

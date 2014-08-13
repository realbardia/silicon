#include <QApplication>
#include <QSplashScreen>
#include <QPixmap>
#include <QString>
#include <QStringList>
#include <QList>
#include <QImage>
#include <QDir>
#include <QDebug>

#include "siliconui.h"
#include "qtsingleapplication/qtsingleapplication.h"

#include <SArgumentList>
#include <SImage>
#include <SInnerMessage>
#include <SDataBase>

int main(int argc, char *argv[])
{
    // Setup Graphic system raster or native engine ===========================//

    SDataBase *db = new SDataBase( QDir::homePath() + "/.config/silicon/config" );
    if( !db->checkHeadExist("Appearance") )
        db->addHead("Appearance");
    if( !db->checkChildExist("Appearance","graphics_system") )
        db->addChild("Appearance","graphics_system");

    delete db;

    // END =================//


    QtSingleApplication app(argc, argv);
        app.setApplicationName( "Silicon" );
        app.setApplicationVersion( "Empire" );
        app.setQuitOnLastWindowClosed( false );

    QStringList args = QApplication::arguments();
        args.replace( 0 , QDir::currentPath() );

    QString args_str = args.join(";");
    if( app.isRunning() )
    {
        app.sendMessage( args_str );
        return 0;
    }

    SDefaults defaults;
    perConf prc( defaults.Personal_Configs_file );


    int ret_code;
    if( true /*args.contains("-s") || args.count() == 1*/ )
    {
        QString color_str = prc.readOption( perConf::HIGHLIGHT );
        QColor color = QPalette().color(QPalette::Highlight);
        if( !color_str.isEmpty() )
            color.setNamedColor( color_str );

        QImage image = SImage::colorize( QImage(":/files/Pixs/splash.png"), color.rgb() );

        QPixmap pixmap(QPixmap::fromImage(image));
        QSplashScreen splash(pixmap);
            splash.show();

        SiliconUI silicon( &prc );
            silicon.readMessage( args_str );
            silicon.show();

        QObject::connect( &app , SIGNAL(messageReceived(QString)) , &silicon , SLOT(readMessage(QString)) , Qt::QueuedConnection );

        splash.finish( &silicon );
        app.setActivationWindow( &silicon );

        silicon.initializeFinished();
        ret_code = app.exec();
    }

    qDebug() << "GoodBye";

    return ret_code;
}

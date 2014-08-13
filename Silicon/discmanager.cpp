#include "discmanager.h"

#include <QListWidgetItem>

#include <SMasterIcons>
#include <SCatWidget>
#include <SInfoWidget>
#include <SSql>
#include <Silicon>

class DiscManagerPrivate
{
public:
    SCatWidget  *cat_widget;
    SInfoWidget *info_widget;
    SSql        *sql;

    QListWidgetItem *cat_item;
};

DiscManager::DiscManager( SCatWidget *cat_widget , SInfoWidget *info_widget , QObject *parent ) :
    QObject(parent)
{
    p = new DiscManagerPrivate;
        p->info_widget = info_widget;

    p->cat_item = new QListWidgetItem();
        p->cat_item->setText( tr("Discs & Images") );
        p->cat_item->setIcon( SMasterIcons::icon( QSize(48,48) , "media-optical-dvd.png" ) );
        p->cat_item->setSizeHint( QSize(300,57) );

    p->cat_widget  = cat_widget;

    p->sql = new SSql( this );
    p->sql->setDefaultSQLAddress( Silicon::config() + "/silicon.db" );
        p->sql->lock();

    QObject::connect( p->sql , SIGNAL(discsIdDetected(QStringList)) , SLOT(discsDetected(QStringList))  );
    QObject::connect( p->sql , SIGNAL(detailDetected(SDiscInfo))    , SLOT(details_detected(SDiscInfo)) );

    init_discs();
}

void DiscManager::init_discs()
{/*
    // Test ===/
    SFileInfo file;
        file.setParent( "/home/bardia/abcd" );
        file.setDiscId( "Bardia" );
        file.setName( "fileA" );

    SMusicInfo music( QString("abcd") , QString("efgh") );
        music.setArtist( "Varan" );
        music.setAlbum("Piadeh");
        music.setYear(2005);
        file.setMusicInfo( music );


    SDiscInfo disc("Bardia");
        disc.setSize( 5 );
        disc.setFreeCapacity( 10 );

    SDiscInfo disc2("Amir");
        disc2.setSize( 5 );
        disc2.setFreeCapacity( 10 );

    //END ===//

    p->sql->insertDisc( disc );
    p->sql->insertDisc( disc2 );
    p->sql->insertFile( file );*/
    p->sql->discsID();
}

void DiscManager::showDisc( const QString & disc_id )
{
    p->sql->discDetails( disc_id );
}

void DiscManager::discsDetected( const QStringList & list )
{
    p->cat_widget->addCat( p->cat_item );
    for( int i=0 ; i<list.count() ; i++ )
        discDetected( list.at(i) );
}

void DiscManager::discDetected( const QString & disc_id )
{
    QIcon disc_icon = SMasterIcons::icon( QSize(48,48) , "media-optical-dvd.png" );

    p->cat_widget->addItem( tr("Discs & Images") , disc_icon , disc_id );
}

void DiscManager::details_detected( const SDiscInfo & disc )
{
    QIcon icon;
    switch( static_cast<int>(disc.type()) )
    {
    case SDiscInfo::Audio :
        icon = SMasterIcons::icon( QSize(256,256) , "media-optical-audio.png" );
        break;

    case SDiscInfo::BluRay_VIDEO :
        icon = SMasterIcons::icon( QSize(256,256) , "media-optical-blu-ray.png" );
        break;

    case SDiscInfo::Data :
        icon = SMasterIcons::icon( QSize(256,256) , "media-optical-data.png" );
        break;

    case SDiscInfo::DVD_VIDEO :
        icon = SMasterIcons::icon( QSize(256,256) , "media-optical-dvd-video.png" );
        break;

    case SDiscInfo::SVCD :
        icon = SMasterIcons::icon( QSize(256,256) , "media-optical-video.png" );
        break;

    case SDiscInfo::VCD :
        icon = SMasterIcons::icon( QSize(256,256) , "media-optical-video.png" );
        break;

    default:
        icon = SMasterIcons::icon( QSize(256,256) , "media-optical.png" );
        break;
    }

    QString text =   "<br /><b>Label: </b>"            + disc.label()
                   + "<br /><b>Type: </b>"             + disc.typeString()
                   + "<br /><b>Address: </b>"          + disc.address()
                   + "<br /><b>MD5 of First 1kb: </b>" + disc.md5Sum()
                   + "<br /><b>Application ID: </b>"   + disc.applicationId()
                   + "<br /><b>Publisher: </b>"        + disc.publisher()
                   + "<br /><b>Format: </b>"           + disc.format()
                   + "<br /><b>CopyRight: </b>"        + disc.copyRight()
                   + "<br /><b>System ID: </b>"        + disc.sysId()
                   + "<br /><b>Size: </b>"             + QString::number(disc.size())
                   + "<br /><b>Capacity: </b>"         + QString::number(disc.freeCapacity())
                   + "<br /><b>Appendable: </b>"       + QString::number(disc.appendable());

    p->info_widget->setTitle( disc.discId() );
    p->info_widget->setDescription( text );
    p->info_widget->setPicture( icon.pixmap(256,256) );
}

DiscManager::~DiscManager()
{
    delete p;
}

#include "ssqlwidget.h"

#include <SSql>
#include <SMasterIcons>

#include <QListWidget>
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QHash>
#include <QFileInfo>

class SSqlWidgetPrivate
{
public:
    SSql *sql;

    QVBoxLayout *layout;
    QListWidget *list_widget;

    QHash<QListWidgetItem *, STinyFileInfo> hash;

    STinyFileInfo current;
    STinyFileInfo current_dest;
};

SSqlWidget::SSqlWidget(QWidget *parent) :
    QWidget(parent)
{
    p = new SSqlWidgetPrivate;

    p->sql = new SSql( this );
    p->list_widget = new QListWidget();
        p->list_widget->setIconSize( QSize(48,48) );
        p->list_widget->setViewMode( QListWidget::IconMode );
        p->list_widget->setGridSize( QSize(128,92) );
        p->list_widget->setMovement( QListView::Snap );
        p->list_widget->setSelectionMode( QAbstractItemView::ExtendedSelection );
        p->list_widget->setContextMenuPolicy( Qt::CustomContextMenu );

    p->layout = new QVBoxLayout( this );
        p->layout->addWidget( p->list_widget );

    QObject::connect( p->list_widget , SIGNAL(itemDoubleClicked(QListWidgetItem*)) , SLOT(item_double_clicked(QListWidgetItem*)) );

    QObject::connect( p->sql , SIGNAL(discsIdDetected(QStringList))                 , SLOT(discs_id_detected(QStringList))                 );
    QObject::connect( p->sql , SIGNAL(detectedFilesOf(STinyFileInfo,SFileInfoList)) , SLOT(detected_files_of(STinyFileInfo,SFileInfoList)) );
    QObject::connect( p->sql , SIGNAL(detailDetected(SFileInfo))                    , SIGNAL(detailsDetected(SFileInfo))                   );
    QObject::connect( p->sql , SIGNAL(founded(QString,SFileInfoList))               , SLOT(founded(QString,SFileInfoList))                 );

    init_root_items();
}

void SSqlWidget::up()
{
    QString disc = p->current_dest.discID();
    QString address = p->current_dest.address();

    if( address.isEmpty() && p->current_dest.name().isEmpty() )
        disc = QString();

    cd( disc, address );
}

void SSqlWidget::cd( const QString & disc , const QString & dir )
{
    if( disc.isEmpty() )
    {
        p->current_dest = STinyFileInfo();
        init_root_items();
        return;
    }

    QFileInfo info( dir );

    QString address = info.path();
    if( dir == "/" )
        address = QString();

    STinyFileInfo file;
        file.setDiscID( disc );
        file.setName( info.fileName() );
        file.setAddress( address );
        file.setDirectory( true );

    p->sql->filesOf( file );
    p->current_dest = file;
}

const STinyFileInfo & SSqlWidget::current() const
{
    return p->current;
}

SFileInfoList SSqlWidget::currentFiles() const
{
    return p->hash.values();
}

SFileInfoList SSqlWidget::selectedFiles() const
{
    SFileInfoList list;
    QList<QListWidgetItem *> items = p->list_widget->selectedItems();

    for( int i=0 ; i<items.count() ; i++ )
        list << p->hash.value(items.at(i));

    return list;
}

void SSqlWidget::detailsOf( const STinyFileInfo & file )
{
    p->sql->fileDetails( file.ID() );
}

void SSqlWidget::search( const QString & key , SFileInfo::FileCat cat )
{
    p->sql->search( key , cat );
}

void SSqlWidget::init_root_items()
{
    p->sql->discsID();
}

void SSqlWidget::item_double_clicked( QListWidgetItem *item )
{
    STinyFileInfo dir = p->hash.value( item );

    if( dir.isDirectory() )
    {
        p->sql->filesOf( dir );
        p->current_dest = dir;
    }
    else
    {
        p->sql->fileDetails( dir.ID() );
    }
}

void SSqlWidget::discs_id_detected( const QStringList & list )
{
    p->hash.clear();
    p->list_widget->clear();

    p->current = STinyFileInfo();

    QIcon disc_icon = SMasterIcons::icon( QSize(48,48) , "media-optical-dvd.png" );
    QListWidgetItem *item;

    for( int i=0 ; i<list.count() ; i++ )
    {
        item = new QListWidgetItem();
            item->setText( list.at(i) );
            item->setIcon( disc_icon );

        p->list_widget->addItem( item );

        STinyFileInfo dir;
            dir.setDiscID( list.at(i) );
            dir.setDirectory( true );

        p->hash.insert( item , dir );
    }
}

void SSqlWidget::detected_files_of( const STinyFileInfo & current , const SFileInfoList & files )
{
    p->hash.clear();
    p->list_widget->clear();

    p->current = current;

    QIcon files_icon  = SMasterIcons::icon( QSize(48,48) , "unknown.png" );
    QIcon folder_icon = SMasterIcons::icon( QSize(48,48) , "folder.png" );

    QListWidgetItem *item;

    for( int i=0 ; i<files.count() ; i++ )
    {
        const STinyFileInfo & file = files.at(i);

        item = new QListWidgetItem();
            item->setText( file.name() );

        if( file.isDirectory() )
            item->setIcon( folder_icon );
        else
            item->setIcon( files_icon );

        p->list_widget->addItem( item );

        if( file.isDirectory() )
        {
            STinyFileInfo dir = file;
            p->hash.insert( item , dir );
        }
    }

    emit this->directoryChanged( p->current );
}

void SSqlWidget::founded( const QString & key , const SFileInfoList & list )
{
    detected_files_of( p->current , list );
}

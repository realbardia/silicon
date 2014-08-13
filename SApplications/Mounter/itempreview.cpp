#include "itempreview.h"

#include <smastericons.h>
#include <svariableconverter.h>

ItemPreview::ItemPreview(QWidget *parent) :
    QWidget(parent)
{
    setFixedHeight( 92 );
    setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Fixed );

    picture_widget = new SPictureWidget();
        picture_widget->setFixedSize( 92 , 92 );
        picture_widget->drawPixmap( QPixmap(":/mounter/files/pic.png") );

    name_label = new QLabel();
    address_label = new QLabel();
    mount_point_label = new QLabel();
    size_label = new QLabel();
    volume_label = new QLabel();
    application_id_label = new QLabel();
    publisher_label = new QLabel();
    system_label = new QLabel();
    format_label = new QLabel();
    copyright_label = new QLabel();

    hide_action = new QAction( SMasterIcons::icon( QSize(48,48) , "go-down.png" ) , tr("hide") , this );
    umount_action = new QAction( SMasterIcons::icon( QSize(48,48) , "media-eject.png" ) , tr("unmount") , this );

    actions_bar = new QToolBar();
        actions_bar->addAction( hide_action );
        actions_bar->addAction( umount_action );
        actions_bar->setToolButtonStyle( Qt::ToolButtonIconOnly );
        actions_bar->setOrientation( Qt::Vertical );
        actions_bar->setStyleSheet( "QToolBar{border-style:solid}" );

    data_area_widget = new QWidget();
    data_area = new QScrollArea();
        data_area->setFrameShadow( QFrame::Plain );
        data_area->setWidgetResizable( true );
        data_area->setFocusPolicy(Qt::NoFocus);
        data_area->setWidget( data_area_widget );
        data_area->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        data_area->viewport()->setAutoFillBackground( false );
        data_area->viewport()->setStyleSheet( "QWidget{background-color : transparent}" );

    form_layout = new QFormLayout( data_area_widget );
        form_layout->addRow( tr("Name : ") , name_label );
        form_layout->addRow( tr("Address : ") , address_label );
        form_layout->addRow( tr("MountPoint : ") , mount_point_label );
        form_layout->addRow( tr("Size : ") , size_label );
        form_layout->addRow( tr("Volume Label : ") , volume_label );
        form_layout->addRow( tr("Application ID : ") , application_id_label );
        form_layout->addRow( tr("Publisher : ") , publisher_label );
        form_layout->addRow( tr("System ID : ") , system_label );
        form_layout->addRow( tr("Format : ") , format_label );
        form_layout->addRow( tr("Copyright : ") , copyright_label );

    base_widget = new QWidget();

    layout = new QHBoxLayout( base_widget );
        layout->addWidget( picture_widget );
        layout->addWidget( data_area );
        layout->addWidget( actions_bar );

    base_layout = new QVBoxLayout( this );
        base_layout->addWidget( base_widget );
        base_layout->setContentsMargins( 4 , 4 , 4 , 4 );

    animation = new SAnimation( this );
        animation->setFixOptions( SAnimation::WIDTH_FIX );
        animation->setFrameRate( 1 );

    QObject::connect( hide_action , SIGNAL(triggered()) , this , SLOT(hideWithAnim()) );
    QObject::connect( umount_action , SIGNAL(triggered()) , this , SLOT(eject_clicked()) );
    QObject::connect( animation , SIGNAL(finished()) , this , SLOT(animation_finished()) );
}

void ItemPreview::setName( const QString & name )
{
    name_label->setText( name );
}

void ItemPreview::setAddress( const QString & address )
{
    address_label->setText( address );
}

void ItemPreview::setMountPoint( const QString & mountPoint )
{
    mount_point_label->setText( mountPoint );
}

void ItemPreview::setSize( int size_kb )
{
    size_label->setText( SVariableConverter::intToString(size_kb) + " KB" );
}

void ItemPreview::setVolumeID( const QString & volume )
{
    this->volume_label->setText( volume );
}

void ItemPreview::setApplicationID( const QString & id )
{
    this->application_id_label->setText( id );
}

void ItemPreview::setPublisher( const QString & publisher )
{
    this->publisher_label->setText( publisher );
}

void ItemPreview::setSystemID( const QString & system )
{
    this->system_label->setText( system );
}

void ItemPreview::setFormat( const QString & format )
{
    this->format_label->setText( format );
}

void ItemPreview::setCopyright( const QString & copyright )
{
    this->copyright_label->setText( copyright );
}


QString ItemPreview::name()
{
    return name_label->text();
}

QString ItemPreview::address()
{
    return address_label->text();
}

QString ItemPreview::mountPoint()
{
    return mount_point_label->text();
}

int ItemPreview::sizeKB()
{
    bool ok;
    return size_label->text().toInt(&ok);
}

QString ItemPreview::volumeLabel()
{
    return this->volume_label->text();
}

QString ItemPreview::applicationID()
{
    return this->application_id_label->text();
}

QString ItemPreview::publisher()
{
    return this->publisher_label->text();
}

QString ItemPreview::systemID()
{
    return this->system_label->text();
}

QString ItemPreview::format()
{
    return this->format_label->text();
}

QString ItemPreview::copyright()
{
    return this->copyright_label->text();
}

void ItemPreview::reload()
{
    if( base_widget->isHidden() )
        showWithAnim();
}

void ItemPreview::hideWithAnim()
{
    base_widget->hide();
    animation->start( this , QSize( 0 , 0 ) , SAnimation::RESIZE , 13 , SAnimation::MIN_MAX );
}

void ItemPreview::showWithAnim()
{
    base_widget->hide();
    animation->start( this , QSize( 9 , 92 ) , SAnimation::RESIZE , 13 , SAnimation::MIN_MAX );
}

void ItemPreview::animation_finished()
{
    bool ok = ( this->height() == 0 );

    static_cast<QWidget*>(parent())->setHidden( ok );
    base_widget->setHidden( ok );
}

void ItemPreview::eject_clicked()
{
    emit this->eject();
}

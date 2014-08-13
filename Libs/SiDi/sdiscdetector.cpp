#define RATIO_SIZE (1024*1024)

#include "sdiscdetector.h"
#include "sdevicelist.h"
#include "sdiscfeatures.h"

#include <Silicon>
#include <SDialog>

#include <QMessageBox>
#include <QHBoxLayout>
#include <QAbstractButton>
#include <QFileInfo>

class SDiscDetectorPrivate
{
public:
    QString     source_img;
    SDeviceItem source;
    SDeviceItem destionation;
    SDeviceItem rewribale;
    SDeviceItem disc;

    SDeviceList *device_list;
    SDialog     *dialog;
    QMessageBox *message;

    quint64 custom_min_size;
};

SDiscDetector::SDiscDetector(QObject *parent) :
    QObject(parent)
{
    p = new SDiscDetectorPrivate;
        p->dialog = 0;
        p->custom_min_size = 0;

    p->device_list = new SDeviceList( this );
}

void SDiscDetector::init_dialog( const QString & str )
{
    p->message = new QMessageBox();
        p->message->setWindowFlags( Qt::Widget );
        p->message->setWindowTitle( tr("CdRecord Notification") );
        p->message->setIcon( QMessageBox::Warning );
        p->message->addButton( QMessageBox::Ok );
        p->message->addButton( QMessageBox::Cancel );
        p->message->setText( str );

    p->dialog = new SDialog( 437 , 113 );
    QHBoxLayout *layout = new QHBoxLayout( p->dialog );
        layout->addWidget( p->message );


    connect( p->message , SIGNAL(buttonClicked(QAbstractButton*)) , SLOT(button_clicked(QAbstractButton*)) , Qt::QueuedConnection );
    connect( p->dialog  , SIGNAL(destroyed())                     , SLOT(dialog_destroyed())               , Qt::QueuedConnection );
}

void SDiscDetector::button_clicked( QAbstractButton *button )
{
    QMessageBox::ButtonRole role = p->message->buttonRole( button );
    switch( static_cast<int>(role) )
    {
    case QMessageBox::Rejected:
        close_dialog();
        checking();
        break;

    case QMessageBox::Accepted:
        close_dialog();
        disconnect( p->device_list , SIGNAL(deviceDetected(SDeviceItem)) , this , SLOT(checking()) );
        emit rejected();
        break;
    }
}

void SDiscDetector::close_dialog()
{
    disconnect( p->dialog , SIGNAL(destroyed()) , this , SLOT(dialog_destroyed()) );
    p->dialog->close();
    p->dialog = 0;
}

void SDiscDetector::dialog_destroyed()
{
    p->dialog = 0;
    checking();
}

void SDiscDetector::setSourceImage( const QString & image )
{
    p->source_img = image;
    p->source = SDeviceItem();
}

void SDiscDetector::setSourceDisc( const SDeviceItem & device )
{
    p->source = device;
    p->source_img = QString();
}

void SDiscDetector::setDestinationDisc( const SDeviceItem & device , quint64 custom_min_size )
{
    p->custom_min_size = custom_min_size;
    p->destionation = device;
}

void SDiscDetector::setReWritableDisc( const SDeviceItem & device )
{
    p->rewribale = device;
}

void SDiscDetector::setDisc( const SDeviceItem & device )
{
    p->disc = device;
}

void SDiscDetector::check()
{
    connect( p->device_list , SIGNAL(deviceDetected(SDeviceItem)) , SLOT(checking()) );
    checking();
}

void SDiscDetector::checking()
{
    int unsolved = 0;
    QString message;
    unsigned long int minimum_dst_size = 0;



    if( !p->source.isEmpty() )
    {
        const SDiscFeatures & disc = p->source.currentDiscFeatures();
        if( disc.volume_disc_is_blank || disc.volume_size_int <= 0 )
        {
            unsolved++;
            message += tr( "Can't detect disc on source device (%1)\n" ).arg( p->source.name() );
        }
        else
            minimum_dst_size = 1+ disc.volume_size_int / RATIO_SIZE;
    }
    if( !p->source_img.isEmpty() )
    {
        minimum_dst_size = 1+ QFileInfo(p->source_img).size() / RATIO_SIZE;
    }
    if( p->custom_min_size != 0 )
    {
        minimum_dst_size = p->custom_min_size / RATIO_SIZE;
    }
    if( !p->destionation.isEmpty() )
    {
        const SDiscFeatures & disc = p->destionation.currentDiscFeatures();
        if( !disc.volume_disc_is_blank || minimum_dst_size > disc.volume_capacity_int/RATIO_SIZE )
        {
            unsolved++;
            message += tr( "Can't detect disc on \"%1\".\nPlease Insert an empty disc.\n" ).arg( p->destionation.name() );
            if( minimum_dst_size != 0 )
                message += tr( "Free space needed: %2MB\n" ).arg( QString::number(minimum_dst_size) );
        }
    }
    if( !p->rewribale.isEmpty() )
    {
        const SDiscFeatures & disc = p->rewribale.currentDiscFeatures();
        if( !p->rewribale.deviceFeatures().media_available_stt || !disc.volume_disc_type_str.contains("rw",Qt::CaseInsensitive) )
        {
            unsolved++;
            message += tr( "Can't detect rewritable Disc on \"%1\".\nPlease Insert a rewritable disc.\n" ).arg( p->rewribale.name() );
        }
    }
    if( !p->disc.isEmpty() )
    {
        if( p->disc.deviceFeatures().media_available_stt )
        {
            unsolved++;
            message += tr( "Can't detect any Disc on \"%1\".\n" ).arg( p->disc.name() );
        }
    }



    if( unsolved == 0 )
    {
        if( p->dialog != 0 )
            close_dialog();

        disconnect( p->device_list , SIGNAL(deviceDetected(SDeviceItem)) , this , SLOT(checking()) );
        emit accepted();
    }
    else if( p->dialog == 0 && unsolved != 0 )
    {
        init_dialog( message );
    }
}

SDiscDetector::~SDiscDetector()
{
    if( p->dialog != 0 )
        close_dialog();

    delete p;
}

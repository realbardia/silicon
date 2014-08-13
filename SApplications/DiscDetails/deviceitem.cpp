#define ICON_SIZE  QSize(48,48)

#include "deviceitem.h"
#include "ui_device.h"

#include <SDeviceList>
#include <SMasterIcons>

#include <QList>

QString bool2String( bool stt )
{
    QString yes = "<b><font color=green>" + QObject::tr("YES") + "</font></b>";
    QString no = "<b><font color=red>" + QObject::tr("NO") + "</font></b>";

    if( stt )
        return yes;
    else
        return no;
}


class DeviceItemPrivate
{
public:
    Ui::DiscUi  *ui;
    SDeviceList *device_list;

    QList<SDeviceItem> devices;
};

DeviceItem::DeviceItem( QWidget *parent ) :
    QWidget(parent)
{
    p = new DeviceItemPrivate;

    p->ui = new Ui::DiscUi;
        p->ui->setupUi( this );
        p->ui->close_button->setIcon( SMasterIcons::icon(ICON_SIZE,"application-exit.png") );

    p->device_list = new SDeviceList( this );

    connect( p->device_list      , SIGNAL(deviceDetected(SDeviceItem)) , SLOT(deviceDetected(SDeviceItem)) );
    connect( p->ui->close_button , SIGNAL(clicked())                   , SLOT(closeButtonClicked())        );

    p->device_list->refresh();
}

void DeviceItem::deviceDetected( const SDeviceItem & device )
{
    if( !p->devices.contains(device) )
    {
        p->devices << device;
        p->ui->devices_combo->insertItem( p->devices.count(), SMasterIcons::icon(ICON_SIZE,"drive-optical.png") , device.name() );
    }
    else
    {
        int index = p->devices.indexOf( device );
        p->devices.replace( index , device );
        p->ui->devices_combo->setItemText( index , device.name() );
    }

    comboChanged( p->ui->devices_combo->currentIndex() );
}

void DeviceItem::comboChanged( int index )
{
    const SDeviceFeatures & features = p->devices[index].deviceFeatures();
    const SDiscFeatures   & disc     = p->devices[index].currentDiscFeatures();

    p->ui->read_speed_text->setText(      QString::number(features.read_speed_int)  );
    p->ui->device_address_text->setText(  features.device_block_str                 );
    p->ui->drive_type_text->setText(      features.storage_drive_type_str           );
    p->ui->firmware_text->setText(        features.storage_firmware_str             );
    p->ui->disc_type_text->setText(       disc.volume_disc_type_str                 );
    p->ui->disc_blank_text->setText(      bool2String(disc.volume_disc_is_blank)    );
    p->ui->disc_capacity_text->setText(   QString::number(disc.volume_capacity_int) );
    p->ui->removable_media_text->setText( bool2String(features.removable_stt)       );
    p->ui->cdr_text->setText(             bool2String(features.cdr_stt)             );
    p->ui->cdrw_text->setText(            bool2String(features.cdrw_stt)            );
    p->ui->dvd_text->setText(             bool2String(features.dvd_stt)             );
    p->ui->dvdr_text->setText(            bool2String(features.dvdr_stt)            );
    p->ui->dvdrw_text->setText(           bool2String(features.dvdrw_stt)           );
    p->ui->dvdrdl_text->setText(          bool2String(features.dvdrdl_stt)          );
    p->ui->dvdram_text->setText(          bool2String(features.dvdram_stt)          );
    p->ui->dvdplusr_text->setText(        bool2String(features.dvdplusr_stt)        );
    p->ui->dvdplusrw_text->setText(       bool2String(features.dvdplusrw_stt)       );
    p->ui->dvdplusrwdl_text->setText(     bool2String(features.dvdplusrwdl_stt)     );
    p->ui->dvdplusrdl_text->setText(      bool2String(features.dvdplusrdl_stt)      );
    p->ui->bd_text->setText(              bool2String(features.bd_stt)              );
    p->ui->bdr_text->setText(             bool2String(features.bdr_stt)             );
    p->ui->hddvd_text->setText(           bool2String(features.hddvd_stt)           );
    p->ui->hddvdr_text->setText(          bool2String(features.hddvdr_stt)          );
    p->ui->hddvdrw_text->setText(         bool2String(features.hddvdrw_stt)         );

    p->ui->volume_text->setText(      disc.volume_label_str                       );
    p->ui->fstype_text->setText(      disc.volume_fstype_str                      );
    p->ui->fsversion_text->setText(   disc.volume_fsversion_str                   );
    p->ui->disc_type_text_2->setText( disc.volume_disc_type_str                   );
    p->ui->volume_size_text->setText( QString::number(disc.volume_size_int)       );
    p->ui->block_size_text->setText(  QString::number(disc.volume_block_size_int) );
    p->ui->mountpoint_text->setText(  disc.mount_point_str                        );
    p->ui->appendable_text->setText(  bool2String(disc.volume_is_appendable)      );
    p->ui->data_text->setText(        bool2String(disc.dataDisc)                  );
    p->ui->audio_text->setText(       bool2String(disc.audio)                     );
    p->ui->vcd_text->setText(         bool2String(disc.vcd)                       );
    p->ui->svcd_text->setText(        bool2String(disc.svcd)                      );
    p->ui->vdvd_text->setText(        bool2String(disc.videoDVD)                  );
    p->ui->vbr_text->setText(         bool2String(disc.videoBluRay)               );
}

void DeviceItem::closeButtonClicked()
{
    emit closeRequest( this );
}

DeviceItem::~DeviceItem()
{
    delete p->ui;
    delete p;
}

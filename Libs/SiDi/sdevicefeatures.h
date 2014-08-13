#ifndef SDEVICEFEATURES_H
#define SDEVICEFEATURES_H

#include <QList>
#include <QString>

#include "SiDi_global.h"

class SIDI_EXPORT SDeviceFeatures
{
public:
    SDeviceFeatures();

    int hotplug_type_int;
    unsigned long long media_size_int;
    int block_major_int;
    int block_minor_int;

    QString partitioning_scheme_str;
    QString storage_device_str;
    QString device_block_str;
    QString storage_bus_str;
    QString storage_drive_type_str;
    QString storage_drive_model_str;
    QString storage_vendor_str;
    QString storage_firmware_str;
    QString storage_address_str;

    QString info_product_str;
    QString info_vendor_str;

    bool cdr_stt;
    bool cdrw_stt;
    bool dvd_stt;
    bool dvdr_stt;
    bool dvdrw_stt;
    bool dvdrdl_stt;
    bool dvdram_stt;
    bool dvdplusr_stt;
    bool dvdplusrw_stt;
    bool dvdplusrwdl_stt;
    bool dvdplusrdl_stt;
    bool bd_stt;
    bool bdr_stt;
    bool bdre_stt;
    bool hddvd_stt;
    bool hddvdr_stt;
    bool hddvdrw_stt;
    bool mo_stt;
    bool mrw_stt;
    bool mrw_w_stt;

    bool block_is_volume_stt;
    bool multi_session_support_stt;
    bool media_changed_support_stt;
    bool storage_no_partition_hint_stt;
    bool storage_media_check_stt;
    bool storage_auto_mount_stt;
    bool media_available_stt;
    bool removable_stt;


    int read_speed_int;
    int write_speed_int;
    QList<int> speed_list;

    QList<int> cd_speed_list;
    QList<int> dvd_speed_list;
    QList<int> bluray_speed_list;
};

#endif // SDEVICEFEATURES_H

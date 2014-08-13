#ifndef SDISCFEATURES_H
#define SDISCFEATURES_H

#include <QString>

#include "SiDi_global.h"

class SIDI_EXPORT SDiscFeatures
{
public:
    SDiscFeatures();

    int hotplug_type_int;
    int volume_block_size_int;
    int block_major_int;
    int block_minor_int;

    unsigned long long volume_size_int;
    unsigned long long block_numbers_int;
    unsigned long long volume_capacity_int;

    bool videoDVD;
    bool videoBluRay;
    bool rewritable;
    bool dataDisc;
    bool vcd;
    bool svcd;
    bool audio;

    bool volume_is_appendable;
    bool volume_is_mounted;
    bool volume_mounted_readonly;
    bool volume_device_mapper;
    bool volume_disc_is_blank;

    QString mount_point_str;
    QString parent_str;
    QString info_product_str;
    QString info_udi_str;
    QString block_storage_device_str;
    QString volume_fstype_str;
    QString volume_fsusage_str;
    QString volume_fsversion_str;
    QString volume_label_str;
    QString volume_disc_type_str;
    QString block_device_str;
};

#endif // SDISCFEATURES_H

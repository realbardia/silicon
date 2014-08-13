#include "sdiscfeatures.h"

SDiscFeatures::SDiscFeatures()
{
    hotplug_type_int = -1;
    volume_block_size_int = -1;
    block_major_int = -1;
    block_minor_int = -1;

    volume_size_int = 0;
    block_numbers_int = 0;
    volume_capacity_int = 0;

    videoDVD = false;
    videoBluRay = false;
    rewritable = false;
    dataDisc = false;
    vcd = false;
    svcd = false;
    audio = false;

    volume_is_appendable = false;
    volume_is_mounted = false;
    volume_mounted_readonly = false;
    volume_device_mapper = false;
    volume_disc_is_blank = false;
}

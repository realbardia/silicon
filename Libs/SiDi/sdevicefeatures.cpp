#include "sdevicefeatures.h"

SDeviceFeatures::SDeviceFeatures()
{
    hotplug_type_int = -1;
    media_size_int = 0;
    block_major_int = -1;
    block_minor_int = -1;
/*
    partitioning_scheme_str = 0;
    storage_device_str = 0;
    device_block_str = 0;
    storage_bus_str = 0;
    storage_drive_type_str = 0;
    storage_drive_model_str = 0;
    storage_vendor_str = 0;
    storage_firmware_str = 0;

    info_product_str = 0;
    info_vendor_str = 0;
*/
    cdr_stt = false;
    cdrw_stt = false;
    dvd_stt = false;
    dvdr_stt = false;
    dvdrw_stt = false;
    dvdrdl_stt = false;
    dvdram_stt = false;
    dvdplusr_stt = false;
    dvdplusrw_stt = false;
    dvdplusrwdl_stt = false;
    dvdplusrdl_stt = false;
    bd_stt = false;
    bdr_stt = false;
    bdre_stt = false;
    hddvd_stt = false;
    hddvdr_stt = false;
    hddvdrw_stt = false;
    mo_stt = false;
    mrw_stt = false;
    mrw_w_stt = false;

    block_is_volume_stt = false;
    multi_session_support_stt = false;
    media_changed_support_stt = false;
    storage_no_partition_hint_stt = false;
    storage_media_check_stt = false;
    storage_auto_mount_stt = false;
    media_available_stt = false;
    removable_stt = false;


    read_speed_int = -1;
//  speed_list.clear();
}

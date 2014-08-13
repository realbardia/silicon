#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    AudioCdRecordPlugin *plugin = new AudioCdRecordPlugin();
    return plugin;
}

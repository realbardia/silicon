#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    CdRecordPlugin *plugin = new CdRecordPlugin();
    return plugin;
}

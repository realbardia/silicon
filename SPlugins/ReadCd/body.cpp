#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    ReadCdPlugin *plugin = new ReadCdPlugin();
    return plugin;
}

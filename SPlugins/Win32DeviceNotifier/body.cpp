#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    WdnPlugin *plugin = new WdnPlugin();
    return plugin;
}

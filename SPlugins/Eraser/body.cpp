#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    EraserPlugin *plugin = new EraserPlugin();
    return plugin;
}

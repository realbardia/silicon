#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    MPG321Plugin *plugin = new MPG321Plugin();
    return plugin;
}

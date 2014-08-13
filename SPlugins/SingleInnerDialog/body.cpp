#include "plugin.h"

extern "C" SPlugin *plugin()       // Required Function <-----
{
    SingleInnerDialog *plugin = new SingleInnerDialog();
    return plugin;
}

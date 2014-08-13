#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>
#include "systray.h"

class SystemTrayPlugin : public SPlugin
{
public:
    SystemTrayPlugin();
    ~SystemTrayPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    SysTray *tray;
};

#endif // PLUGIN_H

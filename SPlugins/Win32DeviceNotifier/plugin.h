#ifndef WDNPLUGIN_H
#define WDNPLUGIN_H

#include <SPlugin>

#include "win32devicenotifier.h"

class WdnPlugin : public SPlugin
{
public:
    WdnPlugin();
    ~WdnPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    Win32DeviceNotifier *wdn;
};

#endif // WDNPLUGIN_H

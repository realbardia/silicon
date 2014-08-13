#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>
#include <SiDiTools>

#include "readcdengine.h"

class ReadCdPlugin : public SPlugin
{
public:
    ReadCdPlugin();
    ~ReadCdPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    ReadCdEngine *engine;
};

#endif // PLUGIN_H

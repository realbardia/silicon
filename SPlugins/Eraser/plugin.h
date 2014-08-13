#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>
#include <SiDiTools>

#include "eraserengine.h"

class EraserPlugin : public SPlugin
{
public:
    EraserPlugin();
    ~EraserPlugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    EraserEngine *engine;
};

#endif // PLUGIN_H

#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>
#include <SiDiTools>

#include "mpg123engine.h"

class MPG123Plugin : public SPlugin
{
public:
    MPG123Plugin();
    ~MPG123Plugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    MPG123Engine *engine;
    QMultiHash<QString,QString> mpg123_types;
};

#endif // PLUGIN_H

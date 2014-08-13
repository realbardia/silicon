#ifndef PLUGIN_H
#define PLUGIN_H

#include <SPlugin>
#include <SiDiTools>

#include "mpg321engine.h"

class MPG321Plugin : public SPlugin
{
public:
    MPG321Plugin();
    ~MPG321Plugin();

protected:
    bool startEvent();
    bool stopEvent();

private:
    MPG321Engine *engine;
    QMultiHash<QString,QString> mpg123_types;
};

#endif // PLUGIN_H

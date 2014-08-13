#ifndef ROOTMOUNTENGINE_H
#define ROOTMOUNTENGINE_H

#include <SAbstractProcessEngine>

class RootMountEngine : public SAbstractProcessEngine
{
public:
    SAbstractProcess *create( QObject *parent = 0 );
};

#endif // ROOTMOUNTENGINE_H

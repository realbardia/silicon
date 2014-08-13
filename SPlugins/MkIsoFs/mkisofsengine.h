#ifndef MKISOFSENGINE_H
#define MKISOFSENGINE_H

#include <SAbstractProcessEngine>

class MkIsoFsEngine : public SAbstractProcessEngine
{
public:
    SAbstractProcess *create( QObject *parent = 0 );
};

#endif // MKISOFSENGINE_H

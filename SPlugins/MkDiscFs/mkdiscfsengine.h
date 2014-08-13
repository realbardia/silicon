#ifndef MKDISCFSENGINE_H
#define MKDISCFSENGINE_H

#include <SAbstractProcessEngine>

class MkDiscFsEngine : public SAbstractProcessEngine
{
public:
    SAbstractProcess *create( QObject *parent = 0 );
};

#endif // MKDISCFSENGINE_H

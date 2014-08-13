#ifndef READCDENGINE_H
#define READCDENGINE_H

#include <SAbstractProcessEngine>

class ReadCdEngine : public SAbstractProcessEngine
{
public:
    SAbstractProcess *create( QObject *parent = 0 );
};

#endif // READCDENGINE_H

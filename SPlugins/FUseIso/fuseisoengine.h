#ifndef FUSEISOENGINE_H
#define FUSEISOENGINE_H

#include <SAbstractProcessEngine>

class FUseIsoEngine : public SAbstractProcessEngine
{
public:
    SAbstractProcess *create( QObject *parent = 0 );
};

#endif // FUSEISOENGINE_H

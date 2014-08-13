#ifndef SABSTRACTPROCESSENGINE_H
#define SABSTRACTPROCESSENGINE_H

#include "SiDi_global.h"

class QObject;
class SAbstractProcess;

class SIDI_EXPORT SAbstractProcessEngine
{
public:
    virtual SAbstractProcess *create( QObject *parent = 0 );
};

#endif // SABSTRACTPROCESSENGINE_H

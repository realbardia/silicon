#ifndef AUDIOCDRECORDENGINE_H
#define AUDIOCDRECORDENGINE_H

#include <SAbstractProcessEngine>

class AudioCdRecordEngine : public SAbstractProcessEngine
{
public:
    SAbstractProcess *create( QObject *parent = 0 );
};

#endif // AUDIOCDRECORDENGINE_H

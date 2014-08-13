#ifndef CDRECORDENGINE_H
#define CDRECORDENGINE_H

#include <SAbstractProcessEngine>

class CdRecordEngine : public SAbstractProcessEngine
{
public:
    SAbstractProcess *create( QObject *parent = 0 );
};

#endif // CDRECORDENGINE_H

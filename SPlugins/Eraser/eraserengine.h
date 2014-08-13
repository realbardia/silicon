#ifndef ERASERENGINE_H
#define ERASERENGINE_H

#include <SAbstractProcessEngine>

class EraserEngine : public SAbstractProcessEngine
{
public:
    SAbstractProcess *create( QObject *parent = 0 );
};

#endif // ERASERENGINE_H

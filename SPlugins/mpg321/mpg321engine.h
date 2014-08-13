#ifndef MPG321ENGINE_H
#define MPG321ENGINE_H

#include <SAbstractConverterEngine>

class MPG321Engine : public SAbstractConverterEngine
{
public:
    SAbstractProcess *create( QObject *parent = 0 );
};

#endif // MPG321ENGINE_H

#ifndef MPG123ENGINE_H
#define MPG123ENGINE_H

#include <SAbstractConverterEngine>

class MPG123Engine : public SAbstractConverterEngine
{
public:
    SAbstractProcess *create( QObject *parent = 0 );
};

#endif // MPG123ENGINE_H

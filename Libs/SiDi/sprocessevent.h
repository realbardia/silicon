#ifndef SPROCESSEVENT_H
#define SPROCESSEVENT_H

#include "sdeviceitem.h"
#include "SiDi_global.h"

#include <QString>

class SIDI_EXPORT SProcessEvent
{
public:
    enum Type{
        Device,
        Address,
        Unknown
    };

    SProcessEvent();
    ~SProcessEvent();

    void setDevice( const SDeviceItem & device );
    void setAddress( const QString & address );

    const QString & address();
    const SDeviceItem & device();

    Type currentType();

    void accept();
    void reject();

    bool isAccepted();
    bool isRejected();
};

#endif // SPROCESSEVENT_H

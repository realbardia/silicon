#ifndef SDISCDETECTOR_H
#define SDISCDETECTOR_H

#include <QObject>

#include "sdeviceitem.h"
#include "SiDi_global.h"

class QAbstractButton;
class SDiscDetectorPrivate;
class SIDI_EXPORT SDiscDetector : public QObject
{
    Q_OBJECT
public:
    SDiscDetector(QObject *parent = 0);
    ~SDiscDetector();

    void setSourceImage( const QString & image );
    void setSourceDisc( const SDeviceItem & device );
    void setDestinationDisc( const SDeviceItem & device , quint64 custom_min_size = 0 );
    void setReWritableDisc( const SDeviceItem & device );
    void setDisc( const SDeviceItem & device );

public slots:
    void check();

signals:
    void accepted();
    void rejected();

private slots:
    void checking();
    void dialog_destroyed();
    void close_dialog();

    void button_clicked( QAbstractButton *button );

private:
    void init_dialog( const QString & str );

private:
    SDiscDetectorPrivate *p;
};

#endif // SDISCDETECTOR_H

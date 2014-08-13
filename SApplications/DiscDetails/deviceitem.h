#ifndef DEVICEITEM_H
#define DEVICEITEM_H

#include <QWidget>

#include <SDeviceItem>

class DeviceItemPrivate;
class DeviceItem : public QWidget
{
    Q_OBJECT
public:
    DeviceItem( QWidget *parent = 0 );
    ~DeviceItem();

signals:
    void closeRequest( QWidget *item );

public slots:

private slots:
    void deviceDetected( const SDeviceItem & device );
    void comboChanged( int index );
    void closeButtonClicked();

private:
    DeviceItemPrivate *p;
};

#endif // DEVICEITEM_H

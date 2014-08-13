#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <SPage>
#include <SApplication>

#include <SDeviceItem>

class MainPagePrivate;
class MainPage : public SPage
{
    Q_OBJECT
public:

    MainPage( SApplication *parent = 0 );
    ~MainPage();

    const SDeviceItem & sourceDevice() const;
    const SDeviceItem & destinationDevice() const;

    bool scan() const;
    int copiesNumber() const;
    int speed() const;
    bool eject() const;
    bool dummy() const;
    bool onFly() const;

    QString scanName() const;


    void setDestinationDevice( const QString & bus_len_id );
    void setSourceDevice( const QString & bus_len_id );

    void setScan( const QString & name );
    void setCopiesNumber( int value );
    void setSpeed( int value );
    void setEject( bool stt );
    void setDummy( bool stt );
    void setOnFly( bool stt );

signals:
    void go();

private slots:
    void deviceDetected( const SDeviceItem & device );
    void setDefaultOptions();

    void go_prev();
    void more_prev();

private:
    void init_actions();

private:
    MainPagePrivate *p;
};

#endif // MAINPAGE_H

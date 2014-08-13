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
    bool eject() const;
    bool force() const;
    bool leadOut() const;


    void setSourceDevice( const QString & bus_len_id );
    void setEject( bool stt );
    void setForce( bool stt );
    void setLeadOut( bool stt );

signals:
    void go();

private slots:
    void deviceDetected( const SDeviceItem & device );

    void go_prev();
    void more_prev();

private:
    void init_actions();

private:
    MainPagePrivate *p;
};

#endif // MAINPAGE_H

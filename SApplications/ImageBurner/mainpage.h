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

    QString sourceImage() const;
    const SDeviceItem & destinationDevice() const;

    bool scan() const;
    int copiesNumber() const;
    int speed() const;
    bool eject() const;
    bool dummy() const;
    bool remove() const;

    QString scanName() const;


    void setDestinationDevice( const QString & bus_len_id );

    void setScan( const QString & str );
    void setCopiesNumber( int value );
    void setSpeed( int speed );
    void setEject( bool stt );
    void setDummy( bool stt );
    void setRemove( bool stt );

public slots:
    void setSourceImage( const QString & file );

signals:
    void go();

private slots:
    void deviceDetected( const SDeviceItem & device );
    void setDefaultOptions();
    void device_index_changed( int index );

    void select_src_image();

    void go_prev();
    void more_prev();

protected:
    void dropEvent( QDropEvent *event );
    void dragEnterEvent( QDragEnterEvent *event );

private:
    void init_actions();

private:
    MainPagePrivate *p;
};

#endif // MAINPAGE_H

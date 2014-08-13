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

    QString destinationImage() const;
    const SDeviceItem & sourceDevice() const;

    bool scan() const;
    bool mount() const;
    bool eject() const;
    bool library() const;

    QString libraryName() const;
    QString libraryTags() const;

    QString scanName() const;

    int startSector() const;
    int endSector() const;


    void setSourceDevice( const QString & bus_len_id );

    void setScan( const QString & str );
    void setEject( bool stt );
    void setMount( bool stt );
    void setSectors( int start , int end );

public slots:
    void setDestinationImage( const QString & file );

signals:
    void go();

private slots:
    void deviceDetected( const SDeviceItem & device );
    void to_spin_changed( int value );
    void setDefaultOptions();

    void select_dst_image();

    void go_prev();
    void more_prev();

    void check_access();

protected:
    void dropEvent( QDropEvent *event );
    void dragEnterEvent( QDragEnterEvent *event );

private:
    void init_actions();

private:
    MainPagePrivate *p;
};

#endif // MAINPAGE_H

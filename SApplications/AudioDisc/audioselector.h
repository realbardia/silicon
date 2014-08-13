#ifndef AUDIOSELECTOR_H
#define AUDIOSELECTOR_H

#include <SPage>
#include <SDeviceItem>

class AudioSelectorPrivate;
class AudioSelector : public SPage
{
    Q_OBJECT
public:
    AudioSelector( SApplication *parent );
    ~AudioSelector();

    QStringList files() const;
    SDeviceItem currentDevice() const;

    void setProgressBar( int value , int maximum );

    int copiesNumber() const;
    int speed() const;
    bool eject() const;
    bool mount() const;
    bool multiSession() const;
    bool dummy() const;
    bool library() const;
    bool trackAtOnce() const;
    bool discAtOnce() const;
    bool swab() const;

    QString libraryName() const;
    QString libraryTags() const;

    QString scanName() const;

    QString volumeLabel() const;
    QString copyRight() const;
    QString publisher() const;
    QString applicationId() const;
    QString systemId() const;
    QString abstract() const;
    QString biblio() const;


public slots:
    void setDevice( const QString & bus_len_id );

    void setScan( const QString & name );
    void setCopiesNumber( int value );
    void setSpeed( int value );
    void setEject( bool stt );
    void setDummy( bool stt );
    void setMultiSession( bool stt );
    void setMount( bool stt );
    void setTrackAtOnce( bool stt );
    void setDiscAtOnce( bool stt );
    void setSwab( bool stt );

    void setVolumeLabel( const QString & name );
    void setCopyRight( const QString & name );
    void setPublisher( const QString & name );
    void setApplicationId( const QString & name );
    void setSystemId( const QString & name );
    void setAbstract( const QString & name );
    void setBiblio( const QString & name );

    void addFiles();
    void addFiles( const QStringList & files );

    void removeCurrents();

signals:
    void go();

protected:
    void dropEvent( QDropEvent *event );
    void dragEnterEvent( QDragEnterEvent *event );

private slots:
    void go_prev();
    void timer_finished( bool stt );

    void deviceDetected( const SDeviceItem & device );
    void device_index_changed( int index );

private:
    void init_actions();
    void init_system();

private:
    AudioSelectorPrivate *p;
};

#endif // AUDIOSELECTOR_H

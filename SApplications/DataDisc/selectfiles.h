#ifndef SELECTFILES_H
#define SELECTFILES_H

#include <SPage>
#include <SAbstractImageCreator>
#include <SAbstractDataBurner>

class SDeviceItem;
class SelectFilesPrivate;
class SelectFiles : public SPage
{
    Q_OBJECT
public:
    SelectFiles( SApplication *parent, SAbstractDataBurner *data_burner, SAbstractImageCreator *image_creator );
    ~SelectFiles();

    enum Type {
        OnFlyDataDisc,
        NoOnFlyDataDisc,
        DataIso,
        Unknown
    };

    Type type() const;

    QStringList pathSpec() const;
    SDeviceItem currentDevice() const;
    const QString & output() const;

    void setProgressBar( int value , int maximum );

    int copiesNumber() const;
    int speed() const;
    bool eject() const;
    bool onFly() const;
    bool mount() const;
    bool multiSession() const;
    bool dummy() const;
    bool library() const;

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
    void setOnFly( bool stt );
    void setMount( bool stt );

    void setVolumeLabel( const QString & name );
    void setCopyRight( const QString & name );
    void setPublisher( const QString & name );
    void setApplicationId( const QString & name );
    void setSystemId( const QString & name );
    void setAbstract( const QString & name );
    void setBiblio( const QString & name );

    void add( const QString & file_or_folder );
    void addFiles( const QStringList & files );
    void addFolder( const QString & folder );
    void newFolder( const QString & name );
    void renameFiles( QString newName );

    void refresh();

signals:
    void reloaded();
    void busyRequest( bool stt );

    void go();


private slots:
    void folderChanged();
    void addressChanged();

    void addFiles();
    void addFolder();
    void newFolder();
    void renameFiles();
    void removeCurrent();
    void openFileDir();
    void showProperties();

    void reload();
    void reloaded_slt();
    void cdUp();
    void cdPrevious();
    void contextMenu( const QPoint & point );

    void deviceDetected( const SDeviceItem & device );
    void device_index_changed( int index );
    void image_selected( const QString & address );
    void image_select_accp( bool stt );

    void go_prev();
    void timer_finished( bool stt );

    void image_size_changed( int size );

protected:
    void closeEvent( QCloseEvent *event );

private:
    void init_actions();
    void init_system();

private:
    SelectFilesPrivate *p;
};

#endif // SELECTFILES_H

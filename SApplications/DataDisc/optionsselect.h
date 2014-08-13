#ifndef OPTIONSSELECT_H
#define OPTIONSSELECT_H

#include <QWidget>
#include <SDeviceItem>

class OptionsSelectPrivate;
class OptionsSelect : public QWidget
{
    Q_OBJECT
public:
    OptionsSelect( QWidget *parent = 0 );
    ~OptionsSelect();

    void expand();
    void unexpand();

    void setCurrentDevice( const SDeviceItem & device );

    void setImageBurn( bool stt );
    void setDiscBurn( bool stt );

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

public slots:
    void setScan( const QString & name );
    void setCopiesNumber( int value );
    void setSpeed( int value );
    void setEject( bool stt );
    void setDummy( bool stt );
    void setMultiSession( bool stt );
    void setOnFly( bool stt );
    void setMount( bool stt );

    void autoExpand();

signals:

private slots:
    void init_defaults();
    void animation_finished();

private:
    OptionsSelectPrivate *p;
};

#endif // OPTIONSSELECT_H

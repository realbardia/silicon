#ifndef AUTOMOUNT_H
#define AUTOMOUNT_H

#include <QString>
#include <QDir>
#include <QWidget>
#include <QObject>

class SAbstractImageMounter;
class AutoMount : public QObject
{
    Q_OBJECT
public:
    AutoMount( QObject *parent = 0 );
    AutoMount( SAbstractImageMounter *m , QObject *parent = 0 );

    void setMounter( SAbstractImageMounter *m );
    void setMasterPoint( const QString & path );

    void process( const QString & fileName );
    void process( const QString & fileName , const QString & mountPoint );

    QString masterPoint();
    QString readOutPut();

signals:

private slots:
    void mounted( bool stt );
    void unmounted( bool stt );
    void error( const QString & error );
    void processFinished( int exitCode );

private:
    SAbstractImageMounter *mounter;

    QString outStr;
    QString masterPointStr;
};

#endif // AUTOMOUNT_H

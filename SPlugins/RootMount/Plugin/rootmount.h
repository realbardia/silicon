#ifndef ROOTMOUNT_H
#define ROOTMOUNT_H

#include <QProcess>

#include <SAbstractImageMounter>
#include <SProcessEvent>

class RootMountPrivate;
class RootMount : public SAbstractImageMounter
{
    Q_OBJECT

public:
    RootMount( QObject *parent = 0 );
    ~RootMount();

    bool isMount( const QString & file ) const;

    QString mountPoint( const QString & file ) const;
    QString file( const QString & mountPoint ) const;

    QString usedCommand() const;
    QString application() const;
    QString errorStr() const;

    QStringList mountedList() const;

public slots:
    void mount( const QString & file , const QString & mount_point );
    void unmount( const QString & mount_point );
    void reload();

protected:
    virtual void goEvent( SProcessEvent *event );
    void stopEvent();

private slots:
    void finish( int exitCode , QProcess::ExitStatus exitStatus );
    void start_prev();

private:
    void read_mtab();
    void go_prev();

private:
    RootMountPrivate *p;
};

#endif // ROOTMOUNT_H

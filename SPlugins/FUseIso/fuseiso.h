#ifndef FUSEISO_H
#define FUSEISO_H

#include <QProcess>

#include <SAbstractImageMounter>
#include <SProcessEvent>

class FUseIsoPrivate;
class FUseIso : public SAbstractImageMounter
{
    Q_OBJECT

public:
    FUseIso( QObject *parent = 0 );
    ~FUseIso();

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
    void mount_finished( int exitCode , QProcess::ExitStatus exitStatus );
    void umount_finished( int exitCode , QProcess::ExitStatus exitStatus );

private:
    void read_mtab();
    void go_prev();

private:
    FUseIsoPrivate *p;
};

#endif // FUSEISO_H

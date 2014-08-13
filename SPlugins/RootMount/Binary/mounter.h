#ifndef MOUNTER_H
#define MOUNTER_H

#include <QObject>
#include <QProcess>

class MounterPrivate;
class Mounter : public QObject
{
    Q_OBJECT
public:
    Mounter(QObject *parent = 0);
    ~Mounter();

public slots:
    void mount( const QString & file , const QString & mount_point );

signals:

private slots:
    void mount_prev( const QString & file , const QString & mount_point );
    void finished( int exit_code , QProcess::ExitStatus state );

private:
    MounterPrivate *p;
};

#endif // MOUNTER_H

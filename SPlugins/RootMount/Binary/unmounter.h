#ifndef UNMOUNTER_H
#define UNMOUNTER_H

#include <QObject>
#include <QProcess>

class UnmounterPrivate;
class Unmounter : public QObject
{
    Q_OBJECT
public:
    Unmounter(QObject *parent = 0);
    ~Unmounter();

public slots:
    void unmount( const QString & mount_point );

signals:

private slots:
    void unmount_prev( const QString & mount_point );
    void finished( int exit_code , QProcess::ExitStatus state );

private:
    UnmounterPrivate *p;
};
#endif // UNMOUNTER_H

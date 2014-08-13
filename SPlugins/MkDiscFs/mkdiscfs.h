#ifndef MKDISKFS_H
#define MKDISKFS_H

#include <QObject>
#include <SAbstractDataBurner>
#include <SProcessEvent>

class MkDiscFsPrivate;
class MkDiscFs : public SAbstractDataBurner
{
    Q_OBJECT

public:
    MkDiscFs( QObject *parent = 0 );
    ~MkDiscFs();

    QString logs() const;
    QString usedCommand() const;

    QString application() const;

    STime elapsedTime() const;
    STime remainingTime() const;

    int imageSize() const;

private slots:
    void processOnOutput();
    void processOnError();
    void clockJobs();
    void stopTimer();
    void checkItemicLog( const QString & str );

protected:
    virtual void goEvent( SProcessEvent *event );
    void stopEvent();

    void pathSpecChanged( const QStringList & list );

private slots:
    void step_1();
    void step_2();
    void step_3();

    void sizeDetected();

private:
    QStringList arguments() const;
    QStringList isofsArguments() const;

    QString command() const;
    QString isofsCommand() const;

    void checkProgressLine( const QString & str );
    void reset();

private:
    MkDiscFsPrivate *p;
};

#endif // MKDISKFS_H

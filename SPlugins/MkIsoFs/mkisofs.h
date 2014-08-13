#ifndef MKISOFS_H
#define MKISOFS_H

#include <QObject>
#include <SAbstractImageCreator>
#include <SProcessEvent>

class MkIsoFsPrivate;
class MkIsoFs : public SAbstractImageCreator
{
    Q_OBJECT

public:
    MkIsoFs( QObject *parent = 0 );
    ~MkIsoFs();

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

    void sizeDetected();

private:
    QStringList arguments() const;
    QString command() const;

    void checkProgressLine( const QString & str );
    void reset();

private:
    MkIsoFsPrivate *p;
};

#endif // MKISOFS_H

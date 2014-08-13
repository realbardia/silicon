#ifndef IMAGEBURNER_H
#define IMAGEBURNER_H

#include <SAboutData>
#include <SApplication>
#include <STime>
#include <SAbstractProcess>

class ImageBurnerPrivate;
class ImageBurner : public SApplication
{
    Q_OBJECT
public:
    ImageBurner( const SAboutData & about );
    ~ImageBurner();

    void setWait( bool stt );
    bool wait() const;

public slots:
    void go();
    void stop();

private slots:
    void init_burner();
    void started();

    void showStopDialog();
    void stopDialogButtonClicked( int ret );

    void setLog( const QString & str );
    void setSpeed( double value );
    void setSize( int value );
    void setTime( STime elaps );
    void itemicLogAdded( SAbstractProcess::ItemicLogType type , const QString & message );

    void finish();

protected:
    void runtimeArgsEvent( const QVariantList & args );
    void closeEvent( QCloseEvent *event );

private:
    void startImageToDisc();

private:
    ImageBurnerPrivate *p;
};

#endif // IMAGEBURNER_H

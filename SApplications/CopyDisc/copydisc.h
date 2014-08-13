#ifndef COPYDISC_H
#define COPYDISC_H

#include <SAboutData>
#include <SApplication>
#include <STime>
#include <SAbstractProcess>

class CopyDiscPrivate;
class CopyDisc : public SApplication
{
    Q_OBJECT
public:
    CopyDisc( const SAboutData & about );
    ~CopyDisc();

    bool wait() const;

public slots:
    void setWait( bool stt );
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
    void non_onfly_finished();

    void DiscToImage();
    void SwitchToBurn();
    void ImageToDisc();

protected:
    void runtimeArgsEvent( const QVariantList &args );
    void closeEvent( QCloseEvent *event );

private:
    void startOnFly();
    void startNoOnFly();

private:
    CopyDiscPrivate *p;
};

#endif // COPYDISC_H

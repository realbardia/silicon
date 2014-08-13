#ifndef AUDIODISC_H
#define AUDIODISC_H

#include <SApplication>
#include <SAboutData>
#include <SAbstractProcess>

class AudioDiscPrivate;
class AudioDisc : public SApplication
{
    Q_OBJECT
public:
    AudioDisc( const SAboutData & about );
    ~AudioDisc();

    SProgressListItem *progressItem();

    bool wait() const;

public slots:
    void setWait( bool stt );
    void start();
    void stop();

    void showStopDialog();
    void stopDialogButtonClicked( int ret );

    void finished();

    void setLog( const QString & str );
    void setSpeed( double value );
    void setSize( int value );
    void percentChanged( int value );
    void setTime( const STime & elaps );
    void itemicLogAdded( SAbstractProcess::ItemicLogType type , const QString & message );

protected:
    void runtimeArgsEvent( const QVariantList &args );
    void closeEvent( QCloseEvent *event );

private slots:
    void step_1();  // Convert Files
    void step_2();  // Start Burning

private:
    void init_burner();
    void reset();

    SApplication *converter( const QString & input_mp3 , const QString & output_wav );

private:
    AudioDiscPrivate *p;
};

#endif // AUDIODISC_H

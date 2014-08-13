#ifndef DISCIMAGING_H
#define DISCIMAGING_H

#include <SAboutData>
#include <SApplication>
#include <STime>
#include <SAbstractProcess>

class DiscImagingPrivate;
class DiscImaging : public SApplication
{
    Q_OBJECT
public:
    DiscImaging( const SAboutData & about );
    ~DiscImaging();

    void setWait( bool stt );
    bool wait() const;

public slots:
    void go();
    void stop();

private slots:
    void init_reader();
    void started();

    void showStopDialog();
    void stopDialogButtonClicked( int ret );

    void setLog( const QString & str );
    void setSpeed( double value );
    void setTime( STime elaps );
    void itemicLogAdded( SAbstractProcess::ItemicLogType type , const QString & message );
    void sizeChanged( int value );

    void finish();

    void mount( const QString & fileName );
    void addToLibrary( const QString & path , const QString & name , const QStringList & tags );

protected:
    void runtimeArgsEvent( const QVariantList & args );
    void closeEvent( QCloseEvent *event );

private:
    void startImageToDisc();

private:
    DiscImagingPrivate *p;
};

#endif // DISCIMAGING_H

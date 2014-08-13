#ifndef DATADISC_H
#define DATADISC_H

#include <SApplication>
#include <SAboutData>

#include <STime>
#include <SAbstractProcess>

class SProgressListItem;
class DataDiscPrivate;
class DataDisc : public SApplication
{
    Q_OBJECT
public:
    DataDisc( const SAboutData & about );
    ~DataDisc();

    bool wait() const;
    void stop();

public slots:
    void setWait( bool stt );

private slots:
    void started();

    void start();
    void showStopDialog();
    void stopDialogButtonClicked( int ret );

    void setLog( const QString & str );
    void setSpeed( double value );
    void setSize( int value );
    void setTime( const STime & elaps );
    void itemicLogAdded( SAbstractProcess::ItemicLogType type , const QString & message );

    void finish_burn();
    void finish_create();

    void mount( const QString & fileName );
    void addToLibrary( const QString & path , const QString & name , const QStringList & tags );

protected:
    void runtimeArgsEvent( const QVariantList &args );
    void closeEvent( QCloseEvent *event );

private:
    void init_burner();
    void init_creator();
    void reset();

    void startOnFly();
    void startNoOnFly();
    void startImaging();

    void ImageToDisc();

    SProgressListItem *progressItem();

private:
    DataDiscPrivate *p;
};

#endif // DATADISC_H

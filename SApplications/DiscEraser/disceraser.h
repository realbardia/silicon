#ifndef DISCERASER_H
#define DISCERASER_H

#include <SAboutData>
#include <SApplication>
#include <STime>
#include <SAbstractProcess>

class DiscEraserPrivate;
class DiscEraser : public SApplication
{
    Q_OBJECT
public:
    DiscEraser( const SAboutData & about );
    ~DiscEraser();

    void setWait( bool stt );
    bool wait() const;

public slots:
    void go();
    void stop();

private slots:
    void init_eraser();
    void started();

    void showStopDialog();
    void stopDialogButtonClicked( int ret );

    void finish();

protected:
    void runtimeArgsEvent( const QVariantList & args );
    void closeEvent( QCloseEvent *event );

private:
    void StartErasing();

private:
    DiscEraserPrivate *p;
};

#endif // DISCERASER_H

#ifndef CONVERTERPAGE_H
#define CONVERTERPAGE_H

#include <SPage>

class SApplication;
class SConverter;
class ConverterPagePrivate;
class ConverterPage : public SPage
{
    Q_OBJECT
public:
    ConverterPage( SApplication *parent , SConverter *converter );
    ~ConverterPage();

public slots:
    void setSource( const QString & source = QString() );
    void setDestination( const QString & destination = QString() );

    void setSourceType( const QString & type );
    void setDestinationType( const QString & type );

signals:
    void go();

private slots:
    void more_prev();

    void updated();
    void start_timer();
    void start_prev( bool stt );

private:
    void init_actions();

private:
    ConverterPagePrivate *p;
};

#endif // CONVERTERPAGE_H

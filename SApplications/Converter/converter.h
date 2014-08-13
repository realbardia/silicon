#ifndef CONVERTER_H
#define CONVERTER_H

#include <SApplication>
#include <SAboutData>

class ConverterPrivate;
class Converter : public SApplication
{
    Q_OBJECT
public:
    Converter( const SAboutData & parent );
    ~Converter();

    void setWait( bool stt );
    bool wait() const;

signals:

public slots:
    void start();

protected:
    void runtimeArgsEvent( const QVariantList & args );

private slots:
    void finished( int exit_code );

private:
    SProgressListItem *progressItem();

private:
    ConverterPrivate *p;
};

#endif // CONVERTER_H

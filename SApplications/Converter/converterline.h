#ifndef CONVERTERLINE_H
#define CONVERTERLINE_H

#include <QWidget>

class SConverter;
class ConverterLinePrivate;
class ConverterLine : public QWidget
{
    Q_OBJECT
public:
    ConverterLine( SConverter *converter , QWidget *parent = 0 );
    ~ConverterLine();

    QString sourceType() const;
    QString destinationType() const;

    void setSourceType( const QString & type );
    void setDestinationType( const QString & type );

signals:
    void updated();

public slots:
    void refresh();

private slots:
    void update_slt();
    void src_changed();

protected:
    void paintEvent( QPaintEvent *event );

private:
    ConverterLinePrivate *p;
};

#endif // CONVERTERLINE_H

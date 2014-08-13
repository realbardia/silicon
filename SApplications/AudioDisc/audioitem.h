#ifndef AUDIOITEM_H
#define AUDIOITEM_H

#include <QWidget>

class AudioItemPrivate;
class AudioItem : public QWidget
{
    Q_OBJECT
public:
    AudioItem(QWidget *parent = 0);
    ~AudioItem();

    bool setAddress( const QString & address );
    QString address() const;

    QString title() const;
    QString artist() const;
    QString album() const;

    void setIcon( const QIcon & icon );
    const QIcon & icon() const;

public slots:
    void play();

signals:

private:
    AudioItemPrivate *p;
};

#endif // AUDIOITEM_H

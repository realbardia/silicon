#ifndef DISCMANAGER_H
#define DISCMANAGER_H

#include <QObject>

class SDiscInfo;
class SCatWidget;
class SInfoWidget;
class DiscManagerPrivate;

class DiscManager : public QObject
{
    Q_OBJECT
public:
    DiscManager( SCatWidget *cat_widget , SInfoWidget *info_widget , QObject *parent = 0 );
    ~DiscManager();

    void showDisc( const QString & disc_id );

signals:

private slots:
    void discsDetected( const QStringList & list );
    void discDetected(  const QString & disc_id  );

    void details_detected( const SDiscInfo & disc );

private:
    void init_discs();

private:
    DiscManagerPrivate *p;
};

#endif // DISCMANAGER_H

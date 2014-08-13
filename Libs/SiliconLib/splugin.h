#ifndef SPLUGIN_H
#define SPLUGIN_H

#include <QObject>
#include <QString>
#include <QIcon>

#include "libsilicon_global.h"

class SPluginPrivate;

class LIBSILICON_EXPORT SPlugin : public QObject
{
    Q_OBJECT
public:
    SPlugin( const QIcon & icon , const QString & name , const QString & description , QObject *parent = 0 );
    ~SPlugin();

    void setDepends(   const QStringList & plugins_names );
    void setConflicts( const QStringList & plugins_names );

    const QString & name() const;
    const QString & description() const;
    const QIcon & icon() const;

    const QStringList & depends() const;
    const QStringList & conflicts() const;

    bool start();
    bool stop();

    bool isStarted() const;

signals:

public slots:

protected:
    virtual bool startEvent();
    virtual bool stopEvent();

private:
    void setStarted( bool stt );

private:
    SPluginPrivate *p;
};

#endif // SPLUGIN_H

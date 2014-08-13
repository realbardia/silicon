#include "splugin.h"

class SPluginPrivate
{
public:
    QIcon icon;
    QString name;
    QString description;

    QStringList depends;
    QStringList conflicts;

    bool is_started;
};

SPlugin::SPlugin( const QIcon & icon , const QString & name , const QString & description , QObject *parent ) :
    QObject(parent)
{
    p = new SPluginPrivate;

    p->icon = icon;
    p->name = name;
    p->description = description;

    p->is_started = false;
}

void SPlugin::setDepends(   const QStringList & plugins_names )
{
    p->depends = plugins_names;
}

void SPlugin::setConflicts( const QStringList & plugins_names )
{
    p->conflicts = plugins_names;
}

const QString & SPlugin::name() const
{
    return p->name;
}

const QString & SPlugin::description() const
{
    return p->description;
}

const QIcon & SPlugin::icon() const
{
    return p->icon;
}

const QStringList & SPlugin::depends() const
{
    return p->depends;
}

const QStringList & SPlugin::conflicts() const
{
    return p->conflicts;
}

bool SPlugin::isStarted() const
{
    return p->is_started;
}

bool SPlugin::startEvent()
{
    return false;
}

bool SPlugin::stopEvent()
{
    return false;
}

void SPlugin::setStarted( bool stt )
{
    p->is_started = stt;
}

SPlugin::~SPlugin()
{
    delete p;
}

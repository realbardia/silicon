#ifndef SPLUGINENGINE_H
#define SPLUGINENGINE_H

#include <QString>
#include <QLibrary>

#include <SPlugin>

class SPluginEngine
{
public:
    SPluginEngine();
    SPluginEngine( const QString & fileName );
    ~SPluginEngine();

    void setFileName( const QString & fileName );

    bool isPlugin();
    SPlugin *plugin();

private:
    QString file;

    typedef SPlugin *(*pluginProto)();
    pluginProto pluginFunction;

    QLibrary library;
};

#endif // SPLUGINENGINE_H

#ifndef SAPPLICATIONENGINE_H
#define SAPPLICATIONENGINE_H

#include <QWidget>
#include <QString>
#include <QLibrary>
#include <QStringList>

#include <saboutdata.h>
#include <sapplication.h>

class SApplicationEngine
{
public:
    SApplicationEngine();
    SApplicationEngine(const QString & fileName);
    ~SApplicationEngine();

    void setFileName(const QString & fileName);

    bool isApplication();
    bool hasConfigure();

    int application( const QVariantList &args );
    QWidget *configure();
    SAboutData about();

protected:
    bool isAccessName(QString name);

private:
    QString file;

    typedef int (*appProto)( const QVariantList & args );
    typedef QWidget *(*confProto)();
    typedef SAboutData (*aboutProto)();

    appProto appFunction;
    confProto confFunction;
    aboutProto aboutFunction;

    QLibrary library;
};

#endif // SAPPLICATIONENGINE_H

#ifndef SILICONSOLE_H
#define SILICONSOLE_H

#include <QList>
#include <QString>
#include <QDir>
#include <QStringList>

#include <sapplication.h>
#include <saboutdata.h>

#include "sdefaults.h"
#include "sapplicationengine.h"

class siliconsole
{
public:
    siliconsole();

protected:
    void detectApps();
    bool isAdded( SAboutData about );

private:
    QList<SAboutData> appsList;

    SDefaults defaults;

};

#endif // SILICONSOLE_H

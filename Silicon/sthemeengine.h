#ifndef STHEMEENGINE_H
#define STHEMEENGINE_H

#include <QString>
#include <QColor>
#include <scolor.h>
#include <QWidget>

#include <iostream>

#include "sdefaults.h"
#include "perconf.h"

class SThemeEngine
{
public:
    SThemeEngine( PerConf *conf );

    void set(const QString & themeStr);
    QString translate(const QString & theme);
protected:
    QColor readColor(const QString & str);
    SColor doColorOperate(SColor color,const QString operate);

private:
    QString theme;
    QWidget parent;

    SDefaults defaults;
    PerConf *prc;
};

#endif // STHEMEENGINE_H

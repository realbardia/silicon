#ifndef SAPPLICATIONSCONFIGURE_H
#define SAPPLICATIONSCONFIGURE_H

#include <QWidget>
#include <QHash>
#include <QPalette>

#include <SColorButton>

#include "perconf.h"
#include "sdefaults.h"
#include "stabbar.h"

namespace Ui
{
    class SApplicationsConfigureUI;
}

class SiliconDataBox;
class SApplicationsConfigure : public QWidget
{
    Q_OBJECT
public:
    SApplicationsConfigure( perConf *conf = 0 , STabBar *tabbar = 0 , SiliconDataBox *d_box = 0 , QWidget *parent = 0);

signals:
    void tempChanged( const QString & tmp );

public slots:

private slots:
    void colorChanged( SColorButton *button );
    void setColors( int type , const QString & color );
    void setTemp();

private:
    void setupColors();
    void setup();

private:
    Ui::SApplicationsConfigureUI *ui;

    QHash<int,SColorButton *> colors_hash;
    QHash<int,QPalette::ColorRole> colors_role;

    QPalette default_palette;

    SDefaults defaults;
    perConf *prc;

    STabBar *tab_bar;
    SiliconDataBox *data_box;
};

#endif // SAPPLICATIONSCONFIGURE_H

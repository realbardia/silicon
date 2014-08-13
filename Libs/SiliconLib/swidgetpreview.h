#ifndef SWIDGETPREVIEW_H
#define SWIDGETPREVIEW_H

#include "libsilicon_global.h"

#include <QWidget>

class SWidgetPreviewPrivate;
class LIBSILICON_EXPORT SWidgetPreview : public QWidget
{
    Q_OBJECT
public:
    SWidgetPreview( QWidget *parent = 0 );
    ~SWidgetPreview();

    void setWidget( QWidget *widget );
    QWidget *widget() const;

    void setMonitoring( int ms );
    int monitoring() const;

signals:

private slots:
    void widgetDestroyed();

protected:
    void timerEvent( QTimerEvent *event );
    void paintEvent( QPaintEvent *event );

private:
    SWidgetPreviewPrivate *p;
};

#endif // SWIDGETPREVIEW_H

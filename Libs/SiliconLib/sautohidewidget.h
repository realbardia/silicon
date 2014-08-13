#ifndef SAUTOHIDEWIDGET_H
#define SAUTOHIDEWIDGET_H

#include "libsilicon_global.h"

#include <QWidget>

class SAutoHideWidgetPrivate;
class LIBSILICON_EXPORT SAutoHideWidget : public QWidget
{
    Q_OBJECT
public:
    SAutoHideWidget(QWidget *parent = 0);
    ~SAutoHideWidget();

    void setWidget( QWidget *widget );
    QWidget *widget() const;

    void setHiddenTimer( int ms );
    int hiddenTimer() const;

    bool isActive() const;
    bool isDeactive() const;

public slots:
    void showWidget();
    void hideWidget();

    void setActive( bool stt );
    void setDeactive( bool stt );

signals:

private slots:
    void widgetDestroyed();

protected:
    void resizeEvent( QResizeEvent *event );
    void enterEvent( QEvent *event );
    void leaveEvent( QEvent *event );

private:
    SAutoHideWidgetPrivate *p;
};

#endif // SAUTOHIDEWIDGET_H

#ifndef TIMEWIDGETMANAGER_H
#define TIMEWIDGETMANAGER_H

#include <QMainWindow>
#include <QVariantList>

#include "libsilicon_global.h"

class SWidgetManagerPrivate;
class LIBSILICON_EXPORT SWidgetManager : public QMainWindow
{
    Q_OBJECT
public:
    SWidgetManager( QWidget *parent = 0);
    ~SWidgetManager();

    enum ResizeMode{
        PIXEL,
        PERCENT
    };

    int count() const;

    void refresh();

    QWidget *at( int index ) const;
    int indexOf( QWidget *widget ) const;

    void move( int from , int to );

    void remove( QWidget *widget );
    void removeAt( int index );
    QWidget *takeAt( int index );

    bool widgetIsVisible( QWidget *widget ) const;
    bool widgetIsHidden( QWidget *widget ) const;

    void setSideBarWidth( int width );
    int sideBarWidth() const;

    void setShowButtonHeight( int height );
    int showButtonHeight() const;

    void setAnimationFrameRate( int rate );
    int animationFrameRate() const;

    void setAnimationDuration( int dur );
    int animationDuration() const;

public slots:
    void addWidget( QWidget *widget , int default_height = 1 , ResizeMode mode = PERCENT );
    void insertWidget( int index , QWidget *widget , int default_height = 1 , ResizeMode mode = PERCENT );

    void hideWidget( QWidget *widget );
    void showWidget( QWidget *widget );

    void switchWidgetVisiblity( QWidget *widget );

protected:
    void resizeEvent( QResizeEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void timerEvent( QTimerEvent *event );
    void paintEvent( QPaintEvent *event );
    
private:
    SWidgetManagerPrivate *p;
};

#endif // TIMEWIDGETMANAGER_H

#ifndef SWIDGETLIST_PREV_H
#define SWIDGETLIST_PREV_H

#include <QWidget>


class SWidgetListItemPrivate;
class SWidgetListItem : public QWidget
{
    Q_OBJECT
public:
    SWidgetListItem( QWidget *widget );
    ~SWidgetListItem();

    bool isChecked() const;

public slots:
    void setChecked( bool stt );

signals:
    void checked( bool stt );
    void selected( SWidgetListItem *item );
    void clicked( SWidgetListItem *item );
    void doubleClicked( SWidgetListItem *item );

    void moved( SWidgetListItem *item );
    void movingFinished( SWidgetListItem *item );


protected:
    void enterEvent( QEvent *event );
    void leaveEvent( QEvent *event );
    void paintEvent( QPaintEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseDoubleClickEvent( QMouseEvent *event );

private:
    SWidgetListItemPrivate *p;
};


class ViewPortWidgetPrivate;
class ViewPortWidget : public QWidget
{
    Q_OBJECT
public:
    ViewPortWidget( const QList<SWidgetListItem*> & list , QWidget *parent = 0 );
    ~ViewPortWidget();

signals:
    void clicked();

protected:
    void paintEvent( QPaintEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

private:
    ViewPortWidgetPrivate *p;
};

#endif // SWIDGETLIST_PREV_H

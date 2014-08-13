#ifndef DIALOGWIDGET_H
#define DIALOGWIDGET_H

#include <QWidget>
#include <SDialog>

class DialogWidgetPrivate;
class DialogWidget : public QWidget
{
    Q_OBJECT
public:
    DialogWidget(QWidget *parent = 0);
    ~DialogWidget();

    void set( SDialog *dialog );

protected:
    void paintEvent( QPaintEvent *event );
    void mousePressEvent( QMouseEvent *event );
    void mouseMoveEvent( QMouseEvent *event );
    void mouseReleaseEvent( QMouseEvent *event );

private slots:
    void anim_ended();
    void windowedDialogClosed( SDialog *dialog );

private:
    void setWindowedPageDialog( SDialog *dialog );
    void setTabbedPageDialog( SDialog *dialog );

private:
    DialogWidgetPrivate *p;
};

#endif // DIALOGWIDGET_H

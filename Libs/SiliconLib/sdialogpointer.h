#ifndef SDIALOGPOINTER_H
#define SDIALOGPOINTER_H

#include "libsilicon_prev.h"
#include <QDialog>

class SDialogPointerPrivate;
class LIBSILICON_EXPORT SDialogPointer : public QDialog
{
    Q_OBJECT
public:
    enum Type {
        Horizontal = 0x1,
        Vertical = 0x2
    };

    SDialogPointer(QWidget *parent = 0);
    ~SDialogPointer();

    void setRounded( int radius );
    int rounded() const;

    void setAlpha( int alpha );
    int alpha() const;

    void setPointerSize( const QSize & size );
    const QSize & pointerSize() const;

    void setPadding( int pad );
    int padding() const;

    void setWidget( QWidget *widget );
    QWidget *widget() const;

    const QPoint & pointedTo() const;

    void setType( Type type );
    Type type() const;

public slots:
    void pointTo( const QPoint & point );
    void setVisible( bool visible );

signals:

protected:
    void paintEvent( QPaintEvent *event );
    void resizeEvent( QResizeEvent *event);

private slots:
    void widgetDestroyed();
    void focusChanged( QWidget *old , QWidget *now );

private:
    void init_pos();
    
private:
    SDialogPointerPrivate *p;
};

#endif // SDIALOGPOINTER_H

#ifndef SINNERMESSAGE_H
#define SINNERMESSAGE_H

#include <QWidget>
#include <QMessageBox>
#include <QSize>
#include <QPaintEvent>

#include "libsilicon_global.h"

class SInnerMessagePrivate;

class LIBSILICON_EXPORT SInnerMessage : public QWidget
{
    Q_OBJECT
public:
    SInnerMessage( QMessageBox *messageBox , bool removeInDecunstructor = true , QWidget *parent = 0 );
    ~SInnerMessage();

    void setAnimation( bool stt );
    bool animation() const;

    void showMessage( const QSize & size );

    void setColor( const QColor & color );
    const QColor & color() const;

    QMessageBox *messageBox() const;

public slots:
    void hideMessage();

signals:

private slots:
    void animation_finished();

protected:
    void paintEvent( QPaintEvent *event );

private:
    SInnerMessagePrivate *p;
};

#endif // SINNERMESSAGE_H

#ifndef SGRAPHICSTACK_H
#define SGRAPHICSTACK_H

#include <QWidget>
#include <QStringList>
#include <QString>
#include <QPaintEvent>
#include <QToolButton>

#include "libsilicon_global.h"

class SGraphicStackItem;
class SGraphicStackPrivate;
class  LIBSILICON_EXPORT SGraphicStack : public QWidget
{
    Q_OBJECT
public:
    SGraphicStack( QWidget *parent = 0 );
    ~SGraphicStack();

    void push( const QString & str );
    void push( const QIcon & icon , const QString & str );

    QString pop();

    QString top() const;
    QStringList toStringList() const;
    QStringList allToStringList() const;

    int count() const;
    bool isEmpty() const;

    int currentIndex() const;
    QString current() const;

    QString at( int index );

public slots:
    void setCurrentIndex( int index , bool signal_on = true );
    void clear();

signals:
    void currentIndexChanged( int index );
    void currentChanged( const QString & str );

private slots:
    void clicked( SGraphicStackItem *item );

private:
    void recheck();

private:
    SGraphicStackPrivate *p;
};


class SGraphicStackItem : public QToolButton
{
    Q_OBJECT
public:
    SGraphicStackItem( const QString & text , QWidget *parent = 0 );
    ~SGraphicStackItem();

private slots:
    void click_on();

signals:
    void clicked( SGraphicStackItem *item );
};

#endif // SGRAPHICSTACK_H

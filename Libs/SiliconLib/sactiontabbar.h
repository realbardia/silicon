#ifndef SACTIONTABBAR_H
#define SACTIONTABBAR_H

#include <QWidget>
#include <QIcon>
#include <QAction>

#include "libsilicon_global.h"

class SActionTabBarPrivate;

class LIBSILICON_EXPORT SActionTabBar : public QWidget
{
    Q_OBJECT

public:
    SActionTabBar(QWidget *parent = 0);
    ~SActionTabBar();

    QAction *add( const QIcon & icon , const QString & name );
    void add( QAction *action );

    QAction *insert( int index , const QIcon & icon , const QString & name );
    void insert( int index , QAction *action );

    void removeAt( int index );
    void removeOne( QAction *action );

    int count() const;
    void clear();

    bool isEmpty() const;

    int currentIndex() const;
    QAction *currentItem() const;

    QAction *itemAt( int index ) const;
    int indexOf( QAction *action ) const;

    void setOrientation( Qt::Orientation orientation );
    Qt::Orientation orientation() const;

public slots:
    void setCurrentIndex( int index );

signals:
    void tabAdded();
    void currentIndexChanged( int index );

private slots:
    void actionTriggered( QAction *action );

private:
    SActionTabBarPrivate *p;
};

#endif // SACTIONTABBAR_H

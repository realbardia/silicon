#ifndef MENUBARMANAGER_H
#define MENUBARMANAGER_H

#include <QObject>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QList>
#include <QHash>

#include "smenupanel.h"

class MenuBarManager : public QObject
{
    Q_OBJECT
public:
    MenuBarManager( QMenuBar *menuBar , QObject *parent = 0);

    void addAction( QAction *action );
    QAction *addMenu( const QString & name , QMenu *menu );
    QAction *addMenu( const QString & name , SMenuPanel *menu );

    void insertAction( int index , QAction *action );
    QAction *insertMenu( int index , const QString & name , QMenu *menu );
    QAction *insertMenu( int index , const QString & name , SMenuPanel *menu );

    void remove( int index );
    void remove( QAction *action );

    void clear();

    QAction *at( int index );
    int index( QAction *action );

    int count();

signals:

private slots:
    void update( SMenuPanel *menu );

private:
    QMenuBar *menubar;
    QList<QAction *> list;
    QHash<SMenuPanel *,QMenu *> hash;
};

#endif // MENUBARMANAGER_H

#include "menubarmanager.h"

MenuBarManager::MenuBarManager( QMenuBar *menuBar , QObject *parent ) :
    QObject(parent),
    menubar( menuBar )
{
    menubar->clear();
}

void MenuBarManager::addAction( QAction *action )
{
    list << action;
    menubar->addAction( action );
}

QAction *MenuBarManager::addMenu( const QString & name , QMenu *menu )
{
    QAction *action = new QAction( name , this );
        action->setMenu( menu );

    this->addAction( action );

    return action;
}

QAction *MenuBarManager::addMenu( const QString & name , SMenuPanel *menu )
{
    QMenu *mn = new QMenu( name );

    for( int i=0 ; i<menu->count() ; i++ )
        mn->addAction( menu->at(i) );

    hash.insert( menu , mn );
    QObject::connect( menu , SIGNAL(updated(SMenuPanel*)) , SLOT(update(SMenuPanel*)) );

    return this->addMenu( name , mn );
}

void MenuBarManager::insertAction( int index , QAction *action )
{
    QAction *before = 0;
    if( index != 0 )
        before = list.at( index-1 );

    list.insert( index , action );
    menubar->insertAction( before , action );
}

QAction *MenuBarManager::insertMenu( int index , const QString & name , QMenu *menu )
{
    QAction *action = new QAction( name , this );
        action->setMenu( menu );

    this->insertAction( index , action );

    return action;
}

QAction *MenuBarManager::insertMenu( int index , const QString & name , SMenuPanel *menu )
{
    QMenu *mn = new QMenu( name );

    for( int i=0 ; i<menu->count() ; i++ )
        mn->addAction( menu->at(i) );

    hash.insert( menu , mn );
    QObject::connect( menu , SIGNAL(updated(SMenuPanel*)) , SLOT(update(SMenuPanel*)) );

    return this->insertMenu( index , name , mn );
}

void MenuBarManager::remove( int index )
{
    this->remove( list.at(index) );
}

void MenuBarManager::remove( QAction *action )
{
    menubar->removeAction( action );
    list.removeOne( action );
}

void MenuBarManager::clear()
{
    list.clear();
    menubar->clear();
}

QAction *MenuBarManager::at( int index )
{
    return list.at( index );
}

int MenuBarManager::index( QAction *action )
{
    return list.indexOf( action );
}

int MenuBarManager::count()
{
    return list.count();
}

void MenuBarManager::update( SMenuPanel *menu )
{
    QMenu *mn = hash.value( menu );
        mn->clear();

    for( int i=0 ; i<menu->count() ; i++ )
        mn->addAction( menu->at(i) );
}

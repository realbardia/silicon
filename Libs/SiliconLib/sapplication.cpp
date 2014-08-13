/*
 * File:      sapplication.cpp
 * Author:    Bardia Daneshvar (bardia@getsilicon.org)
 * Copyright: (C) 2010 Bardia Daneshvar
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "sapplication.h"

#include <QListWidgetItem>
#include <QApplication>
#include <QProcess>
#include <QDir>
#include <QList>

#include "sprogresslistitem.h"
#include "sinnermessage.h"
#include "sinnerprogressbar.h"
#include "scolor.h"




/*================================*
 |                                |
 |      SApplicationPrivate       |
 |                                |
 *================================*/

class SApplicationPrivate
{
public:
    SAboutData parent;
    QList<SPage *> childs;
    QList<SProgressListItem *> progress_list;
    bool quit_on_last_page_closed;
};




/*================================*
 |                                |
 |          SApplication          |
 |                                |
 *================================*/

SApplication::SApplication( const SAboutData & parent )
{
    p = new SApplicationPrivate;
        p->parent = parent;
        p->quit_on_last_page_closed = true;

    this->registerToSilicon();
}

const SAboutData & SApplication::parent() const
{
    return p->parent;
}

void SApplication::addRuntimeArgs( const QVariantList & args )
{
    runtimeArgsEvent( args );
    emit runtimeArguments( args );
}

void SApplication::addChild( SPage *page )
{
    p->childs << page;
    QObject::connect( page , SIGNAL(destroyed(SPage*)) , SLOT(pageDestroyed(SPage*)) );
}

const QList<SPage *> & SApplication::childs() const
{
    return p->childs;
}

int SApplication::numberOfAllChilds() const
{
    int result = p->childs.count();
    for( int i=0 ; i<p->childs.count() ; i++ )
        result += p->childs.at(i)->numberOfAllChilds();

    return result;
}

int SApplication::numberOfAllSubObjects() const
{
    int result = p->childs.count();
    for( int i=0 ; i<p->childs.count() ; i++ )
        result += p->childs.at(i)->numberOfAllSubObjects();

    return result;
}

void SApplication::addProgress( SProgressListItem *item )
{
    p->progress_list << item;
    QObject::connect( item , SIGNAL(destroyed(SProgressListItem*)) , SLOT(progressDestroyed(SProgressListItem*)) );
}

const QList<SProgressListItem *> & SApplication::progresses() const
{
    return p->progress_list;
}

void SApplication::setQuitOnLastPageClosed( bool stt )
{
    p->quit_on_last_page_closed = stt;
}

bool SApplication::quitOnLastPageClosed() const
{
    return p->quit_on_last_page_closed;
}

void SApplication::pageDestroyed( SPage *page )
{
    p->childs.removeOne( page );
    if( p->quit_on_last_page_closed && p->childs.isEmpty() )
        this->quit();
}

void SApplication::progressDestroyed( SProgressListItem *item )
{
    p->progress_list.removeOne( item );
}

void SApplication::focus()
{
    this->FocusedEvent();
}

void SApplication::runtimeArgsEvent( const QVariantList & )
{

}

void SApplication::closeEvent( QCloseEvent *event )
{
    event->accept();
}

void SApplication::FocusedEvent()
{
    if( p->childs.isEmpty() )
        return ;

    p->childs.first()->show();
}

SApplication::~SApplication()
{
    while( !p->childs.isEmpty() )
        delete p->childs.first();

    while( !p->progress_list.isEmpty() )
        delete p->progress_list.first();

    emit this->destroyed( this );

    delete p;
}

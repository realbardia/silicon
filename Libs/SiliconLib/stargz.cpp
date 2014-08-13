/*
 * File:      stargz.cpp
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

#include "stargz.h"

#include <limits.h>



/*================================*
 |                                |
 |         STarGzPrivate          |
 |                                |
 *================================*/

class STarGzPrivate
{
public:
    QString file;           // Name of tar.gz File
    QString command;        // command name , mostly command string is "tar"

    QProcess *process;      // Process
};



/*================================*
 |                                |
 |             STarGz             |
 |                                |
 *================================*/

STarGz::STarGz( QObject *parent ) : QObject( parent )
{
    this->setup();
}

STarGz::STarGz( const QString & fileName , QObject *parent ) : QObject( parent )
{
    this->setup();
    this->setFile( fileName );
}

STarGz::~STarGz()
{
    delete p;
}

void STarGz::setFile( const QString & fileName )
{
    p->file = fileName ;
}

QStringList STarGz::list()
{
    p->process->start( p->command , QStringList()<<"tzf"<<p->file );
    p->process->waitForFinished( INT_MAX );

    QStringList otput = QString( p->process->readAll() ).split('\n');
    return otput;
}

void STarGz::extractTo( const QString & )
{
    p->process->start( p->command , QStringList()<<"xzvf"<<p->file );
}

void STarGz::setup()
{
    p = new STarGzPrivate;
        p->command = "tar";
        p->process = new QProcess( this->parent() );
}

/*
 * File:      srootprocess.cpp
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

#include <QProcess>
#include <QDebug>

#include "srootprocess.h"



/*================================*
 |                                |
 |       SRootProcessPrivate      |
 |                                |
 *================================*/

class SRootProcessPrivate
{
public:
    QString su_command;
    QString sudo_command;

    QProcess *process;
};



/*================================*
 |                                |
 |          SRootProcess          |
 |                                |
 *================================*/

SRootProcess::SRootProcess( QObject *parent ) : QObject( parent )
{
    p = new SRootProcessPrivate;
    p->process = new QProcess( this );
        p->process->setProcessChannelMode( QProcess::MergedChannels );

    p->su_command = "su";
    p->sudo_command = "sudo";
}

void SRootProcess::start( const QString & program, const QStringList & arguments , Mods mode )
{
    QString command;
    QStringList args;

    switch( mode )
    {
        case SRootProcess::SUDO :
            command = p->sudo_command;
            args << program << arguments;
            break;
        case SRootProcess::SU :
            command = p->su_command;
            args << "-p" << "-c" << QString() + "'" + program + " " + arguments.join(" ") + "'" ;
            break;
    }


    p->process->setEnvironment( QProcess::systemEnvironment() );
    p->process->start( command , args );
    p->process->waitForReadyRead( 10000 );

    qDebug() << p->process->environment().join("\n");
    qDebug() << QString(p->process->readAll());
}

SRootProcess::~SRootProcess()
{
    delete p;
}

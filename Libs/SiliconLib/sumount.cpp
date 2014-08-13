/*
 * File:      sumount.cpp
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

#include "sumount.h"

#include <QDir>
#include <QStringList>
#include <QProcess>

#include <limits.h>


/*================================*
 |                                |
 |        SUmountPrivate          |
 |                                |
 *================================*/

class SUmountPrivate
{
public:
    QString MountPoint;
    QString Output;

    QProcess *process;
};



/*================================*
 |                                |
 |            SUmount             |
 |                                |
 *================================*/

SUmount::SUmount( QObject *parent ) : QObject( parent )
{
    p = new SUmountPrivate;
    p->process = new QProcess( this );
}

SUmount::SUmount( const QString & path , QObject *parent ) : QObject( parent )
{
    p = new SUmountPrivate;
    p->process = new QProcess( this );

    this->setMountPoint( path );
}

void SUmount::reset()
{
    p->MountPoint.clear();
}

void SUmount::setMountPoint(const QString & path)
{
    p->MountPoint = path;
}

const QString & SUmount::getOutput() const
{
    return p->Output;
}

bool SUmount::umount()
{
    if( p->MountPoint.isEmpty() )   // If MountPoint not set before , exit function
        return false;

// Start unmount process and finally read all standard error and outputs =============//

    QString app( "umount" );
    QStringList arguments;
        arguments<< "-v" << p->MountPoint;

    p->process->start( app , arguments );
    p->process->waitForFinished( INT_MAX );

    p->Output = p->process->readAllStandardError() + p->process->readAllStandardOutput();

// END ====================//


    if( p->process->exitCode() == 0)
    {
        QDir().rmdir( p->MountPoint );
        p->Output = "File unmounted from " + p->MountPoint + " successfully\n";
        return true;
    }
    else
        return false;
}

SUmount::~SUmount()
{
    delete p;
}

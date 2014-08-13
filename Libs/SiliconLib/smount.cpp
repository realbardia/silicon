/*
 * File:      smount.cpp
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

#include "smount.h"

#include <QDir>
#include <QStringList>
#include <QProcess>
#include <limits.h>




/*================================*
 |                                |
 |          SMountPrivate         |
 |                                |
 *================================*/

class SMountPrivate
{
public:
    QString MountPoint;
    QString Address;
    QString Output;

    QProcess *process;
};




/*================================*
 |                                |
 |             SMount             |
 |                                |
 *================================*/

SMount::SMount( QObject *parent )
    : QObject( parent )
{
    p = new SMountPrivate;

    this->reset();
    p->process = new QProcess( this );
}

SMount::SMount( const QString & Imagepath , QObject *parent )
    : QObject( parent )
{
    p = new SMountPrivate;

    this->reset();
    this->setAddress(Imagepath);

    p->process = new QProcess( this );
}

SMount::SMount( const QString & Imagepath , const QString & Mountpath , QObject *parent )
    : QObject( parent )
{
    p = new SMountPrivate;

    this->reset();
    this->setAddress(Imagepath);
    this->setMountPoint(Mountpath);

    p->process = new QProcess( this );
}

SMount::~SMount()
{
    delete p;
}

void SMount::reset()
{
    p->Address.clear();
    p->MountPoint.clear();
    p->Output.clear();
}

void SMount::setAddress(const QString & path)
{
    p->Address = path;
}

void SMount::setMountPoint(const QString & path)
{
    p->MountPoint = path;
}

QString SMount::getOutput() const
{
    return p->Output;
}

bool SMount::mount()
{
    if(p->Address.isEmpty() || p->MountPoint.isEmpty())
        return false;

    QDir().mkpath( p->MountPoint );

    QString app("mount");
    QStringList arguments;
    arguments<<"-v"<<"-o"<<"loop"<<p->Address<<p->MountPoint;

    p->process->start(app,arguments);
    p->process->waitForFinished(INT_MAX);

    p->Output= p->process->readAllStandardError() + p->process->readAllStandardOutput();

    if( p->process->exitCode()==0)
    {
        p->Output+=p->Address+" mounted on "+p->MountPoint+" successfully"+"\n";
        return true;
    }
    else
    {
        QDir().rmdir( p->MountPoint );
        return false;
    }
}

/*
 * File:      smastericons.cpp
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

#include "smastericons.h"

#include <QHash>
#include <QFileInfo>
#include <QDir>

#ifdef Q_OS_LINUX
static bool from_system_theme = true;
#else
static bool from_system_theme = false;
#endif

static SMasterIcons *locked_on = 0;         // Locker SMasterIcons Object
static QString current_path;                // Current Path
static QHash<QString,QString> hash;         // < "icon-size/icon-name" , "icon-Address" >




/*================================*
 |                                |
 |          SMasterIcons          |
 |                                |
 *================================*/

SMasterIcons::SMasterIcons( const QString & path )
{
    this->setPath( path );
}

void SMasterIcons::setPath( const QString & path )
{
    if( this->accessToChange() )
    {
        current_path = path;
        this->reload();
    }
    else
        qCritical( "SMasterIcons::setPath() : is Locked" );
}

void SMasterIcons::reload()
{
    hash.clear();

    QDir path_dir( current_path );
    QStringList size_dir_list = path_dir.entryList( QStringList()<<"*" , QDir::Dirs | QDir::NoDotAndDotDot );

    for( int i=0 ; i<size_dir_list.count() ; i++ )
    {
        QDir size_dir( current_path + '/' + size_dir_list.at(i) );
        QStringList dir_list = size_dir.entryList( QStringList()<<"*" , QDir::Dirs | QDir::NoDotAndDotDot );

        for( int j=0 ; j<dir_list.count() ; j++ )
        {
            QDir dir( size_dir.path() + '/' + dir_list.at(j) );
            QStringList file_list = dir.entryList( QDir::Files );

            for( int k=0 ; k<file_list.count() ; k++ )
                hash.insert( size_dir.dirName() + '/' + file_list.at(k) , dir.path() + '/' + file_list.at(k) );
        }
    }

}

void SMasterIcons::lock()
{
    if( !this->accessToChange() )
    {
        qCritical( "SMasterIcons::setPath() : is Locked" );
        return ;
    }

    locked_on = this;
}

void SMasterIcons::unlock()
{
    if( !this->accessToChange() )
    {
        qCritical( "SMasterIcons::setPath() : is Locked" );
        return ;
    }

    locked_on = 0;
}

bool SMasterIcons::isLocked() const
{
    return ( locked_on != 0 );
}

bool SMasterIcons::isUnLocked() const
{
    return !this->isLocked();
}

bool SMasterIcons::accessToChange() const
{
    return ( locked_on == this ||  locked_on == 0 );
}

void SMasterIcons::setFromSystemTheme( bool stt )
{
    if( !this->accessToChange() )
    {
        qCritical( "SMasterIcons::setFromSystemTheme() : is Locked" );
        return ;
    }

    from_system_theme = stt;
}

QIcon SMasterIcons::icon( const QSize & size , const QString & name )
{
    QString size_folder( QString::number(size.width()) + "x"
                         + QString::number(size.height()) );

    QIcon result;

    if( from_system_theme )
        result = QIcon::fromTheme( name.left( name.size()-4 ) );

    if( result.isNull() )
        result = QIcon( hash.value( size_folder + '/' + name ) );

    return result;
}

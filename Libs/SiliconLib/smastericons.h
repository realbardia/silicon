/*
 * File:      smastericons.h
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

#ifndef SMASTERICONS_H
#define SMASTERICONS_H

#include <QString>
#include <QSize>
#include <QIcon>

#include "libsilicon_global.h"

/*! \class
 * The SMasterIcons provides an master icons base class.
 * MasterIcons give a folder path and scan this path for find icons.
 * Then Any Object can use icons with SMasterIcons::icon .
 *
 * path type : Path/Size/icons or icons folder
 *
 * \example path = /home/brdn/icons
 * icons folder containt : 22x22/ , 48x48/ , 128x128/ , 22x66/
 */
class LIBSILICON_EXPORT SMasterIcons
{

/*! List of All Available Functions :
 *
 *    SMasterIcons( const QString & path = 0 );
 *
 *    void setPath( const QString & path );
 *    void reload();
 *
 *    void lock();
 *    void unlock();
 *
 *    bool isLocked() const;
 *    bool isUnLocked() const;
 *    bool accessToChange() const;
 *
 *    void setFromSystemTheme( bool stt );
 *
 *    static QIcon icon( const QSize & size , const QString & name );
 */

public:
    /*!
     * Constructs a Master Icons with "path" path.
     * \sa setPath
     */
    SMasterIcons( const QString & path = 0 );


    /*!
     * Set the path of icons.
     */
    void setPath( const QString & path );


    /*!
     * ReScan the path.
     */
    void reload();


    /*!
     * Lock MasterIcons. When Its locked any other object can't make
     * changes to class and just use icon function.
     * \sa unlcok
     * \sa isLocked
     */
    void lock();


    /*!
     * Unlock MasterIcons. When Its locked any other object can't make
     * changes to class and just use icon function.
     * \sa lock
     * \sa isUnlocked
     */
    void unlock();


    /*!
     * If MasterIcons set locked Returns true .
     * else Returns false.
     * \sa lock
     * \sa isUnLocked
     * \sa accessToChange
     */
    bool isLocked() const;


    /*!
     * If MasterIcons set unlocked Returns true .
     * else Returns false.
     * \sa unlock
     * \sa isLocked
     * \sa accessToChange
     */
    bool isUnLocked() const;


    /*!
     * If MasterIcons is Locked and This is not object that
     * locked the class Returns false. else Returns True.
     * \sa isLocked
     * \sa isUnLocked
     * \sa lock
     */
    bool accessToChange() const;


    /*!
     * Use System Icons theme instead of Silicon Icons.
     * \sa isLocked
     * \sa isUnLocked
     * \sa lock
     */
    void setFromSystemTheme( bool stt );


    /*!
     * Returns Icon with "name" name and "size" size.
     * It's a static class.
     * \sa setPath
     */
    static QIcon icon( const QSize & size , const QString & name );
};

#endif // SMASTERICONS_H

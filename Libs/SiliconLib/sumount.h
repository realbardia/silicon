/*
 * File:      sumount.h
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

#ifndef SUMOUNT_H
#define SUMOUNT_H

#include <QString>
#include <QObject>

#include "libsilicon_global.h"

class SUmountPrivate;

/*! \class
 *  Unmount iso images with umount command on UNIX Base systems
 *  This just work on UNIX systems
 */
class LIBSILICON_EXPORT SUmount : public QObject
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS :
 *     SUmount( QObject *parent = 0 );
 *     SUmount( const QString & path , QObject *parent = 0 );
 *     ~SUmount();
 *
 *     void reset();
 *
 *     void setMountPoint( const QString & path );
 *     bool umount();
 *
 *     const QString & getOutput() const;
 */

public:
    Q_OBJECT
    Q_DISABLE_COPY( SUmount )

    /*!
     *  parent give a parent object and path QString is mount directory of iso image
     * \sa setMountPoint
     */
    SUmount( QObject *parent = 0 );
    SUmount( const QString & path , QObject *parent = 0 );
    ~SUmount();


    /*!
     *  Just Clear MountPoint and set It Empty
     *  This is like setMountPoint( QString() );
     */
    void reset();


    /*!
     *  Set Mount Point to path QString
     */
    void setMountPoint( const QString & path );


    /*!
     *  Unmount mount point that you set before with setMountPoint or SUmount function
     *  \sa setMountPoint
     */
    bool umount();


    /*!
     *  return a QString message that say file unmounted successfully or not
     */
    const QString & getOutput() const;

private:
    SUmountPrivate *p;
};

#endif // SUMOUNT_H

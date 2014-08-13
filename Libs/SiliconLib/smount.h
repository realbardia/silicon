/*
 * File:      smount.h
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

#ifndef SMOUNT_H
#define SMOUNT_H

#include <QString>
#include <QObject>

#include "libsilicon_global.h"

class SMountPrivate;

/*! \class
 * The SMount class mount iso files with mount command.
 * This Class work only on Unix or Unix Like systems.
 */
class LIBSILICON_EXPORT SMount : public QObject
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SMount( QObject *parent = 0 );
 *     SMount( const QString & Imagepath , QObject *parent = 0 );
 *     SMount( const QString & Imagepath , const QString & Mountpath , QObject *parent = 0 );
 *     ~SMount();
 *
 *     void reset();
 *     void setAddress(const QString & path);
 *     void setMountPoint(const QString & path);
 *
 *     bool mount();
 *
 *     QString getOutput() const;
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SMount )

public:
    /*!
     * Constructs an SMount Object with parent "parent"
     * and Imagepath iso file and Mountpath mount point.
     */
    SMount( QObject *parent = 0 );
    SMount( const QString & Imagepath , QObject *parent = 0 );
    SMount( const QString & Imagepath , const QString & Mountpath , QObject *parent = 0 );
    ~SMount();


    /*!
     * Reset SMount object.
     * set Empty image path , mount point and output.
     */
    void reset();


    /*!
     * Set Iso image address to path.
     */
    void setAddress(const QString & path);


    /*!
     * Set Mount Point address to path.
     */
    void setMountPoint(const QString & path);


    /*!
     * Start mount process. If process done successfuly return true.
     * else return false.
     * \sa getOutput
     */
    bool mount();


    /*!
     * Returns Mounting Process output.
     * \sa mount
     */
    QString getOutput() const;

private:
    SMountPrivate *p;
};

#endif // SMOUNT_H

/*
 * File:      srootprocess.h
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

#ifndef SROOTPROCESS_H
#define SROOTPROCESS_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "libsilicon_global.h"

class SRootProcessPrivate;

/*! \class
 *  The SRootProcess Class run commands as super user in Unix base or Unix likes systems .
 */
class LIBSILICON_EXPORT SRootProcess : public QObject
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     enum Mods {
 *         SU
 *        ,SUDO
 *     };
 *
 *     SRootProcess( QObject *parent = 0 );
 *     ~SRootProcess();
 *
 *     void start( const QString & program, const QStringList & arguments , Mods mode = SRootProcess::SU );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SRootProcess )

public:
    /*!
     * This enum describes the way of connect to super user .
     */
    enum Mods {
        SU
       ,SUDO
    };

    SRootProcess( QObject *parent = 0 );
    ~SRootProcess();

    /*!
     * Start command process as super user with arguments "arguments" .
     */
    void start( const QString & program, const QStringList & arguments , Mods mode = SRootProcess::SU );

private:
    SRootProcessPrivate *p;
};

#endif // SROOTPROCESS_H

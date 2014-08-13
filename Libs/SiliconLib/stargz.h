/*
 * File:      stargz.h
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

#ifndef STARGZ_H
#define STARGZ_H

#include <QObject>
#include <QProcess>
#include <QString>
#include <QStringList>

#include "libsilicon_global.h"

class STarGzPrivate;

/*! \class
 *  Class to work with tar and gzip commands
 */
class LIBSILICON_EXPORT STarGz : public QObject
{
/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS :
 *     STarGz( QObject *parent = 0 );
 *     STarGz( const QString & fileName , QObject *parent = 0 );
 *     ~STarGz();
 *
 *     void setFile( const QString & fileName );
 *
 *     QStringList list();
 *     void extractTo( const QString & dest );
 *
 * SIGNALS:
 *     void extractingFinished( int returnCode );
 */

    Q_OBJECT
    Q_DISABLE_COPY( STarGz )
public:
    /*!
     * set work file to fileName QString
     * and set object of parent to parent
     * \sa setFile
     */
    STarGz( QObject *parent = 0 );
    STarGz( const QString & fileName , QObject *parent = 0 );
    ~STarGz();


    /*!
     * set work file to fileName QString
     */
    void setFile( const QString & fileName );


    /*!
     * return name of all files that avalible in tar.gz file
     * \sa extractTo
     */
    QStringList list();


    /*!
     * extarct tar.gz file to dest , when ever process finished , extractingFinished signal
     * will be emit
     * \sa extractingFinished
     */
    void extractTo( const QString & dest );

signals:
    /*!
     * extractingFinished signal emit when
     * extract process (that start with extractTo function) finish .
     */
    void extractingFinished( int returnCode );

private:
    void setup();
    STarGzPrivate *p;
};

#endif // STARGZ_H

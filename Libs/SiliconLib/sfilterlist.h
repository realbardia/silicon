/*
 * File:      sfilterlist.h
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

#ifndef SFILTERLISTLIST_H
#define SFILTERLISTLIST_H

#include <QWidget>
#include <QString>
#include <QStringList>

#include "libsilicon_global.h"

struct SFilterListItem;
class SFilterListPrivate;

/*! \class
 * The SFilterList class provides a List Widget uses for
 * filter lists.
 */
class LIBSILICON_EXPORT SFilterList : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SFilterList(QWidget *parent = 0);
 *     ~SFilterList();
 *
 *     void setFilterList( SFilterList *f_list );
 *     void setItems( const QStringList & path_list );
 *     void addItems( const QStringList & path_list );
 *     void addItem( const QString & path );
 *
 * SIGNALS:
 *     void rowChenged( const QStringList & list );
 *
 * VIRTUAL PROTECTED:
 *     virtual QStringList filtersNames( const QString & path );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SFilterList )

public:
    /*!
     * Construct a Filter List with "*parent" parent.
     */
    SFilterList(QWidget *parent = 0);
    ~SFilterList();


    /*!
     * Set another SFilterList for connecting output to
     * input ( with setItems() function ) of that.
     * \sa setItems
     */
    void setFilterList( SFilterList *f_list );


    /*!
     * Set Empty filter list and add path_list to this.
     * \sa addItems
     * \sa addItem
     */
    void setItems( const QStringList & path_list );


    /*!
     * Add Number of items or an Item to the filter list.
     * \sa setItems
     */
    void addItems( const QStringList & path_list );
    void addItem( const QString & path );

signals:
    /*!
     * This Signal emitted when current selected row changed.
     * list QStringList is filtered items.
     * \sa addItem
     * \sa filtersNames
     */
    void rowChenged( const QStringList & list );

protected:
    /*!
     * This Virtual Function give an added item and Returns
     * strings or paths matches with any algoritm.
     * It's Filter algoritm of Filter List.
     * \sa rowChanged
     * \sa addItem
     */
    virtual QStringList filtersNames( const QString & path );

private slots:
    void filterChanged( int row );

private:
    SFilterListPrivate *p;
    SFilterListItem *item( const QString & name );
};

#endif // SFILTERLISTLIST_H

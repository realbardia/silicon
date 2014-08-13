/*
 * File:      sprogresslist.h
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

#ifndef SPROGRESSLIST_H
#define SPROGRESSLIST_H

#include <QWidget>
#include <QObject>

#include "sprogresslistitem.h"

class SProgressListPrivate;

/*! \class
 * The SProgressList class provides a progress list widget.
 */
class SProgressList : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SProgressList(QWidget *parent = 0);
 *     ~SProgressList();
 *
 *     void insert( int index , SProgressListItem *item );
 *     void take( int index );
 *     void take( SProgressListItem *item );
 *
 *     int count() const;
 *     int index( SProgressListItem *item ) const;
 *     int publicValue() const;
 *
 * PUBLIC SLOTS:
 *     void add( SProgressListItem *item );
 *     void remove( int index );
 *     void remove( SProgressListItem *item );
 *
 * SIGNALS:
 *     void publicValueChanged( int value );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SProgressList )

public:
    /*!
     * Constructs a Progress List with parent "parent".
     */
    SProgressList(QWidget *parent = 0);
    ~SProgressList();


    /*!
     * Insert a progress item to progress list at index position.
     * \sa add
     * \sa remove
     */
    void insert( int index , SProgressListItem *item );


    /*!
     * Remove an item from progress list at index position
     * or in iten pointer .
     * This Function dont delete Item.
     * \sa remove
     */
    void take( int index );
    void take( SProgressListItem *item );


    /*!
     * Returns the number of items in the progress list.
     */
    int count() const;


    /*!
     * Returns index or position of item in the progress list.
     */
    int index( SProgressListItem *item ) const;

    /*!
     * Resturns Avarage of the all progress items.
     * \sa publicValueChanged
     */
    int publicValue() const;

public slots:
    /*!
     * Add a progress item to progress list.
     * \sa insert
     * \sa remove
     */
    void add( SProgressListItem *item );


    /*!
     * Remove an item from progress list at index position
     * or in iten pointer .
     * This Function delete Item after remove from list.
     * \sa take
     */
    void remove( int index );
    void remove( SProgressListItem *item );

signals:
    /*!
     * Emits Avarage of the all progress items.
     * \sa publicValue
     */
    void publicValueChanged( int value );

private slots:
    void expand( QWidget *item );
    void itemDestroyed( SProgressListItem *item );
    void public_value_changed();

private:
    SProgressListPrivate *p;
};

#endif // SPROGRESSLIST_H

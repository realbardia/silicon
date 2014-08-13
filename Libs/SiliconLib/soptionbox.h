/*
 * File:      soptionbox.h
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

#ifndef SOPTIONBOX_H
#define SOPTIONBOX_H

#include <QWidget>
#include <QObject>
#include <QListWidgetItem>
#include <QSize>
#include <QString>
#include <QIcon>

#include "libsilicon_global.h"

class SOptionBoxPrivate;

/*! \class
 * The SProgressList class provides a Option Box widget.
 * Option Box Widget help you to make properties , prefrence or option dialog
 * for your application .
 */
class LIBSILICON_EXPORT SOptionBox : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SOptionBox(QWidget *parent = 0);
 *     ~SOptionBox();
 *
 *     void add(const QString & name);
 *     void add(const QIcon & icon,const QString & name);
 *
 *     void insertItem(int index,const QString & name);
 *     void insertItem(int index,const QIcon & icon,const QString & name);
 *
 *     void remove(int index);
 *
 *     void setIconSize(const QSize & size);
 *
 *     int count() const;
 *     QWidget *widget(int index) const;
 *     QWidget *widget(const QString & name) const;
 *
 *     QSize iconSize() const;
 *
 * PUBLIC SLOTS:
 *     void setCurrentIndex(int index);
 *     void setCurrentPage(QListWidgetItem *item);
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SOptionBox )

public:
    /*!
     * Constructs an Option Box with SOptionBoxparent "parent".
     */
    SOptionBox(QWidget *parent = 0);
    ~SOptionBox();


    /*!
     * Add an item and an Empty Widget to option box list with "name" name
     * and "icon" icon.
     * Item name must be unique.
     * \sa widget
     * \sa insert
     * \sa remove
     */
    void add(const QString & name);
    void add(const QIcon & icon,const QString & name);


    /*!
     * Insert an item and an Empty Widget to option box list at index position
     * with "name" name and "icon" icon.
     * Item name must be unique.
     * \sa widget
     * \sa add
     * \sa remove
     */
    void insertItem(int index,const QString & name);
    void insertItem(int index,const QIcon & icon,const QString & name);


    /*!
     * Remove Item and Widget at position index from option box.
     * \sa add
     * \sa insert
     */
    void remove(int index);


    /*!
     * Set Option Box items icons size to size.
     * \sa iconSize
     */
    void setIconSize(const QSize & size);


    /*!
     * Returns the number of items added to Option Box.
     */
    int count() const;


    /*!
     * Returns widget pointer at poisiton index or "name" name.
     * at default it's an empty widget.
     */
    QWidget *widget(int index) const;
    QWidget *widget(const QString & name) const;


    /*!
     * Returns Option Box items icons size.
     * \sa setIconSize
     */
    QSize iconSize() const;

public slots:
    /*!
     * Set Current showen widget (or Option) to index position.
     */
    void setCurrentIndex(int index);

    /*!
     * Set Current showen widget (or Option) to item.
     */
    void setCurrentPage(QListWidgetItem *item);

private:
    SOptionBoxPrivate *p;

    /*!
     * Returns position of the name item. if it's not exist return -1.
     */
    int itemExist(const QString & name) const;
};

#endif // SOPTIONBOX_H

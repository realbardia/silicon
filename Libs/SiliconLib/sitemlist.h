/*
 * File:      sitemlist.h
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

#ifndef SITEMLIST_H
#define SITEMLIST_H

#include <QList>
#include <QIcon>

#include "libsilicon_global.h"

class SItemListPrivate;
class QListWidgetItem;

/*! \class
 * The SItemList class provides an Item-Category List.
 * SItemList Items Based on QListWidgetItem class.
 */
class LIBSILICON_EXPORT SItemList : public QObject
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SItemList( QObject *parent = 0 );
 *     ~SItemList();
 *
 *     void addCategory(const QString & name);
 *     void addCategory(const QIcon & icon,const QString & name);
 *     void addCategory(QListWidgetItem *catItem);
 *
 *     void insertCategory(int index,const QString & name);
 *     void insertCategory(int index,const QIcon & icon,const QString & name);
 *     void insertCategory(int index,QListWidgetItem *catItem);
 *
 *     void removeCategory(QListWidgetItem *item);
 *     void removeCategory(int index);
 *
 *     QListWidgetItem *catWidget(int index) const;
 *     int catRow(QListWidgetItem *item) const;
 *
 *
 *     void addItem(QListWidgetItem *catItem,const QString & name,QString attachmentText = 0);
 *     void addItem(QListWidgetItem *catItem,const QIcon & icon,const QString & name,QString attachmentText = 0);
 *     void addItem(QListWidgetItem *catItem,QListWidgetItem *item,QString attachmentText = 0);
 *     void addItem(int catIndex,const QString & name,QString attachmentText = 0);
 *     void addItem(int catIndex,const QIcon & icon,const QString & name,QString attachmentText = 0);
 *     void addItem(int catIndex,QListWidgetItem *item,QString attachmentText = 0);
 *
 *     void insertItem(QListWidgetItem *catItem,int itemIndex,const QString & name);
 *     void insertItem(QListWidgetItem *catItem,int itemIndex,const QIcon & icon,const QString & name);
 *     void insertItem(QListWidgetItem *catItem,int itemIndex,QListWidgetItem *item);
 *     void insertItem(int catIndex,int itemIndex,const QString & name);
 *     void insertItem(int catIndex,int itemIndex,const QIcon & icon,const QString & name);
 *     void insertItem(int catIndex,int itemIndex,QListWidgetItem *item);
 *
 *     void removeItem(QListWidgetItem *catItem,QListWidgetItem *item);
 *     void removeItem(int catIndex,QListWidgetItem *item);
 *     void removeItem(QListWidgetItem *catItem,int itemIndex);
 *     void removeItem(int catIndex,int itemIndex);
 *
 *     void clear();
 *     void clearCat( QListWidgetItem *catItem );
 *
 *     QListWidgetItem *itemWidget(QListWidgetItem *catItem,int itemIndex) const;
 *     QListWidgetItem *itemWidget(int catIndex,int itemIndex) const;
 *     int itemRow(QListWidgetItem *catItem,QListWidgetItem *item) const;
 *     int itemRow(int catIndex,QListWidgetItem *item) const;
 *
 *     int catCount() const;
 *     int itemCount(QListWidgetItem *cat) const;
 *     int itemCount(int catIndex) const;
 *
 *     bool isEmpty() const;
 *
 *     QList<QListWidgetItem *> search(const QString & text);
 *
 * SIGNALS:
 *     void catAdded(QListWidgetItem *catItem);
 *     void catAdded(int catIndex);
 *     void catRemoved(QListWidgetItem *catItem);
 *     void catRemoved(int catIndex);
 *
 *     void itemAdded(QListWidgetItem *item);
 *     void itemAdded(int catIndex,int itemIndex);
 *     void itemRemoved(QListWidgetItem *item);
 *     void itemRemoved(int catIndex,int itemIndex);
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SItemList )

public:
    /*!
     * Constructs an Item List with parent "parent".
     */
    SItemList( QObject *parent = 0 );
    ~SItemList();


    /*!
     * Add an Category with "name" name and "icon" icon to
     * the item list.
     * you can use ListWidgetItem instead of name and icon.
     * \sa insertCategory
     * \sa removeCategory
     * \sa catWidget
     */
    void addCategory(const QString & name);
    void addCategory(const QIcon & icon,const QString & name);
    void addCategory(QListWidgetItem *catItem);


    /*!
     * Insert an Category with "name" name and "icon" icon to
     * the item list at position index.
     * you can use ListWidgetItem instead of name and icon.
     * \sa addCategory
     * \sa removeCategory
     * \sa catWidget
     */
    void insertCategory(int index,const QString & name);
    void insertCategory(int index,const QIcon & icon,const QString & name);
    void insertCategory(int index,QListWidgetItem *catItem);


    /*!
     * Remove "*item" item or item at position index
     * from Categories of The Item List.
     * \sa addCategory
     * \sa insertCategory
     * \sa catWidget
     */
    void removeCategory(QListWidgetItem *item);
    void removeCategory(int index);


    /*!
     * Returns ListWidgetItem pointer at position index
     * in Categories of The Item List.
     * \sa addCategory
     * \sa catRow
     */
    QListWidgetItem *catWidget(int index) const;

    /*!
     * Returns position of "*item" item in Categories of The Item List.
     * \sa addCategory
     * \sa catRow
     */
    int catRow(QListWidgetItem *item) const;



    /*!
     * Add an Item with "name" name and "icon" icon to
     * catItem or catIndex from the Item List.
     * you can use ListWidgetItem instead of name and icon.
     * \sa insertItem
     * \sa removeItem
     * \sa itemWidget
     */
    void addItem(QListWidgetItem *catItem,const QString & name,QString attachmentText = 0);
    void addItem(QListWidgetItem *catItem,const QIcon & icon,const QString & name,QString attachmentText = 0);
    void addItem(QListWidgetItem *catItem,QListWidgetItem *item,QString attachmentText = 0);
    void addItem(int catIndex,const QString & name,QString attachmentText = 0);
    void addItem(int catIndex,const QIcon & icon,const QString & name,QString attachmentText = 0);
    void addItem(int catIndex,QListWidgetItem *item,QString attachmentText = 0);


    /*!
     * Insert an Item with "name" name and "icon" icon to
     * catItem or catIndex from the Item List at position itemIndex.
     * you can use ListWidgetItem instead of name and icon.
     * \sa addItem
     * \sa removeItem
     * \sa itemWidget
     */
    void insertItem(QListWidgetItem *catItem,int itemIndex,const QString & name);
    void insertItem(QListWidgetItem *catItem,int itemIndex,const QIcon & icon,const QString & name);
    void insertItem(QListWidgetItem *catItem,int itemIndex,QListWidgetItem *item);
    void insertItem(int catIndex,int itemIndex,const QString & name);
    void insertItem(int catIndex,int itemIndex,const QIcon & icon,const QString & name);
    void insertItem(int catIndex,int itemIndex,QListWidgetItem *item);


    /*!
     * Remove "*item" item or item placed on itemIndex position
     * from "*catItem" cat or from category placed on catIndex position.
     * \sa addItem
     * \sa insertItem
     * \sa itemWidget
     */
    void removeItem(QListWidgetItem *catItem,QListWidgetItem *item);
    void removeItem(int catIndex,QListWidgetItem *item);
    void removeItem(QListWidgetItem *catItem,int itemIndex);
    void removeItem(int catIndex,int itemIndex);


    /*!
     * Clear and delete All of Items and categories
     * added to Item List.
     * \sa removeCat
     * \sa removeItem
     * \sa clearCat
     */
    void clear();

    /*!
     * Clear and delete All of items that added to
     * "*catItem" category.
     * \sa removeCat
     * \sa removeItem
     * \sa clear
     */
    void clearCat( QListWidgetItem *catItem );


    /*!
     * Returns ListWidgetItem pointer at poisition itemIndex from
     * "*catItem" category or category placed on catIndex position.
     * \sa itemRow
     */
    QListWidgetItem *itemWidget(QListWidgetItem *catItem,int itemIndex) const;
    QListWidgetItem *itemWidget(int catIndex,int itemIndex) const;


    /*!
     * Returns "*item" item position in catItem category or
     * category placed on catIndex position.
     * \sa itemWidget
     */
    int itemRow(QListWidgetItem *catItem,QListWidgetItem *item) const;
    int itemRow(int catIndex,QListWidgetItem *item) const;


    /*!
     * Returns number of categories added to the Item List.
     * \sa isEmpty
     * \sa itemCount
     */
    int catCount() const;


    /*!
     * Returns number of item added to "*cat" category or
     * category placed on catIndex position in Item List.
     */
    int itemCount(QListWidgetItem *cat) const;
    int itemCount(int catIndex) const;


    /*!
     * Returns true if Item List is Empty from category and items
     * and Returns false if not.
     * \sa catCount
     */
    bool isEmpty() const;


    /*!
     * Search, Find and Returns List of Items that attachment text
     * or Name text of that containt "text".
     */
    QList<QListWidgetItem *> search(const QString & text);

signals:
    /*!
     * This Signal emitted when an category added to ItemList.
     */
    void catAdded(QListWidgetItem *catItem);
    void catAdded(int catIndex);

    /*!
     * This Signal emitted when an category removed from ItemList.
     */
    void catRemoved(QListWidgetItem *catItem);
    void catRemoved(int catIndex);


    /*!
     * This Signal emitted when an item added to
     * one of the categories of Item List.
     * \sa itemRemoved
     */
    void itemAdded(QListWidgetItem *item);
    void itemAdded(int catIndex,int itemIndex);

    /*!
     * This Signal emitted when an item removed from
     * one of the categories of Item List.
     * \sa itemAdded
     */
    void itemRemoved(QListWidgetItem *item);
    void itemRemoved(int catIndex,int itemIndex);

private:
    SItemListPrivate *p;
};

#endif // SITEMLIST_H

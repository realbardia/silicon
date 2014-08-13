/*
 * File:      scatwidget.h
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

#ifndef SCATWIDGET_H
#define SCATWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QIcon>
#include <QSize>

#include "libsilicon_global.h"

class SCatWidgetPrivate;
class QListWidgetItem;

/*! \class
 * The SCatWidget class provides cat widget class. In Fact
 * the CatWidget is User Interface of the
 * SItemList class.
 * This Class Based on QFrame class.
 */
class LIBSILICON_EXPORT SCatWidget : public QFrame
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SCatWidget(QWidget *parent = 0);
 *     ~SCatWidget();
 *
 *     void addCat( const QString & name );
 *     void addCat(const QIcon & icon, const QString & name);
 *     void addCat(QListWidgetItem *item);
 *     void addItem(const QString & cat,const QString & name);
 *     void addItem(const QString & cat,const QString & name, const QString & attachmentText);
 *     void addItem(const QString & cat,const QIcon & icon, const QString & name);
 *     void addItem(const QString & cat,const QIcon & icon, const QString & name, const QString & attachmentText);
 *     void addItem(const QString & cat,QListWidgetItem *item);
 *
 *     void removeCat( const QString & name );
 *     void removeCat( QListWidgetItem *item );
 *     void removeItem( const QString & cat , const QString & name );
 *     void removeItem( const QString & cat , QListWidgetItem *item );
 *     void removeItem( QListWidgetItem *cat , const QString & name );
 *     void removeItem( QListWidgetItem *cat , QListWidgetItem *item );
 *
 *     void addWidgetToRight(QWidget *widget);
 *     void addWidgetToLeft(QWidget *widget);
 *     void addWidgetToBottom(QWidget *widget);
 *
 *     void setCategoryStyleSheet(const QString & styleSheet);
 *     void setItemListStyleSheet(const QString & styleSheet);
 *     void setStyleSheet(const QString & styleSheet);
 *     void setAnimation(bool stt);
 *     void setSearchBarAnimation(bool stt);
 *
 *     void clear();
 *
 *     void setCatIconSize(const QSize & size);
 *     void setListIconSize(const QSize & size);
 *
 *     void setListGridSize(const QSize & size);
 *     void setIconGridSize(const QSize & size);
 *     void setShortGridSize(const QSize & size);
 *
 *     void setCatWidth( int width );
 *     void setView( QString name );
 *
 *     void setCatFrameshadow( Shadow shdw );
 *     void setListFrameshadow( Shadow shdw );
 *
 *     QString attachmentText(const QString & cat,const QString & name) const;
 *
 *     QString categoryStyleSheet() const;
 *     QString itemListStyleSheet() const;
 *
 *     QListWidgetItem *currentItem() const;
 *     QListWidgetItem *currentCat() const;
 *
 *     QSize catIconSize() const;
 *     QSize listItemSize() const;
 *
 * PUBLIC SLOTS:
 *     void find(const QString & text);
 *     void searching(const QString & text);
 *
 *     void closeSearch();
 *     void autoExpandSearch();
 *     void update();
 *
 *     void viewMenuClose();
 *
 *     void setIconView();
 *     void setListView();
 *     void setShortView();
 *
 *     void setCategoryRow( int row );
 *     void setItemsRow( int row );
 *
 *     void addActionTooItems( QAction *action );
 *
 * SIGNALS:
 *     void itemDoubleClicked( QListWidgetItem *item );
 *     void itemSingleClicked( QListWidgetItem *item );
 *     void currentItemChanged( QListWidgetItem *item );
 *     void currentCatChanged( QListWidgetItem *item );
 *     void contextMenuReguest( QListWidgetItem *item );
 *     void contextMenuReguest( QPoint point );
 *
 *     void viewChanged( QString name );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SCatWidget )

public:
    /*!
     * Constructs The Cat Widget with "*parent" parent.
     */
    SCatWidget(QWidget *parent = 0);
    ~SCatWidget();


    /*!
     * Add "name" category with "icon" icon or Add
     * "*item" item to the cat widget.
     * \sa addItem
     * \sa removeCat
     */
    void addCat( const QString & name );
    void addCat(const QIcon & icon, const QString & name);
    void addCat(QListWidgetItem *item);


    /*!
     * Add "name" item or *item" item to "cat" category
     * to the cat widget.
     * \sa addCat
     * \sa removeItem
     */
    void addItem(const QString & cat,const QString & name);
    void addItem(const QString & cat,const QString & name, const QString & attachmentText);
    void addItem(const QString & cat,const QIcon & icon, const QString & name);
    void addItem(const QString & cat,const QIcon & icon, const QString & name, const QString & attachmentText);
    void addItem(const QString & cat,QListWidgetItem *item);


    /*!
     * Removes "name" category or "*item" category from
     * the cat widget.
     * \sa addCat
     * \sa removeItem
     * \sa clear
     */
    void removeCat( const QString & name );
    void removeCat( QListWidgetItem *item );


    /*!
     * Removes "name" item or "*item" item from "cat" category
     * from the cat widget.
     * \sa addItem
     * \sa removeCat
     * \sa clear
     */
    void removeItem( const QString & cat , const QString & name );
    void removeItem( const QString & cat , QListWidgetItem *item );
    void removeItem( QListWidgetItem *cat , const QString & name );
    void removeItem( QListWidgetItem *cat , QListWidgetItem *item );


    /*!
     * Add a Widget to right , left or buttom of the cat widget.
     */
    void addWidgetToRight(QWidget *widget);
    void addWidgetToLeft(QWidget *widget);
    void addWidgetToBottom(QWidget *widget);


    /*!
     * Set Style Sheet of the Category List to
     * styleSheet string.
     * \sa setItemListStyleSheet
     * \sa categoryStyleSheet
     * \sa setStyleSheet
     */
    void setCategoryStyleSheet(const QString & styleSheet);

    /*!
     * Set Style Sheet of the Item List to
     * styleSheet string.
     * \sa setCategoryStyleSheet
     * \sa itemListStyleSheet
     * \sa setStyleSheet
     */
    void setItemListStyleSheet(const QString & styleSheet);

    /*!
     * Set Style Sheet of the Cat Widget.
     * \sa setItemListStyleSheet
     * \sa setCategoryStyleSheet
     */
    void setStyleSheet(const QString & styleSheet);


    /*!
     * Set Animation on/off state to stt.
     * \sa setSearchBarAnimation
     */
    void setAnimation(bool stt);


    /*!
     * Set SearchBar Animation on/off state to stt.
     * \sa setAnimation
     */
    void setSearchBarAnimation(bool stt);


    /*!
     * Clear all of Items and Categories in the cat widget.
     * \sa removeCat
     * \sa removeItem
     */
    void clear();


    /*!
     * Set Category List Icons Size to "size".
     * \sa setListIconSize
     * \sa catIconSize
     */
    void setCatIconSize(const QSize & size);

    /*!
     * Set Item List Icons Size to "size".
     * \sa setCatIconSize
     * \sa listItemSize
     */
    void setListIconSize(const QSize & size);


    /*!
     * Set Grid size of List View of Item List to size.
     * \sa setIconGridSize
     * \sa setShortGridSize
     */
    void setListGridSize(const QSize & size);

    /*!
     * Set Grid size of Icon View of Item List to size.
     * \sa setListGridSize
     * \sa setShortGridSize
     */
    void setIconGridSize(const QSize & size);

    /*!
     * Set Grid size of Short View of Item List to size.
     * \sa setIconGridSize
     * \sa setListGridSize
     */
    void setShortGridSize(const QSize & size);


    /*!
     * Set Category List width to width.
     */
    void setCatWidth( int width );


    /*!
     * Set View mode to name string.
     * name string must be "icon" , "list" or "short".
     * \sa setIconView
     * \sa setShortView
     * \sa setListView
     * \sa viewChanged
     */
    void setView( QString name );


    /*!
     * Set Catgory or Item List Frame Shadow to shdw.
     */
    void setCatFrameshadow( Shadow shdw );
    void setListFrameshadow( Shadow shdw );


    /*!
     * Returns attachment text of the name item from cat category
     * from the Cat Widget.
     * \sa addItem
     */
    QString attachmentText(const QString & cat,const QString & name) const;


    /*!
     * Returns Category List or Item List Style Sheet.
     * \sa setItemListStyleSheet
     * \sa setCategoryStyleSheet
     */
    QString categoryStyleSheet() const;
    QString itemListStyleSheet() const;


    /*!
     * Return current selected category or item from
     * the Cat Widget.
     * \sa currentCatChanged
     * \sa itemSingleClicked
     */
    QListWidgetItem *currentItem() const;
    QListWidgetItem *currentCat() const;


    /*!
     * Return Category or Item Icon Size.
     * \sa setCatIconSize
     * \sa setListIconSize
     */
    QSize catIconSize() const;
    QSize listItemSize() const;


    /*!
     * When an item is selected in item widget , The SCatWidget
     * show "*action" action on the end of the item.
     */
    void addActionTooItems( QAction *action );


    /*!
     * Returns number of category or items added to the SCatWidget.
     * \sa addCat
     * \sa addItem
     */
    int catCount() const;
    int itemsCount() const;

public slots:
    /*!
     * Search and Find "text" in cat widget items and
     * show result items in items list widget.
     * \sa searching
     */
    void find(const QString & text);


    /*!
     * It's Like find function . But When using searchinig function
     * Cat List Widget Hide Automaticaly and some other effects.
     * \sa find
     */
    void searching(const QString & text);


    /*!
     * Hide or Close search box widget.
     * \sa autoExpandSearch
     * \sa viewMenuClose
     */
    void closeSearch();


    /*!
     * If Search Menu is Expanded set to Unexpanded and else
     * set to Expanded.
     * \sa closeSearch
     */
    void autoExpandSearch();


    /*!
     * It's Like refresh or reload function.
     * refreshing or reloading cat widget class.
     */
    void update();


    /*!
     * Hide or Close View Menu Widget.
     * \sa closeSearch
     */
    void viewMenuClose();


    /*!
     * Set List Widget view mode to Icon View.
     * \sa setListView
     * \sa setShortView
     * \sa setView
     */
    void setIconView();

    /*!
     * Set List Widget view mode to list View.
     * \sa setIconView
     * \sa setShortView
     * \sa setView
     */
    void setListView();

    /*!
     * Set List Widget view mode to short View.
     * \sa setListView
     * \sa setIconView
     * \sa setView
     */
    void setShortView();


    /*!
     * Set Cateogry or Items List of the SCatWidget row to "row".
     */
    void setCategoryRow( int row );
    void setItemsRow( int row );

signals:
    /*!
     * This Double emitted when user Double click on
     * on of the cat widget items.
     * \sa itemSingleClicked
     * \sa currentItemChanged
     */
    void itemDoubleClicked( QListWidgetItem *item );

    /*!
     * This Signal emitted when user Double click on
     * on of the cat widget items.
     * \sa itemDoubleClicked
     * \sa currentItemChanged
     */
    void itemSingleClicked( QListWidgetItem *item );

    /*!
     * This Signal emitted when current selected Item
     * in item List changed.
     * \sa itemDoubleClicked
     * \sa itemSingleClicked
     */
    void currentItemChanged( QListWidgetItem *item );


    /*!
     * This Signal emitted when current selected category
     * in cat List changed.
     */
    void currentCatChanged( QListWidgetItem *item );


    /*!
     * This Signals emitted when user right click on
     * an item of the item list of the cat widget.
     */
    void contextMenuReguest( QListWidgetItem *item );
    void contextMenuReguest( QPoint point );


    /*!
     * This Signal emitted when view type of the item list
     * changed.
     * name QString can be "list","icon" or "short".
     * \sa setView
     */
    void viewChanged( QString name );

private slots:
    void addCatWidgetCat(QListWidgetItem *cat);
    void removeCatWidgetCat(QListWidgetItem *cat);
    void showItems(int row);
    void showItems(QListWidgetItem *cat);
    void itemRowChanged( int row );

    void contextMenuSlot( QPoint point );

    void animationFinished();

protected:
    int checkCatExist(const QString & name );
    void keyPressEvent( QKeyEvent *event );

private:
    SCatWidgetPrivate *p;
};

#endif // SCATWIDGET_H

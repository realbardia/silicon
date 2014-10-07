/*
 * File:      silitab.h
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

#ifndef SILITAB_H
#define SILITAB_H

#include <QWidget>
#include <QString>
#include <QSize>
#include <QIcon>

#include "stabitem.h"

#include "libsilicon_global.h"

class SiliTabPrivate;

/*! \class
 * The siliTab class provides an tabbar class.
 * It's without stacked widget.
 */
class LIBSILICON_EXPORT SiliTab : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     siliTab( QWidget *parent = 0 );
 *     ~siliTab();
 *
 *     STabItem *addTab( const QString & name );
 *     STabItem *addTab( const QIcon & icon , const QString & name );
 *
 *     STabItem *insertTab( int index , const QString & name );
 *     STabItem *insertTab( int index , const QIcon & icon , const QString & name );
 *
 *     bool removeTab( int index );
 *
 *     void moveTab( int from , int to );
 *
 *     QIcon tabIcon( int index ) const;
 *     QString tabText( int index ) const;
 *
 *     STabItem *item( int index ) const;
 *     int index( STabItem *button ) const;
 *
 *
 *     QString styleSheet() const;
 *
 *     QSize itemsSize() const;
 *     int itemsWidth() const;
 *     int itemsHeight() const;
 *
 *     int currentIndex() const;
 *     int count() const;
 *
 *     void setTabText( int index , const QString & text );
 *     void setTabIcon( int index , const QIcon & icon );
 *     void setStyleSheet( const QString & styleSheet );
 *
 *     void setItemsSize( const QSize & size );
 *     void setItemsWidth( int width );
 *     void setItemHeight( int height );
 *
 *     void setAnimation( bool stt );
 *     bool isAnimated() const;
 *
 *     void setCurrentIndex( int index );
 *     void unselectAll( bool stt );
 *
 *     void setPreviewer( bool stt );
 *     bool previewer() const;
 *
 *     void setPreviewerDelay( int ms );
 *     int previewerDelay() const;
 *
 *     void pointTabToWidget( int index , QWidget *widget );
 *     QWidget *pointedWidgetToTab( int index ) const;
 *
 * SIGNALS:
 *     void indexChange( int index );
 *     void itemChange( STabItem *item );
 *     void tabAdded( int index );
 *     void emptied();
 *     void closeRequest( int index );
 *
 *     void moved( int from , int to );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SiliTab )

public:
    /*!
     * Constructs a tabbar with "*parent" parent.
     */
    SiliTab( QWidget *parent = 0 );
    ~SiliTab();


    /*!
     * Add a tab to tabbar with "name" name and if added successfuly
     * Returns true. else Returns false.
     * Normaly return false when animation of the tabbar dont finished.
     * \sa insertTab
     * \sa removeTab
     * \sa count
     */
    STabItem *addTab( const QString & name );
    STabItem *addTab( const QIcon & icon , const QString & name );


    /*!
     * Insert a tab to tabbar with "name" name at position index and
     * if added successfuly Returns true. else Returns false.
     * Normaly return false when animation of the tabbar dont finished.
     * \sa addTab
     * \sa removeTab
     * \sa count
     */
    STabItem *insertTab( int index , const QString & name );
    STabItem *insertTab( int index , const QIcon & icon , const QString & name );


    /*!
     * Remove tab position at index and if removed successfuly
     * Returns true. else Returns false.
     * Normaly return false when animation of the tabbar dont finished.
     * \sa addTab
     * \sa insertTab
     * \sa count
     */
    bool removeTab( int index );


    /*!
     * Move Tab from position "from" to position "to".
     */
    void moveTab( int from , int to );


    /*!
     * Returns icon of the tab placed on pisition index.
     * \sa setTabIcon
     * \sa count
     */
    QIcon tabIcon( int index ) const;

    /*!
     * Returns text of the tab placed on pisition index in tabbar.
     * \sa setTabText
     * \sa count
     */
    QString tabText( int index ) const;


    /*!
     * Returns Tab Item Pointer placed on position index in tabbar.
     * \sa index
     * \sa count
     */
    STabItem *item( int index ) const;

    /*!
     * Returns position of the *button in the tabbar.
     * \sa item
     */
    int index( STabItem *button ) const;


    /*!
     * Returns Style sheet of the tabbar.
     * \sa setStyleSheet
     */
    QString styleSheet() const;


    /*!
     * Returns default tabItems size of the tabbar.
     * \sa setItemsSize
     */
    QSize itemsSize() const;

    /*!
     * Returns default tabItems width of the tabbar.
     * \sa setItemsWidth
     */
    int itemsWidth() const;

    /*!
     * Returns default tabItems hegith of the tabbar.
     * \sa setItemsHeight
     */
    int itemsHeight() const;


    /*!
     * Returns position of the current selected tab.
     * \sa setCurrentIndex
     */
    int currentIndex() const;


    /*!
     * Returns number of the tabs added to tabbar.
     * \sa addTab
     * \sa insertTab
     */
    int count() const;


    /*!
     * Set tab text position at index to "text" string.
     * \sa tabText
     */
    void setTabText( int index , const QString & text );


    /*!
     * Set Tab Icon position at index to "icon" icon.
     * \sa tabIcon
     */
    void setTabIcon( int index , const QIcon & icon );


    /*!
     * Set Style Sheet of the tabbar to styleSheet.
     * \sa styleSheet
     */
    void setStyleSheet( const QString & styleSheet );


    /*!
     * Set Default Items size to "size".
     * \sa itemsSize
     */
    void setItemsSize( const QSize & size );


    /*!
     * Set Default Items width to "width".
     * \sa itemsWidth
     */
    void setItemsWidth( int width );


    /*!
     * Set Default Items height to "height".
     * \sa itemsHeight
     */
    void setItemHeight( int height );


    /*!
     * Set Animation on/off state to stt.
     */
    void setAnimation( bool stt );
    bool isAnimated() const;


    /*!
     * Set current selected tab to tab position at index.
     * \sa currentIndex
     */
    void setCurrentIndex( int index );


    /*!
     * Unselect current tab.
     */
    void unselectAll( bool stt );


    /*!
     * Showing a Preview widget when mouse on tab items.
     */
    void setPreviewer( bool stt );
    bool previewer() const;

    /*!
     * Set Preview widget delay.
     */
    void setPreviewerDelay( int ms );
    int previewerDelay() const;

    /*!
     * Pointing a widget to a tab for previewing.
     */
    void pointTabToWidget( int index , QWidget *widget );
    QWidget *pointedWidgetToTab( int index ) const;

signals:
    /*!
     * This Signals emitted when current selected tab changed.
     */
    void indexChange( int index );
    void itemChange( STabItem *item );


    /*!
     * This Signal emitted when a tab added or inserted
     * to the tabbar.
     * \sa addTab
     * \sa insertTab
     */
    void tabAdded( int index );


    /*!
     * This Signals emitted when the tab bar setted emtpy.
     */
    void emptied();


    /*!
     * This Signals emitted when user click on the close button
     * on a tabItem.
     * \sa removeTab
     * \sa STabItem::closeButton
     */
    void closeRequest( int index );


    /*!
     * This Signal emitted when a tab item moved from 'from'
     * position to 'to' position.
     */
    void moved( int from , int to );


protected:
    void timerEvent( QTimerEvent *event );


private slots:
    void anim_finished( QWidget *widget );
    void clicked_on_item( STabItem *item );
    void on_item( STabItem *item );
    void close_on_item( STabItem *item );
    void close_request( STabItem *item );

private:
    SiliTabPrivate *p;
};

#endif // SILITAB_H

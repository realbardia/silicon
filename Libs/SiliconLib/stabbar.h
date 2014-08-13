/*
 * File:      stabbar.h
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

#ifndef STABBAR_H
#define STABBAR_H

#include <QWidget>
#include <QIcon>
#include <QString>
#include <QSize>

#include "stabitem.h"
#include "libsilicon_global.h"

class STabBarPrivate;

/*! \class
 *  The STabBar class provides a tab bar, e.g. for use in tabbed dialogs
 */
class LIBSILICON_EXPORT STabBar : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS :
 *     STabBar(QWidget *parent = 0);
 *     ~STabBar();
 *
 *     bool addTab(const QString & name);
 *     bool addTab(const QIcon & icon,const QString & name);
 *     bool insertTab(int index,const QString & name);
 *     bool insertTab(int index,const QIcon & icon,const QString & name);
 *     bool removeTab(int index);
 *
 *     void moveTab(int from,int to);
 *
 *     QIcon tabIcon(int index) const;
 *     QString tabText(int index) const;
 *
 *     QString styleSheet() const;
 *
 *     QSize tabsSize() const;
 *     int tabsWidth() const;
 *     int tabsHeight() const;
 *
 *     int currentIndex() const;
 *     int count() const;
 *
 *     QWidget *takeTabBar() const;
 *
 *     QWidget *page(int index) const;
 *     STabItem *tab(int index) const;
 *
 *     void setTabText(int index,const QString & text);
 *     void setTabIcon(int index,const QIcon & icon);
 *     void setStyleSheet(const QString & styleSheet);
 *
 *     void setStackedPalette( const QPalette & palette );
 *     const QPalette & stackedPalette() const;
 *
 *     void setTransparentBackground( bool stt );
 *
 *     void setItemsSize(const QSize & size);
 *     void setItemsWidth(int width);
 *     void setItemHeight(int height);
 *
 *     void setCurrentIndex(int index);
 *
 *     void setAnimation(bool stt);
 *
 *     void addWidgetBeforeTab(QWidget *widget);
 *     void addWidgetAfterTab(QWidget *widget);
 *
 *     void unselectAll( bool stt );
 *
 * PUBLIC SLOTS:
 *     void newTab();
 *     void closeTab();
 *
 * SIGNALS:
 *     void tabChanged(int index);
 *     void tabAdded(int index);
 *     void emptied();
 *     void closeRequest(int index);
 *     void closeRequest(QWidget *widget);
 */

    Q_OBJECT
    Q_DISABLE_COPY( STabBar )

public:
    /*!
     *  Creates a new tab bar with the given parent.
     */
    STabBar( QWidget *parent = 0 );
    ~STabBar();


    /*!
     *  Adds a new tab with text "text" and icon . Returns true if successfully added .
     *  \sa insertTab
     *  \sa removeTab
     */
    bool addTab(const QString & name);
    bool addTab(const QIcon & icon,const QString & name);


    /*!
     *  Insert a new tab with text "text" and icon in index.
     *  Returns true if successfully added .
     *  \sa addTab
     *  \sa removeTab
     */
    bool insertTab(int index,const QString & name);
    bool insertTab(int index,const QIcon & icon,const QString & name);


    /*!
     *  Removes the tab at position index.
     *  \sa addTab
     *  \sa insertTab
     */
    bool removeTab(int index);


    /*!
     * Moves the item at index position int(from) to index position int(to).
     */
    void moveTab(int from,int to);


    /*!
     * Returns the icon of the tab at position index,
     * or a null icon if index is out of range.
     * \sa setTabIcon
     */
    QIcon tabIcon(int index) const;

    /*!
     * Returns the text of the tab at position index,
     * or an empty string if index is out of range.
     * \sa setTabText
     */
    QString tabText(int index) const;


    /*!
     * Returns the styleSheet of tabbar .
     * \sa setStyleSheet
     */
    QString styleSheet() const;


    /*!
     * Sets and Returns palette of the stacked widget in the
     * stabbar.
     */
    void setStackedPalette( const QPalette & palette );
    const QPalette & stackedPalette() const;


    /*!
     * Returns size of the tab items on the tabbar .
     */
    QSize tabsSize() const;
    int tabsWidth() const;
    int tabsHeight() const;

    /*!
     * This property holds the index of the tab bar's visible tab.
     * The current index is -1 if there is no current tab.
     */
    int currentIndex() const;

    /*!
     * Returns the number of tabs in the tab bar.
     */
    int count() const;


    /*!
     * Take tabbar Widget from top of the STabBar and
     * retruns pointer of the silitab to put to other place.
     */
    QWidget *takeTabBar() const;


    /*!
     * Returns widget pointer of the tab at position index .
     *
     * \example : add a pushbutton to tabbar
     *
     *      STabBar tab;
     *      tab.addTab( "new tab" );        // tab added at position 0
     *
     *      QWidget *page_zero = tab.page(0);
     *      QPushButton button( page_zero );
     */
    QWidget *page(int index) const;


    /*!
     * Returns TabItem pointer of the tab at position index .
     */
    STabItem *tab(int index) const;

    /*!
     * Sets the text of the tab at position index to text.
     * \sa tabText
     */
    void setTabText(int index,const QString & text);

    /*!
     * Sets the icon of the tab at position index to icon.
     * \sa tabIcon
     */
    void setTabIcon(int index,const QIcon & icon);

    /*!
     * Sets the styleSheet of the tabbar to stylesheet.
     * \sa styleSheet
     */
    void setStyleSheet(const QString & styleSheet);


    /*!
     * If stt is true set background of the tabbar to transparent
     * else set to untransparent .
     */
    void setTransparentBackground( bool stt );


    /*!
     * Set size of the tab items on the tabbar to size .
     */
    void setItemsSize( const QSize & size );
    void setItemsWidth( int width );
    void setItemHeight( int height );

    /*!
     * set Animation Enabled status too stt .
     */
    void setAnimation( bool stt );

    /*!
     * Add Widget to left or right of the ItemBar of the TabBar .
     */
    void addWidgetBeforeTab( QWidget *widget );
    void addWidgetAfterTab( QWidget *widget );

    /*!
     * Un Select Current selected tab.
     */
    void unselectAll( bool stt );

public slots:
    /*!
     * Add a new tab to tabbar.
     */
    void newTab();

    /*!
     * Close current tab of tabbar.
     */
    void closeTab();

    /*!
     * set Current position of tabbar to index.
     */
    void setCurrentIndex( int index );

signals:
    /*!
     * This signal is emitted when the tab bar's current tab changes.
     * The new current has the given index, or -1 if there isn't a new one
     */
    void tabChanged(int index);

    /*!
     * This signal is emitted when the new tab is added.
     */
    void tabAdded(int index);

    /*!
     * This signal is emitted when the tabbar is empty.
     */
    void emptied();

    /*!
     * This signal is emitted when the close button on a tab is clicked.
     * The index is the index that should be removed.
     */
    void closeRequest(int index);

    /*!
     * This signal is emitted when the close button on a tab is clicked.
     * The widget is the widget that should be removed.
     */
    void closeRequest(QWidget *widget);

private slots:
    void tabChangedSlot( int index );
    void sendCloseTabRequest(int index);

private:
    STabBarPrivate *p;
};

#endif // STABBAR_H

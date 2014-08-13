/*
 * File:      sprogresslistitem.h
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

#ifndef SPROGRESSLISTITEM_H
#define SPROGRESSLISTITEM_H

#include <QWidget>
#include <QObject>
#include <QPushButton>
#include <QString>

#include "libsilicon_global.h"

class SPage;
class SApplication;
class SProgressListItemPrivate;

/*! \class
 * The SProgressListItem class provides an item for use with the SProgressList class.
 */
class LIBSILICON_EXPORT SProgressListItem : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SProgressListItem( QWidget *parent = 0 );
 *     SProgressListItem( const QString & name , QWidget *parent = 0 );
 *     ~SProgressListItem();
 *
 *     void setTitle( const QString & name );
 *     void setInfoText( const QString & str );
 *     void setIcon( const QIcon & icon );
 *
 *     void showExtra( bool stt );
 *     void hideExtra( bool stt );
 *
 *     int value() const;
 *
 *     QPushButton *cancelButton() const;
 *     QWidget *extraWidget() const;
 *
 * PUBLIC SLOTS:
 *     void setValue( int value );
 *
 * SIGNALS:
 *     void valueChanged( int value );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SProgressListItem )

public:
    /*!
     * Constructs a Silicon Progress List Item with parent and
     * "name" title.
     */
    SProgressListItem( SPage *page );
    SProgressListItem( SApplication *app );
    ~SProgressListItem();


    /*!
     * Set Title of item to name.
     * \sa setInfoText
     * \sa setIcon
     */
    void setTitle( const QString & name );

    /*!
     * Set Information text of item that show on below the title to str.
     * \sa setTitle
     * \sa setIcon
     */
    void setInfoText( const QString & str );

    /*!
     * Set Icon of the item to icon.
     * \sa setTitle
     * \sa setInfoText
     */
    void setIcon( const QIcon & icon );


    /*!
     * Set Item expanded or to showen extra widget.
     * This function called when user click on More Button.
     */
    void showExtra( bool stt );

    /*!
     * Set Item unexpanded or hidden extra widget.
     * This function called when user click on Less Button.
     */
    void hideExtra( bool stt );


    /*!
     * Returns value of the progress bar on item.
     * \sa setValue
     * \sa valueChanged
     */
    int value() const;

    /*!
     * Returns pointer of Cancel Button on item.
     */
    QPushButton *cancelButton() const;

    /*!
     * Returns pointer of Extra Widget on item.
     * Programer can customize this widget.
     */
    QWidget *extraWidget() const;

public slots:
    /*!
     * Set Value of the progress bar on the item.
     * \sa value
     * \sa valueChanged
     */
    void setValue(int value);


    void autoExpand();

signals:
    /*!
     * This Signal emitted when value of the progress bar changed.
     * \sa setValue
     * \sa value
     */
    void valueChanged( int value );

    void destroyed( SProgressListItem *item );

private slots:
    void animationFinished();

private:
    void setup();
    void registerItem();

private:
    SProgressListItemPrivate *p;
};

#endif // SPROGRESSLISTITEM_H

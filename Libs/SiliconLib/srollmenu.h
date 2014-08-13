/*
 * File:      srollmenu.h
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

#ifndef SROLLMENU_H
#define SROLLMENU_H

#include <QWidget>
#include <QObject>
#include <QFrame>
#include <QString>
#include <QSize>

#include "libsilicon_global.h"

class SRollMenuPrivate;

/*! \class
 *  The SRollMenu Class is a type of menu that made by Silicon.
 */
class LIBSILICON_EXPORT SRollMenu : public QFrame
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     SRollMenu(QWidget *parent = 0);
 *     ~SRollMenu();
 *
 *     void setText( const QString & name );
 *     void setWidget( QWidget *widget );
 *
 *     void setAnimation(bool stt);
 *     void setTargetSize( QSize size );
 *     void setStartSize( QSize size );
 *     void setButtonSize( QSize size );
 *     void setStyleSheet( const QString & style );
 *
 *     QString text() const;
 *     QWidget *widget() const;
 *
 *     bool animationStt() const;
 *     QSize targetSize() const;
 *     QSize startSize() const;
 *     QSize buttonSize() const;
 *     QString styleSheet() const;
 *
 *     bool isExpanded() const;
 *
 * PUBLIC SLOTS:
 *     void autoExpand();
 *     void expand(bool stt);
 *
 * SIGNALS:
 *     void expanded(bool stt);
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SRollMenu )

public:
    /*!
     * Constructs a Roll Menu with parent "parent".
     */
    SRollMenu( QWidget *parent = 0 );
    ~SRollMenu();


    /*!
     * Set the text that showen on the roll menu button to name.
     * \sa text
     */
    void setText( const QString & name );


    /*!
     * Set the widget of roll menu to widget.
     * This widget showen when user click on menu button and
     * hidden when click on button again.
     * \sa widget
     */
    void setWidget( QWidget *widget );


    /*!
     * set Animation Enabled status too stt.
     * \sa setStartSize
     * \sa setTargetSize
     * \sa animationStt
     */
    void setAnimation( bool stt );


    /*!
     * Set the expanded size of menu.
     * \sa setStartSize
     * \sa targetSize
     */
    void setTargetSize( QSize size );


    /*!
     * Set the size of menu when its going to expanding.
     * this option effective when animation status set enable.
     * \sa setTargetSize
     * \sa startSize
     * \sa setAnimation
     */
    void setStartSize( QSize size );

    /*!
     * Set the unexpanded size of menu or size of button.
     * \sa buttonSize
     */
    void setButtonSize( QSize size );


    /*!
     * Set style sheet of the scroll widget to styleSheet.
     */
    void setStyleSheet( const QString & style );

    /*!
     * Returns text that showen on roll menu button.
     * \sa setText
     */
    QString text() const;


    /*!
     * Returns pointer of widget that set to showen on roll menu.
     * \sa setWidget
     */
    QWidget *widget() const;


    /*!
     * Returns status of the animation as boolian type.
     * if animations is Enabled return true and else return false.
     * \sa setAnimation
     */
    bool animationStt() const;


    /*!
     * Returns target size or size of expanded roll menu.
     * \sa setTargetSize
     */
    QSize targetSize() const;

    /*!
     * Returns the size of menu when its going to expanding.
     * this option effective when animation status set enable.
     * \sa setStartSize
     */
    QSize startSize() const;

    /*!
     * Returns the unexpanded size of menu or size of button.
     * \sa setButtonSize
     */
    QSize buttonSize() const;


    /*!
     * Returns style sheet of the scroll widget as a QString.
     * \sa setStyleSheet
     */
    QString styleSheet() const;


    /*!
     * Returns true if the roll menu expanded ,else return false.
     * \sa autoExpand
     * \sa expand
     */
    bool isExpanded() const;

public slots:
    /*!
     * If roll menu expanded set unexpanded and else set expanded.
     * \sa expand
     * \sa isExpanded
     */
    void autoExpand();

    /*!
     * Set expand state of the roll menu to stt.
     * \sa autoExpand
     * \sa isExpanded
     */
    void expand(bool stt);

signals:
    /*!
     * This Signal emitted when menu expand state changed.
     * If menu expanded emitted with true stt and else emitted with false stt.
     * \sa expand
     * \sa autoExpand
     */
    void expanded(bool stt);

private slots:
    void animationFinished();

private:
    SRollMenuPrivate *p;
};

#endif // SROLLMENU_H

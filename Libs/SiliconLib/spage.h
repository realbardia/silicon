/*
 * File:      spage.h
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

#ifndef SPAGE_H
#define SPAGE_H

#include <QWidget>
#include <QSize>
#include <QStringList>

#include "smenupanel.h"
#include "sprogresslistitem.h"
#include "libsilicon_global.h"

class QMessageBox;
class QStatusBar;
class QToolBar;
class SPagePrivate;
class SApplication;
class SDialog;

/*! \class
 * The SApplication class provides a Silicon Page
 * class based on The QWidget.
 */
class LIBSILICON_EXPORT SPage : public QWidget
{

/*! List of All Available Functions :
 *
 * PUBLIC FUNCTIONS:
 *     enum ParentType{
 *         Page,
 *         Application
 *     };
 *
 *     enum PageType{
 *         TabedPage,
 *         WindowedPage
 *     };
 *
 *     SPage( const QString & name , SApplication *parent , PageType type = SPage::TabedPage );
 *     SPage( const QString & name , SPage *parent , PageType type = SPage::TabedPage );
 *     ~SPage();
 *
 *     void setPageType( PageType type );
 *     PageType pageType() const;
 *
 *     ParentType parentType() const;
 *
 *     SApplication *parentApp() const;
 *     SPage *pageParent() const;
 *
 *     SMenuPanel *menuPanel();
 *     void setMenuPanel( SMenuPanel *menuPanel );
 *
 *     void setToolBar( QToolBar *toolbar );
 *     QToolBar *toolBar() const;
 *
 *     void setStatusBar( QStatusBar *statusbar );
 *     QStatusBar *statusBar() const;
 *
 *     void showMessage( QMessageBox *message , const QSize & size );
 *
 *     void addChild( SPage *page );
 *     const QList<SPage *> & childs() const;

 *     void addDialog( SDialog *dialog );
 *     const QList<SDialog *> & dialogs() const;
 *
 *     int numberOfAllChilds() const;
 *     int numberOfAllSubObjects() const;
 *
 *     void setTitle( const QString & title );
 *     const QString & title() const;
 *
 *     void setIcon( const QIcon & icon );
 *     const QIcon & icon() const;
 *
 * PUBLIC SLOTS:
 *     void setBusy( bool stt );
 *     void hideMessage();
 *     void setVisible( bool visible );
 *
 * SIGNALS:
 *     void panelUpdated();
 *     void panelUpdated( SMenuPanel *panel );
 *     void destroyed( SPage *sapp );
 *
 * PROTECTED FUNCTIONS:
 *     void resizeEvent( QResizeEvent *event );
 *     virtual void typeChanged( PageType type );
 *
 */

    Q_OBJECT
    Q_DISABLE_COPY( SPage )

public:
    /*!
     * Type of the Parent.
     * \sa parentType
     */
    enum ParentType{
        Page,               /*! SPage parent */
        Application         /*! SApplication Parent */
    };

    /*!
     * Type of the Page.
     * \sa setPageType
     */
    enum PageType{
        TabedPage,          /*! Silicon Tab Page */
        WindowedPage        /*! Silicon Windowed Page */
    };


    /*!
     * Constructs a SPage Class whit "name" title and "*parent" parent.
     */
    SPage( const QString & name , SApplication *parent , PageType type = SPage::TabedPage );
    SPage( const QString & name , SPage *parent , PageType type = SPage::TabedPage );
    ~SPage();


    /*!
     * Set Type of the Page to type.
     */
    void setPageType( PageType type );
    PageType pageType() const;

    /*!
     * Returns type of the page parent.
     */
    ParentType parentType() const;

    /*!
     * Returns SApplication or SPage parent if exists.
     * Else returns zero.
     */
    SApplication *parentApp() const;
    SPage *pageParent() const;

    /*!
     * Returns Menu Panel object of sapplication.
     * \sa setMenuPanel
     */
    SMenuPanel *menuPanel();

    /*!
     * Set Menu Panel of the sapplcation to "*menuPanel".
     * \sa menuPanel
     */
    void setMenuPanel( SMenuPanel *menuPanel );


    /*!
     * Hold ToolBar pointer that showen above the page.
     */
    void setToolBar( QToolBar *toolbar );
    QToolBar *toolBar() const;


    /*!
     * Hold Status Bar widget pointer.
     */
    void setStatusBar( QStatusBar *statusbar );
    QStatusBar *statusBar() const;

    /*!
     * Under Construction ....
     */
    void showMessage( QMessageBox *message , const QSize & size );


    /*!
     * Set a page to child of the current SPage.
     * \sa childs
     */
    void addChild( SPage *page );

    /*!
     * Returns list of the page childs.
     * \sa addChild
     */
    const QList<SPage *> & childs() const;


    /*!
     * Set a Dialog to child of the current SPage.
     * \sa dialogs
     */
    void addDialog( SDialog *dialog );

    /*!
     * Returns list of the page dialogs.
     * \sa addDialog
     */
    const QList<SDialog *> & dialogs() const;

    /*!
     * Returns Number of all of the childs in the childs tree.
     */
    int numberOfAllChilds() const;
    int numberOfAllSubObjects() const;


    /*!
     * Holds title of the current page that showing on the tabbar
     * or Window Decoration.
     */
    void setTitle( const QString & title );
    const QString & title() const;


    /*!
     * Holds icon of the current page that showing on the tabbar
     * or Window Decoration.
     */
    void setIcon( const QIcon & icon );
    const QIcon & icon() const;

public slots:
    /*!
     * Set SApplication to Busy and show an InnerProgressbar
     * on SApplication widget.
     */
    void setBusy( bool stt );

    /*!
     * Under Construction ....
     */
    void hideMessage();

    /*!
     * Set Current Page Visible or Unvisible.
     * to switch silicon to current page use setVisible(true) or
     * show() functions.
     */
    void setVisible( bool visible );

    void switchPageType();

signals:
    /*!
     * This Signal emitted when add some items or any
     * other changes to the Menu the Menu Panel.
     */
    void panelUpdated();
    void panelUpdated( SMenuPanel *panel );


    /*!
     * This Signal emitted when SApplication going to
     * destroy .
     */
    void destroyed( SPage *sapp );

    void resized( const QSize & size );

protected:
    /*!
     * This Virtual Protected Function Call when SApplication
     * resized.
     */
    void resizeEvent( QResizeEvent *event );

    /*!
     * Called When current page type changed.
     * \sa setPageType
     */
    virtual void typeChanged( PageType type );

protected:
    bool event( QEvent *event );

private slots:
    void pageDestroyed( SPage *page );
    void dialogDestroyed( SDialog *dialog );
    void pageSettingsChanged();

private:
    void registerToSilicon();
    void init();
    void pageTypeChanged();
    void registerPanel();

private:
    SPagePrivate *p;
};

#endif // SPAGE_H

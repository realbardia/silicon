/*
 * File:      scatwidget.cpp
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

#include "scatwidget.h"

#include <QColor>
#include <QList>
#include <QPalette>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QPoint>
#include <QSpacerItem>
#include <QLineEdit>
#include <QPushButton>
#include <QPaintEvent>
#include <QToolBar>
#include <QAction>

#include "sanimation.h"
#include "sitemlist.h"
#include "sstylesheet.h"
#include "srollmenu.h"
#include "smastericons.h"





/*================================*
 |                                |
 |       Special QListWidget      |
 |                                |
 *================================*/

class SpecListWidget : public QListWidget
{
public:
    SpecListWidget( QWidget *parent = 0 );

    void addActionToPanel( QAction *action );

protected:
    void paintEvent( QPaintEvent *event );

private:
    QToolBar *toolbar;
    QListWidgetItem *last_current;
};




SpecListWidget::SpecListWidget( QWidget *parent )
    : QListWidget( parent )
{
    last_current = 0;
    toolbar = new QToolBar( this );
        toolbar->setToolButtonStyle( Qt::ToolButtonIconOnly );
        toolbar->setIconSize( QSize(22,22) );
        toolbar->setStyleSheet( "QToolBar{ border-style:solid; }"  "QToolButton{ border-style:solid; }" );
}

void SpecListWidget::addActionToPanel( QAction *action )
{
    toolbar->addAction( action );
}

void SpecListWidget::paintEvent( QPaintEvent *event )
{
    QListWidget::paintEvent( event );

    if( this->currentItem() <= 0 )
    {
        toolbar->hide();
        return ;
    }

    QRect  rect  = this->visualItemRect(this->currentItem());
    QPoint point = rect.topRight();

    toolbar->move( point.x() - toolbar->width() , point.y() + rect.height()/2 - toolbar->height()/2 );

    toolbar->show();
}





/*================================*
 |                                |
 |         Extra Functions        |
 |                                |
 *================================*/

QList<QListWidgetItem *> removeDuplicates( QList<QListWidgetItem *> list )
{
    for( int i=0;i<list.count()-1;i++ )
        for( int j=i+1;j<list.count();j++ )
            if( list.at( i )->text() == list.at( j )->text() )
            {
                list.takeAt( j );
                j--;
            }

    return list;
}




/*================================*
 |                                |
 |        SCatWidgetPrivate       |
 |                                |
 *================================*/

class SCatWidgetPrivate
{
public:
    bool animStt;
    int cat_width;

    SItemList *itemList;
    QListWidget *catWidget;
    SpecListWidget *itemWidget;
    QFrame *baseFrame;
    QFrame *panelFrame;

    QFrame *viewFrame;
    QHBoxLayout *viewLayout;
    QPushButton *iconViewButton;
    QPushButton *listViewButton;
    QPushButton *shortViewButton;

    QSize iconGrid;
    QSize listGrid;
    QSize shortGrid;

    QHBoxLayout *mainLayout;
    QVBoxLayout *baseLayout;
    QVBoxLayout *itemLayout;
    QHBoxLayout *panelLayout;
    QString style;

    SAnimation *animation;
    SRollMenu *viewMenu;
    SRollMenu *searchMenu;
    QLineEdit *searchLine;
};




/*================================*
 |                                |
 |           SCatWidget           |
 |                                |
 *================================*/

SCatWidget::SCatWidget( QWidget *parent ) : QFrame( parent )
{
    p = new SCatWidgetPrivate;

    p->animStt = true;
    p->cat_width = 175;

    p->animation = new SAnimation();
        QObject::connect( p->animation , SIGNAL( finished() ) , this , SLOT( animationFinished() ) );

    p->itemWidget = new SpecListWidget();
        p->itemWidget->setContextMenuPolicy( Qt::CustomContextMenu );
        p->itemWidget->setSortingEnabled( true );
        p->itemWidget->setFrameShadow( QFrame::Plain );
        //p->itemWidget->setFocusPolicy( Qt::NoFocus );
        QObject::connect( p->itemWidget , SIGNAL( itemDoubleClicked( QListWidgetItem* ) ) , this , SIGNAL(itemDoubleClicked(QListWidgetItem*)) );
        QObject::connect( p->itemWidget , SIGNAL( itemClicked( QListWidgetItem* ) )       , this , SIGNAL(itemSingleClicked(QListWidgetItem*)) );
        QObject::connect( p->itemWidget , SIGNAL( currentRowChanged(int) )                , this , SLOT(itemRowChanged(int))                   );
        QObject::connect( p->itemWidget , SIGNAL( customContextMenuRequested( QPoint ) )  , this , SLOT( contextMenuSlot( QPoint ) )           );

    p->baseFrame = new QFrame();
        p->baseFrame->setAutoFillBackground( false );

    p->catWidget = new QListWidget();
        p->catWidget->setFixedWidth( p->cat_width );
        p->catWidget->setFrameShadow( QFrame::Plain );
        p->catWidget->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        p->catWidget->setFocusPolicy( Qt::NoFocus );
        QObject::connect( p->catWidget , SIGNAL( currentRowChanged( int ) ) , this , SLOT( showItems( int ) ) );

    p->searchLine = new QLineEdit();
        p->searchLine->setSizePolicy( QSizePolicy::Minimum  , QSizePolicy::Minimum );
        QObject::connect( p->searchLine , SIGNAL( textChanged( QString ) ) , this , SLOT( searching( QString ) ) );

    p->searchMenu = new SRollMenu();
        p->searchMenu->setText( "search" );
        p->searchMenu->setWidget( p->searchLine );
        p->searchMenu->setTargetSize( QSize( 137 , 25 ) );
        p->searchMenu->setStartSize( QSize( 0 , 25 ) );
        p->searchMenu->setAnimation( true );
        p->searchMenu->setButtonSize( QSize( 90 , 25 ) );
        QObject::connect( p->searchMenu , SIGNAL( expanded( bool ) ) , this , SLOT( closeSearch() ) );

    p->iconViewButton = new QPushButton();
        p->iconViewButton->setCheckable( true );
        p->iconViewButton->setChecked( false );
        p->iconViewButton->setIcon( SMasterIcons::icon( QSize(48,48) , "view-list-icons.png" ) );
        p->iconViewButton->setToolTip( tr("Icon View") );
        QObject::connect( p->iconViewButton , SIGNAL( clicked() ) , this , SLOT( setIconView() ) );

    p->listViewButton = new QPushButton();
        p->listViewButton->setCheckable( true );
        p->listViewButton->setChecked( false );
        p->listViewButton->setIcon( SMasterIcons::icon( QSize(48,48) , "view-list-details.png" ) );
        p->listViewButton->setToolTip( tr("List View") );
        QObject::connect( p->listViewButton , SIGNAL( clicked() ) , this , SLOT( setListView() ) );

    p->shortViewButton = new QPushButton();
        p->shortViewButton->setCheckable( true );
        p->shortViewButton->setChecked( false );
        p->shortViewButton->setIcon( SMasterIcons::icon( QSize(48,48) , "view-list-tree.png" ) );
        p->shortViewButton->setToolTip( tr("Short View") );
        QObject::connect( p->shortViewButton , SIGNAL( clicked() ) , this , SLOT( setShortView() ) );

    p->viewLayout = new QHBoxLayout();
        p->viewLayout->setSpacing( 1 );
        p->viewLayout->setContentsMargins( 0 , 0 , 0 , 0 );
        p->viewLayout->addWidget( p->iconViewButton );
        p->viewLayout->addWidget( p->listViewButton );
        p->viewLayout->addWidget( p->shortViewButton );

    p->viewFrame = new QFrame();
        p->viewFrame->setLayout( p->viewLayout );

    p->viewMenu = new SRollMenu();
        p->viewMenu->setText( "View" );
        p->viewMenu->setAnimation( true );
        p->viewMenu->setWidget( p->viewFrame );
        p->viewMenu->setTargetSize( QSize( 93 , 25 ) );
        p->viewMenu->setStartSize( QSize( 0 , 25 ) );
        p->viewMenu->setButtonSize( QSize( 90 , 25 ) );
        QObject::connect( p->viewMenu , SIGNAL( expanded( bool ) ) , this , SLOT( viewMenuClose() ) );

    p->panelFrame = new QFrame();

    p->panelLayout = new QHBoxLayout( p->panelFrame );
        p->panelLayout->addSpacerItem( new QSpacerItem( 4 ,  2 ,  QSizePolicy::Expanding ,  QSizePolicy::Minimum ) );
        p->panelLayout->addWidget( p->viewMenu );
        p->panelLayout->addWidget( p->searchMenu );
        p->panelLayout->setContentsMargins( 6 , 6 , 6 , 6 );
        p->panelLayout->setSpacing( 0 );

    p->itemLayout = new QVBoxLayout();
        p->itemLayout->addWidget( p->itemWidget );
        p->itemLayout->addWidget( p->panelFrame );
        p->itemLayout->setContentsMargins( 0 , 0 , 0 , 0 );
        p->itemLayout->setSpacing( 0 );

    p->mainLayout = new QHBoxLayout( p->baseFrame );
        p->mainLayout->addWidget( p->catWidget );
        p->mainLayout->addLayout( p->itemLayout );
        p->mainLayout->setContentsMargins( 0 , 0 , 0 , 0 );
        p->mainLayout->setSpacing( 0 );

    p->baseLayout = new QVBoxLayout( this );
        p->baseLayout->addWidget( p->baseFrame );
        p->baseLayout->setContentsMargins( 0 , 0 , 0 , 0 );

    p->itemList = new SItemList( this );
        QObject::connect( p->itemList , SIGNAL( catAdded( QListWidgetItem* ) ) , this , SLOT( addCatWidgetCat( QListWidgetItem* ) ) );
        QObject::connect( p->itemList , SIGNAL( catRemoved( QListWidgetItem* ) ) , this , SLOT( removeCatWidgetCat( QListWidgetItem* ) ) );

    this->setListView();
}

void SCatWidget::addCat( const QString & name )
{
    this->addCat( QIcon() , name );
}

void SCatWidget::addCat( const QIcon & icon ,  const QString & name )
{
    this->addCat( new QListWidgetItem( icon , name ) );
}

void SCatWidget::addCat( QListWidgetItem *item )
{
    if( this->checkCatExist( item->text() )!=-1 )
        return ;

    p->itemList->addCategory( item );

    if( p->catWidget->count() == 1 )
    {
        p->catWidget->setCurrentRow( 0 );
        this->showItems( 0 );
    }
}

void SCatWidget::addItem( const QString & cat , const QString & name )
{
    this->addItem( cat , QIcon() , name );
}

void SCatWidget::addItem( const QString & cat , const QString & name ,  const QString & attachmentText )
{
    this->addItem( cat , QIcon() , name , attachmentText );
}

void SCatWidget::addItem( const QString & cat , const QIcon & icon ,  const QString & name )
{
    this->addItem( cat , icon , name , QString() );
}

void SCatWidget::addItem( const QString & cat , const QIcon & icon ,  const QString & name ,  const QString & attachmentText )
{
    QListWidgetItem *item = new QListWidgetItem( icon , name );
        item->setStatusTip( attachmentText );

    this->addItem( cat , item );
}

void SCatWidget::addItem( const QString & cat , QListWidgetItem *item )
{
    int index=this->checkCatExist( cat );
    if( index==-1 )
        return ;

    p->itemList->addItem( index , item , item->statusTip() );

    this->update();
}

void SCatWidget::removeCat( const QString & name )
{
    for( int i=0 ; i<p->itemList->catCount() ; i++ )
        if( p->itemList->catWidget( i )->text() == name )
            p->itemList->removeCategory( p->itemList->catWidget( i ) );
}

void SCatWidget::removeCat( QListWidgetItem *item )
{
    p->itemList->removeCategory( item );
}

void SCatWidget::removeItem( const QString & cat , const QString & name )
{
    for( int i=0 ; i<p->itemList->catCount() ; i++ )
        if( p->itemList->catWidget( i )->text() == cat )
            this->removeItem( p->itemList->catWidget( i ) , name );

    this->showItems( p->catWidget->currentRow() );
}

void SCatWidget::removeItem( const QString & cat , QListWidgetItem *item )
{
    for( int i=0 ; i<p->itemList->catCount() ; i++ )
        if( p->itemList->catWidget( i )->text() == cat )
            this->removeItem( p->itemList->catWidget( i ) , item );

    this->showItems( p->catWidget->currentRow() );

}

void SCatWidget::removeItem( QListWidgetItem *cat , const QString & name )
{
    for( int i=0 ; i<p->itemList->itemCount( cat ) ; i++ )
        if( p->itemList->itemWidget( cat , i )->text() == name )
            p->itemList->removeItem( cat , i );

    this->showItems( p->catWidget->currentRow() );
}

void SCatWidget::removeItem( QListWidgetItem *cat , QListWidgetItem *item )
{
    p->itemList->removeItem( cat , item );

    this->showItems( p->catWidget->currentRow() );
}

void SCatWidget::addWidgetToRight( QWidget *widget )
{
    p->mainLayout->addWidget( widget );
}

void SCatWidget::addWidgetToLeft( QWidget *widget )
{
    p->mainLayout->insertWidget( 0 , widget );
}

void SCatWidget::addWidgetToBottom( QWidget *widget )
{
    p->panelLayout->insertWidget( 0 , widget );
}

void SCatWidget::setCategoryStyleSheet( const QString & styleSheet )
{
    p->catWidget->setStyleSheet( styleSheet );
}

void SCatWidget::setItemListStyleSheet( const QString & styleSheet )
{
    p->itemWidget->setStyleSheet( styleSheet );
}

void SCatWidget::setStyleSheet( const QString & styleSheet )
{
    p->style=styleSheet;
    QString backgroundStr="Background" ,
            categoryStr="Category" ,
            itemsStr="List" ,
            viewBarStr="ViewBar" ,
            searchBarStr="SearchBar";

    SStyleSheet css;
        css.addStyle( "SCatWidget" , styleSheet );

    SStyleSheet tmpCss;
        tmpCss.addStyle( css.filter( backgroundStr ) );
        tmpCss.replace( backgroundStr  , "QFrame" );

    p->baseFrame->setStyleSheet( tmpCss.style() );



    tmpCss.clear();
        tmpCss.addStyle( css.filter( categoryStr ) );
        tmpCss.replace( categoryStr  , "QListWidget" );

    p->catWidget->setStyleSheet( tmpCss.style() );



    tmpCss.clear();
        tmpCss.addStyle( css.filter( itemsStr ) );
        tmpCss.replace( itemsStr  , "QListWidget" );

    p->itemWidget->setStyleSheet( tmpCss.style() );



    tmpCss.clear();
        tmpCss.addStyle( css.filter( searchBarStr ) );
        tmpCss.replace( searchBarStr  , "SRollMenu" );

    p->searchMenu->setStyleSheet( tmpCss.style() );



    tmpCss.clear();
        tmpCss.addStyle( css.filter( viewBarStr ) );
        tmpCss.replace( viewBarStr  , "SRollMenu" );

    p->viewMenu->setStyleSheet( tmpCss.style() );

    p->cat_width = p->catWidget->width();
}

void SCatWidget::setAnimation( bool stt )
{
    p->animStt = stt;
}

void SCatWidget::setSearchBarAnimation( bool stt )
{
    p->searchMenu->setAnimation( stt );
}

void SCatWidget::clear()
{
    p->itemList->clear();
    p->catWidget->clear();
    p->itemWidget->clear();
}

void SCatWidget::setCatIconSize( const QSize & size )
{
    p->catWidget->setIconSize( size );
}

void SCatWidget::setListIconSize( const QSize & size )
{
    p->itemWidget->setIconSize( size );
}

void SCatWidget::setListGridSize( const QSize & size )
{
    if( p->listViewButton->isChecked() )
        p->itemWidget->setGridSize( size );

    p->listGrid = size;
}

void SCatWidget::setIconGridSize( const QSize & size )
{
    if( p->iconViewButton->isChecked() )
        p->itemWidget->setGridSize( size );

    p->iconGrid = size;
}

void SCatWidget::setShortGridSize( const QSize & size )
{
    if( p->shortViewButton->isChecked() )
        p->itemWidget->setGridSize( size );

    p->shortGrid = size;
}

void SCatWidget::setCatWidth( int width )
{
    p->cat_width = width;
    p->catWidget->setFixedWidth( p->cat_width );
}

void SCatWidget::setView( QString name )
{
    QString str = name.toLower();


    if( str == "icon" )
        this->setIconView();

    else if( str == "list" )
        this->setListView();

    else if( str == "short" )
        this->setShortView();


    return ;
}

void SCatWidget::setCatFrameshadow( Shadow shdw )
{
    p->catWidget->setFrameShadow( shdw );
}

void SCatWidget::setListFrameshadow( Shadow shdw )
{
    p->itemWidget->setFrameShadow( shdw );
}

void SCatWidget::setIconView()
{
    p->itemWidget->setAlternatingRowColors( false );
    p->itemWidget->setProperty( "isWrapping" ,  QVariant( true ) );
    p->itemWidget->setSpacing( 13 );
    p->itemWidget->setMovement( QListView::Static );
    p->itemWidget->setViewMode( QListView::IconMode );
    p->itemWidget->setGridSize( p->iconGrid );

    p->iconViewButton->setChecked( true );
    p->listViewButton->setChecked( false );
    p->shortViewButton->setChecked( false );

    emit this->viewChanged( "icon" );
}

void SCatWidget::setListView()
{
    p->itemWidget->setAlternatingRowColors( false );
    p->itemWidget->setProperty( "isWrapping" ,  QVariant( false ) );
    p->itemWidget->setSpacing( 0 );
    p->itemWidget->setMovement( QListView::Static );
    p->itemWidget->setViewMode( QListView::ListMode );
    p->itemWidget->setGridSize( p->listGrid );

    p->iconViewButton->setChecked( false );
    p->listViewButton->setChecked( true );
    p->shortViewButton->setChecked( false );

    emit this->viewChanged( "list" );
}

void SCatWidget::setShortView()
{
    p->itemWidget->setAlternatingRowColors( false );
    p->itemWidget->setProperty( "isWrapping" ,  QVariant( true ) );
    p->itemWidget->setSpacing( 3 );
    p->itemWidget->setMovement( QListView::Static );
    p->itemWidget->setViewMode( QListView::ListMode );
    p->itemWidget->setGridSize( p->shortGrid );

    p->iconViewButton->setChecked( false );
    p->listViewButton->setChecked( false );
    p->shortViewButton->setChecked( true );

    emit this->viewChanged( "short" );
}

void SCatWidget::setCategoryRow( int row )
{
    p->catWidget->setCurrentRow( row );
}

void SCatWidget::setItemsRow( int row )
{
    p->itemWidget->setCurrentRow( row );
}

QString SCatWidget::attachmentText(const QString & cat,const QString & name) const
{

}

QString SCatWidget::categoryStyleSheet() const
{
    return p->catWidget->styleSheet();
}

QString SCatWidget::itemListStyleSheet() const
{
    return p->itemWidget->styleSheet();
}

QListWidgetItem *SCatWidget::currentItem() const
{
    return p->itemWidget->currentItem();
}
QListWidgetItem *SCatWidget::currentCat() const
{
    return p->catWidget->currentItem();
}

QSize SCatWidget::catIconSize() const
{
    return p->catWidget->iconSize();
}

QSize SCatWidget::listItemSize() const
{
    return p->itemWidget->iconSize();
}

void SCatWidget::addActionTooItems( QAction *action )
{
    p->itemWidget->addActionToPanel( action );
}

int SCatWidget::catCount() const
{
    return p->catWidget->count();
}

int SCatWidget::itemsCount() const
{
    return p->itemWidget->count();
}

void SCatWidget::find( const QString & text )
{
    QList<QListWidgetItem *> result = p->itemList->search( text );

    int count = p->itemWidget->count();
    for( int i=0;i<count;i++ )
        p->itemWidget->takeItem( 0 );

    if( text.isEmpty() )
        return ;

    result = removeDuplicates( result );

    for( int i=0;i<result.count();i++ )
        p->itemWidget->addItem( result.at( i ) );
}

void SCatWidget::searching( const QString & text )
{
    if( p->itemList->isEmpty() )
        return ;

    if( !text.isEmpty() && p->catWidget->width()!=0 )
    {
        QSize target = QSize( 0 , p->catWidget->height() );

        if( p->animStt )
            p->animation->start( p->catWidget , target , SAnimation::RESIZE , 7 , SAnimation::MIN_MAX );
        else
            p->catWidget->setFixedWidth( target.width() );
    }
    else if( text.isEmpty() )
    {
        QSize target = QSize( p->cat_width , p->catWidget->height() );

        if( p->animStt )
            p->animation->start( p->catWidget , QSize( p->cat_width , p->catWidget->height() ) , SAnimation::RESIZE , 7 , SAnimation::MIN_MAX );
        else
            p->catWidget->setFixedWidth( target.width() );

        this->update();
        return ;
    }

    this->find( text );
}

void SCatWidget::closeSearch()
{
    if( !p->searchMenu->isExpanded() )
    {
        if( p->viewMenu->isExpanded() )
            p->viewMenu->expand( false );
        return ;
    }

    p->searchLine->clear();
    this->searching( QString() );
}

void SCatWidget::autoExpandSearch()
{
    if( p->searchMenu->isExpanded() )
        p->searchMenu->expand( false );
    else
        p->searchMenu->expand( true );

    p->searchLine->clear();
    this->searching( QString() );
}

void SCatWidget::update()
{
    this->showItems( p->itemList->catWidget( p->catWidget->currentRow() ) );
}

void SCatWidget::viewMenuClose()
{
    if( !p->viewMenu->isExpanded() && p->searchMenu->isExpanded() )
        p->searchMenu->expand( false );
}

void SCatWidget::addCatWidgetCat( QListWidgetItem *cat )
{
    p->catWidget->addItem( cat );
}

void SCatWidget::removeCatWidgetCat( QListWidgetItem *cat )
{
    p->catWidget->removeItemWidget( cat );
}

void SCatWidget::showItems( int row )
{
    if( p->catWidget->count() == 0 || row == -1 )
        return ;

    this->showItems( p->catWidget->item( row ) );
}

void SCatWidget::contextMenuSlot( QPoint point )
{
    emit this->contextMenuReguest( point );
    emit this->contextMenuReguest( p->itemWidget->currentItem() );
}

void SCatWidget::animationFinished()
{
    p->catWidget->setMinimumHeight( 0 );
    p->catWidget->setMaximumHeight( 16777215 );
}

void SCatWidget::showItems( QListWidgetItem *cat )
{
    emit this->currentCatChanged( cat );

    int index=p->itemList->catRow( cat );
    if( index==-1 )
        return ;

    for( int i=p->itemWidget->count()-1;i>=0;i-- )
        p->itemWidget->takeItem( i );

    for( int i=0;i<p->itemList->itemCount( index );i++ )
        p->itemWidget->addItem( p->itemList->itemWidget( index , i ) );
}

void SCatWidget::itemRowChanged( int row )
{
    emit this->currentItemChanged( p->itemWidget->item(row) );
}

int SCatWidget::checkCatExist( const QString & name )
{
    for( int i=0;i<p->itemList->catCount();i++ )
        if( p->itemList->catWidget( i )->text().toLower() == name.toLower() )
            return i;

    return -1;
}

void SCatWidget::keyPressEvent( QKeyEvent *event )
{
    if( event->text().isEmpty() )
        return ;

    if( !p->searchMenu->isExpanded() )
        p->searchMenu->expand( true );

    p->searchLine->setText( p->searchLine->text() + event->text() );
}

SCatWidget::~SCatWidget()
{
    delete p;
}


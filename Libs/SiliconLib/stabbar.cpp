/*
 * File:      stabbar.cpp
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

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

#include "stabbar.h"

#include "svariableconverter.h"
#include "sstylesheet.h"
#include "silistacked.h"
#include "silitab.h"
#include "sscrollwidget.h"



/*================================*
 |                                |
 |         STabBarPrivate         |
 |                                |
 *================================*/

class STabBarPrivate
{
public:
    QHBoxLayout *tabLayout;
    QVBoxLayout *widgetLayout;
    QFrame *topFrame;

    QString style;

    SScrollWidget *scrollWidget;
    SiliTab *tab;
    siliStacked *page;

    bool background_transparent_stt;
};



/*================================*
 |                                |
 |            STabBar             |
 |                                |
 *================================*/

STabBar::STabBar( QWidget *parent ) : QWidget( parent )
{
    p = new STabBarPrivate;

    p->tab = new SiliTab();
    p->page = new siliStacked();

    p->background_transparent_stt = false;

    p->scrollWidget=new SScrollWidget();
        p->scrollWidget->setEnableVerticalArrow(false);
        p->scrollWidget->setMaximumHeight(32);
        p->scrollWidget->setWidget( p->tab );

    p->tabLayout=new QHBoxLayout();
        p->tabLayout->addWidget( p->scrollWidget );
        p->tabLayout->setContentsMargins(0,0,0,0);
        p->tabLayout->setSpacing(0);

    p->topFrame=new QFrame();
        p->topFrame->setLayout( p->tabLayout );
        p->topFrame->setSizePolicy( QSizePolicy::Minimum ,QSizePolicy::Maximum );

    p->widgetLayout=new QVBoxLayout(this);
        p->widgetLayout->addWidget( p->topFrame  );
        p->widgetLayout->addWidget( p->page );
        p->widgetLayout->setContentsMargins( 0 , 0 , 0 , 0 );
        p->widgetLayout->setSpacing(0);

    QObject::connect( p->tab, SIGNAL(indexChange(int)) , this , SLOT(tabChangedSlot(int))      );
    QObject::connect( p->tab, SIGNAL(indexChange(int)) , this , SIGNAL(tabChanged(int))        );
    QObject::connect( p->tab, SIGNAL(tabAdded(int))    , this , SIGNAL(tabAdded(int))          );
    QObject::connect( p->tab, SIGNAL(emptied())        , this , SIGNAL(emptied())              );
    QObject::connect( p->tab, SIGNAL(closeRequest(int)), this , SLOT(sendCloseTabRequest(int)) );
}


bool STabBar::addTab(const QString & name)
{
    return this->addTab(QIcon(),name);
}

bool STabBar::addTab(const QIcon & icon,const QString & name)
{
    if( p->tab->addTab(icon,name) )
    {
        p->page->addPage();
            p->page->setCurrentIndex( p->tab->currentIndex() );

        p->tab->pointTabToWidget( p->tab->currentIndex() , p->page->currentPage() );
        return true;
    }
    else
        return false;
}

bool STabBar::insertTab(int index,const QString & name)
{
    return this->insertTab( index , QIcon() , name );
}

bool STabBar::insertTab(int index,const QIcon & icon,const QString & name)
{
    if( p->tab->insertTab(index,icon,name) )
    {
        p->page->insertPage(index);
            p->page->setCurrentIndex( p->tab->currentIndex() );

        p->tab->pointTabToWidget( p->tab->currentIndex() , p->page->currentPage() );
        return true;
    }
    else
        return false;
}

bool STabBar::removeTab(int index)
{
    if( p->tab->removeTab(index) )
    {
        p->page->removePage(index);
        return true;
    }
    else
        return false;
}



QIcon STabBar::tabIcon(int index) const
{
    return p->tab->tabIcon(index);
}

QString STabBar::tabText(int index) const
{
    return p->tab->tabText(index);
}

QString STabBar::styleSheet() const
{
    return p->style;
}

void STabBar::setStackedPalette( const QPalette & palette )
{
    p->page->setPalette( palette );
}

const QPalette & STabBar::stackedPalette() const
{
    return p->page->palette();
}

QSize STabBar::tabsSize() const
{
    return p->tab->itemsSize();
}

int STabBar::tabsWidth() const
{
    return p->tab->itemsWidth();
}

int STabBar::tabsHeight() const
{
    return p->tab->itemsHeight();
}

int STabBar::currentIndex() const
{
    return p->tab->currentIndex();
}

QWidget *STabBar::page(int index) const
{
    return p->page->page(index);
}

STabItem *STabBar::tab(int index) const
{
    return p->tab->item( index );
}

int STabBar::count() const
{
    return p->tab->count();
}




QWidget *STabBar::takeTabBar() const
{
    p->widgetLayout->removeWidget( p->topFrame );
    return p->topFrame;
}

void STabBar::setTabText(int index,const QString & text)
{
    p->tab->setTabText(index,text);
}

void STabBar::setTabIcon(int index,const QIcon & icon)
{
    p->tab->setTabIcon(index,icon);
}

void STabBar::setStyleSheet(const QString & styleSheet)
{
    p->style = styleSheet;
    SStyleSheet stl;
        stl.addStyle("STabBar",styleSheet);
        stl.enter("STabBar");

    p->tab->setStyleSheet( stl.filter("item") );
    p->scrollWidget->setStyleSheet( stl.filter("TabBar") );

    SStyleSheet tmpCss;
        tmpCss.addStyle( stl.filter("Background") );
        tmpCss.replace( "Background" , "QFrame" );

    if( !p->background_transparent_stt )
        p->topFrame->setStyleSheet( tmpCss.style() );

}

void STabBar::setTransparentBackground( bool stt )
{
    p->background_transparent_stt = stt;
}

void STabBar::setItemsSize(const QSize & size)
{
    p->tab->setItemsSize(size);
}

void STabBar::setItemsWidth(int width)
{
    p->tab->setItemsWidth(width);
}

void STabBar::setItemHeight(int height)
{
    p->tab->setItemHeight(height);
}

void STabBar::setCurrentIndex(int index)
{
    p->tab->setCurrentIndex(index);
    p->page->setCurrentIndex(index);
}

void STabBar::setAnimation(bool stt)
{
    p->tab->setAnimation(stt);
}



void STabBar::addWidgetBeforeTab(QWidget *widget)
{
    p->tabLayout->insertWidget(0,widget);
}

void STabBar::addWidgetAfterTab(QWidget *widget)
{
    p->tabLayout->addWidget(widget);
}

void STabBar::unselectAll( bool stt )
{
    p->tab->unselectAll( stt );
}



void STabBar::newTab()
{
    static int count=0;
    if(this->addTab(QString()+"NewTab "+ SVariableConverter::intToString(count) ))
        count++;
}

void STabBar::closeTab()
{
    this->removeTab( p->tab->currentIndex());
}

void STabBar::tabChangedSlot( int index )
{
    if( index == -1 )
        return ;

    p->page->setCurrentIndex( index );
}

void STabBar::sendCloseTabRequest(int index)
{
    emit this->closeRequest( index );
    emit this->closeRequest( p->page->page(index) );
}

STabBar::~STabBar()
{
    delete p;
}

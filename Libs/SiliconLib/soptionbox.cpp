/*
 * File:      soptionbox.cpp
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

#include "soptionbox.h"

#include <QFont>
#include <QSpacerItem>
#include <QListWidget>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>

#include "spicturewidget.h"




/*================================*
 |                                |
 |        SOptionBoxPrivate       |
 |                                |
 *================================*/

class SOptionBoxPrivate
{
public:
    QListWidget *list;
    QScrollArea *scroll_area;
    QStackedWidget *stack;
    QLabel *nameLabel;
    SPictureWidget *icon;

    QHBoxLayout *layout;
    QVBoxLayout *stackLayout;
    QHBoxLayout *titleLayout;
};




/*================================*
 |                                |
 |           SOptionBox           |
 |                                |
 *================================*/

SOptionBox::SOptionBox(QWidget *parent) : QWidget( parent )
{
    p = new SOptionBoxPrivate;

    QFont font = this->font();
        font.setBold(true);
        font.setItalic(true);

    p->nameLabel=new QLabel();
        p->nameLabel->setFont( font );

    p->icon = new SPictureWidget();
        p->icon->setFixedSize( 22 ,22 );

    p->titleLayout=new QHBoxLayout();
        p->titleLayout->addWidget( p->nameLabel );
        p->titleLayout->setContentsMargins( 7,7,3,13);
        p->titleLayout->addSpacerItem( new QSpacerItem(4, 2, QSizePolicy::Expanding, QSizePolicy::Minimum) );
        p->titleLayout->addWidget( p->icon );

    p->list=new QListWidget();
        p->list->setMaximumWidth(150);
        p->list->setMinimumWidth(150);
        p->list->setCurrentRow(0);

    p->stack=new QStackedWidget();
        p->stack->removeWidget( p->stack->widget(0) );

    p->scroll_area = new QScrollArea();
        p->scroll_area->setWidgetResizable( true );
        p->scroll_area->setWidget( p->stack );
        p->scroll_area->setFrameShadow( QFrame::Plain );
        p->scroll_area->setFrameShape( QFrame::NoFrame );
        p->scroll_area->viewport()->setAutoFillBackground( false );
        p->scroll_area->viewport()->setSizePolicy( QSizePolicy::Preferred , QSizePolicy::Minimum );

    p->stackLayout=new QVBoxLayout();
        p->stackLayout->addLayout( p->titleLayout );
        p->stackLayout->addWidget( p->scroll_area );

    p->layout=new QHBoxLayout(this);
        p->layout->addWidget( p->list );
        p->layout->addLayout( p->stackLayout);
        p->layout->setContentsMargins(0,0,0,0);
        p->layout->setSpacing(0);

    QObject::connect( p->list,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*))
                     ,this,SLOT(setCurrentPage(QListWidgetItem*)));
}

void SOptionBox::add(const QString & name)
{
    this->add(QIcon(),name);
}

void SOptionBox::add(const QIcon & icon,const QString & name)
{
    int row = this->itemExist( name );
    if(row != -1)
        return ;

    p->list->addItem( new QListWidgetItem(icon,name) );
    p->stack->addWidget( new QWidget() );

    if( p->list->currentRow() == -1)
        this->setCurrentIndex(0);
}

void SOptionBox::insertItem(int index,const QString & name)
{
    this->insertItem(index,QIcon(),name);
}

void SOptionBox::insertItem(int index,const QIcon & icon,const QString & name)
{
    int row = this->itemExist( name );
    if(row != -1)
        return ;

    p->list->insertItem( index , new QListWidgetItem(icon,name) );
    p->stack->insertWidget( index , new QWidget() );

    if( p->list->currentRow() == -1)
        this->setCurrentIndex(0);
}

void SOptionBox::remove( int index )
{
    p->list->removeItemWidget( p->list->item(index) );
    p->stack->removeWidget( p->stack->widget(index) );
}

void SOptionBox::setIconSize(const QSize & size)
{
    p->list->setIconSize(size);
}

int SOptionBox::count() const
{
    return p->list->count();
}

QWidget *SOptionBox::widget(int index) const
{
    return p->stack->widget(index);
}

QWidget *SOptionBox::widget(const QString & name) const
{
    int row = this->itemExist( name );
    if(row == -1)
        return 0;

    return this->widget(row);
}

QSize SOptionBox::iconSize() const
{
    return p->list->iconSize();
}

int SOptionBox::itemExist(const QString & name) const
{
    for(int i=0;i< p->list->count();i++)
        if( p->list->item(i)->text() == name )
            return i;

    return -1;
}

void SOptionBox::setCurrentIndex(int index)
{
    p->list->setCurrentRow(index);
}

void SOptionBox::setCurrentPage(QListWidgetItem *item)
{
    p->nameLabel->setText( item->text() );
    p->icon->drawIcon( item->icon() , QSize(22,22) );
    p->stack->setCurrentIndex( p->list->row(item) );
}

SOptionBox::~SOptionBox()
{
    delete p;
}

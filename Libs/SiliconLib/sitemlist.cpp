/*
 * File:      sitemlist.cpp
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

#include "sitemlist.h"

#include <QListWidgetItem>




/*================================*
 |                                |
 |          SList Struct          |
 |                                |
 *================================*/

struct SList
{
    SList(QListWidgetItem *item){ this->item=item; }
    QList<QListWidgetItem *> list;
    QListWidgetItem *item;
    ~SList()
    {
        return ; // BUG , Fix Me :) ===========================
        delete item;
        for(int i=0;i<list.count();i++)
            delete list.at(i);
    }
};




/*================================*
 |                                |
 |        SItemListPrivate        |
 |                                |
 *================================*/

class SItemListPrivate
{
public:
    ~SItemListPrivate()
    {
        for(int i=0;i<cat.count();i++)
            delete cat.at(i);
    }

    QList<SList *> cat;
};




/*================================*
 |                                |
 |           SItemList            |
 |                                |
 *================================*/

SItemList::SItemList( QObject *parent ) : QObject( parent )
{
    p = new SItemListPrivate;
}

void SItemList::addCategory(const QString & name)
{
    this->addCategory(QIcon(),name);
}

void SItemList::addCategory(const QIcon & icon,const QString & name)
{
    this->addCategory(new QListWidgetItem(icon,name));
}

void SItemList::addCategory(QListWidgetItem *catItem)
{
    p->cat<<new SList(catItem);

    emit catAdded(catItem);
    emit catAdded(p->cat.count()-1);
}

void SItemList::insertCategory(int index,const QString & name)
{
    this->insertCategory(index,QIcon(),name);
}

void SItemList::insertCategory(int index,const QIcon & icon,const QString & name)
{
    this->insertCategory(index,new QListWidgetItem(icon,name));
}

void SItemList::insertCategory(int index,QListWidgetItem *catItem)
{
    p->cat.insert(index,new SList(catItem));

    emit catAdded(catItem);
    emit catAdded(index);
}

void SItemList::removeCategory(QListWidgetItem *item)
{
    int index=this->catRow(item);
    if(index==-1)
        return ;

    this->removeCategory(index);
}

void SItemList::removeCategory(int index)
{
    emit catRemoved(p->cat.at(index)->item);
    emit catRemoved(index);

    p->cat.removeAt(index);
}

QListWidgetItem *SItemList::catWidget(int index) const
{
    return p->cat.at(index)->item;
}

int SItemList::catRow(QListWidgetItem *item) const
{
    for(int i=0;i<p->cat.count();i++)
        if(item==p->cat.at(i)->item)
            return i;

    return -1;
}


void SItemList::addItem(QListWidgetItem *catItem,const QString & name,QString attachmentText)
{
    this->addItem(catItem,QIcon(),name,attachmentText);
}

void SItemList::addItem(QListWidgetItem *catItem,const QIcon & icon,const QString & name,QString attachmentText)
{
    this->addItem(catItem,new QListWidgetItem(icon,name),attachmentText);
}

void SItemList::addItem(QListWidgetItem *catItem,QListWidgetItem *item,QString attachmentText)
{
    int index=this->catRow(catItem);
    if(index==-1)
        return ;

    this->addItem(index,item,attachmentText);
}

void SItemList::addItem(int catIndex,const QString & name,QString attachmentText)
{
    this->addItem(catIndex,QIcon(),name,attachmentText);
}

void SItemList::addItem(int catIndex,const QIcon & icon,const QString & name,QString attachmentText)
{
    this->addItem(catIndex,new QListWidgetItem(icon,name),attachmentText);
}

void SItemList::addItem(int catIndex,QListWidgetItem *item,QString attachmentText)
{
    item->setStatusTip( attachmentText );
    p->cat.at(catIndex)->list<<item;

    emit itemAdded(item);
    emit itemAdded(catIndex,p->cat.at(catIndex)->list.count()-1);
}

void SItemList::insertItem(QListWidgetItem *catItem,int itemIndex,const QString & name)
{
    this->insertItem(catItem,itemIndex,QIcon(),name);
}

void SItemList::insertItem(QListWidgetItem *catItem,int itemIndex,const QIcon & icon,const QString & name)
{
    this->insertItem(catItem,itemIndex,new QListWidgetItem(icon,name));
}

void SItemList::insertItem(QListWidgetItem *catItem,int itemIndex,QListWidgetItem *item)
{
    int index=this->catRow(catItem);
    if(index==-1)
        return ;

    this->insertItem(index,itemIndex,item);
}

void SItemList::insertItem(int catIndex,int itemIndex,const QString & name)
{
    this->insertItem(catIndex,itemIndex,QIcon(),name);
}

void SItemList::insertItem(int catIndex,int itemIndex,const QIcon & icon,const QString & name)
{
    this->insertItem(catIndex,itemIndex,new QListWidgetItem(icon,name));
}

void SItemList::insertItem(int catIndex,int itemIndex,QListWidgetItem *item)
{
    p->cat.at(catIndex)->list.insert(itemIndex,item);

    emit itemAdded(item);
    emit itemAdded(catIndex,itemIndex);
}

void SItemList::removeItem(QListWidgetItem *catItem,QListWidgetItem *item)
{
    int index=this->catRow(catItem);
    if(index==-1)
        return ;

    this->removeItem(index,item);
}

void SItemList::removeItem(int catIndex,QListWidgetItem *item)
{
    int index=this->itemRow(catIndex,item);
    if(index==-1)
        return ;

    this->removeItem(catIndex,index);
}

void SItemList::removeItem(QListWidgetItem *catItem,int itemIndex)
{
    int index=this->catRow(catItem);
    if(index==-1)
        return ;

    this->removeItem( index , itemIndex );
}

void SItemList::removeItem(int catIndex,int itemIndex)
{
    emit itemRemoved(p->cat.at(catIndex)->list.at(itemIndex));
    emit itemRemoved(catIndex,itemIndex);

    p->cat.at(catIndex)->list.removeAt(itemIndex);
}

void SItemList::clear()
{
    int count = p->cat.count();
    for( int i=0 ; i<count ; i++ )
        delete p->cat.at(i);

    p->cat.clear();
}

void SItemList::clearCat( QListWidgetItem *catItem )
{
    int index = this->catRow( catItem );
    if( index == -1 )
        return ;

    int count = p->cat.at(index)->list.count();
    for( int i=0 ; i<count ; i++ )
        delete p->cat.at(index)->list.at(i);

    p->cat.at(index)->list.clear();
}

QListWidgetItem *SItemList::itemWidget(QListWidgetItem *catItem,int itemIndex) const
{
    int index=this->catRow(catItem);
    if(index==-1)
        return NULL;

    return this->itemWidget(index,itemIndex);
}

QListWidgetItem *SItemList::itemWidget(int catIndex,int itemIndex) const
{
    return p->cat.at(catIndex)->list.at(itemIndex);
}

int SItemList::itemRow(QListWidgetItem *catItem,QListWidgetItem *item) const
{
    int index=this->catRow(catItem);
    if(index==-1)
        return -1;

    return this->itemRow(index,item);

}

int SItemList::itemRow(int catIndex,QListWidgetItem *item) const
{
    for(int i=0;i<p->cat.at(catIndex)->list.count();i++)
        if(item==p->cat.at(catIndex)->list.at(i))
            return i;

    return -1;
}

int SItemList::catCount() const
{
    return p->cat.count();
}

int SItemList::itemCount(QListWidgetItem *cat) const
{
    int index=this->catRow(cat);
    if(index==-1)
        return -1;

    return this->itemCount(index);
}

bool SItemList::isEmpty() const
{
    return p->cat.isEmpty();
}

QList<QListWidgetItem *> SItemList::search(const QString & text)
{
    QList<QListWidgetItem *> result;

    for( int i=0 ; i<p->cat.count() ; i++ )
        for( int j=0 ; j<p->cat.at(i)->list.count() ; j++ )
            if(    p->cat.at(i)->list.at(j)->text().contains( text , Qt::CaseInsensitive )
                || p->cat.at(i)->list.at(j)->whatsThis().contains( text , Qt::CaseInsensitive ) )
                result << p->cat.at(i)->list.at(j);

    return result;
}

int SItemList::itemCount(int catIndex) const
{
    return p->cat.at(catIndex)->list.count();
}

SItemList::~SItemList()
{
    delete p;
}


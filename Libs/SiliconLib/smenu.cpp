/*
 * File:      smenu.cpp
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

#include "smenu.h"

#include <QSize>
#include <QPalette>
#include <QColor>
#include <QHBoxLayout>
#include <QGridLayout>

#include "sstylesheet.h"
#include "scolor.h"




/*================================*
 |                                |
 |          SSnap Struct          |
 |                                |
 *================================*/

struct SSnap
{
    QWidget *widget;
    int top;
    int left;
    int right;
    int bottom;
    bool stt;

    bool operator==( SSnap snap );
};

bool SSnap::operator ==( SSnap snap )
{
    return widget == snap.widget && top == snap.top && left == snap.left &&
            right == snap.right && bottom == snap.bottom && stt == snap.stt;
}




/*================================*
 |                                |
 |          SMenuPrivate          |
 |                                |
 *================================*/

class SMenuPrivate
{
public:
    QList<QFrame *> frame;
    QWidget *Main;
    QWidget *default_widget;

    QGridLayout *layout;
    QHBoxLayout *widget_layout;
    QSize last_size;

    QString style;

    bool animState;
    bool on_showing_anim;

    SAnimation *animation;
    SSnap snapWidget;
};




/*================================*
 |                                |
 |             SMenu              |
 |                                |
 *================================*/

SMenu::SMenu(QWidget *parent) : QWidget( parent )
{
    p = new SMenuPrivate;

    this->setDefaults();

    p->Main=new QWidget();

    QPalette palette = this->palette();
    SColor color( palette.color(QPalette::Highlight) );
        //color = color/3;
        color.setAlpha( 37 );

    palette.setColor( QPalette::Window , color );

    p->default_widget = new QWidget();
        p->default_widget->setAutoFillBackground( true );
        p->default_widget->setPalette( palette );
        p->default_widget->hide();

    p->widget_layout = new QHBoxLayout();
        p->widget_layout->addWidget( p->default_widget );

    p->layout=new QGridLayout( this );

    this->setupFrames();

    p->animation=new SAnimation(this);

    QObject::connect(p->animation,SIGNAL(finished()),this,SLOT(finishAnim()));
}

void SMenu::setDefaults()
{
    this->setAutoFillBackground( false );
    p->snapWidget.stt=false;
    p->animState=false;
}

void SMenu::setupFrames()
{
    p->layout->setContentsMargins(0,0,0,0);
    p->layout->setSpacing(0);

    if(p->frame.count()==0)
    {
        for(int i=0;i<9;i++)
        {
            if(i==4)
            {
                p->layout->addWidget(p->Main,i/3,i%3);
                continue;
            }

            p->frame<<new QFrame();
                p->frame.last()->setMinimumSize(QSize(3,3));
                p->frame.last()->setFrameShadow(QFrame::Plain);
            if(i/3==1)
                p->frame.last()->setMaximumWidth(3);
            else
                p->frame.last()->setMaximumHeight(3);

            p->layout->addWidget(p->frame.last(),i/3,i%3);
        }
    }

    p->layout->addLayout( p->widget_layout , 1 , 1 );
}

void SMenu::snapTo(QWidget *widget,int top,int left,int right,int bottom)
{
    bool stt = p->snapWidget.widget == widget &&
               p->snapWidget.top    == top    &&
               p->snapWidget.left   == left   &&
               p->snapWidget.right  == right  &&
               p->snapWidget.bottom == bottom &&
               p->last_size == widget->size();


    p->snapWidget.widget = widget;
    p->snapWidget.top    = top;
    p->snapWidget.left   = left;
    p->snapWidget.right  = right;
    p->snapWidget.bottom = bottom;

    if( !stt )
        this->snapCheck();
}

void SMenu::setAnimation(bool stt)
{
    this->p->animState=stt;
}

void SMenu::setMainWidget(QWidget *widget)
{
    delete p->Main;
    p->Main=widget;
    p->widget_layout->addWidget( p->Main );
}

void SMenu::setStyleSheet(const QString & style)
{
    p->style = style;

    SStyleSheet stl;
        stl.addStyle( style );
        stl.setStyle( stl.filter( "SMenu" ) );
        stl.enter( "SMenu" );

    SStyleSheet tmpCss;
        tmpCss.addStyle( stl.filter( "topLeft" ) );
        tmpCss.replace( "topLeft" , "QFrame" );
    p->frame.at(0)->setStyleSheet( tmpCss.filter("QFrame") );

    tmpCss.clear();
        tmpCss.addStyle( stl.filter( "top" ) );
        tmpCss.replace( "top" , "QFrame" );
    p->frame.at(1)->setStyleSheet( tmpCss.filter("QFrame") );

    tmpCss.clear();
        tmpCss.addStyle( stl.filter( "topRight" ) );
        tmpCss.replace( "topRight" , "QFrame" );
    p->frame.at(2)->setStyleSheet( tmpCss.filter("QFrame") );

    tmpCss.clear();
        tmpCss.addStyle( stl.filter( "left" ) );
        tmpCss.replace( "left" , "QFrame" );
    p->frame.at(3)->setStyleSheet( tmpCss.filter("QFrame") );

    tmpCss.clear();
        tmpCss.addStyle( stl.filter( "right" ) );
        tmpCss.replace( "right" , "QFrame" );
    p->frame.at(4)->setStyleSheet( tmpCss.filter("QFrame") );

    tmpCss.clear();
        tmpCss.addStyle( stl.filter( "bottomLeft" ) );
        tmpCss.replace( "bottomLeft" , "QFrame" );
    p->frame.at(5)->setStyleSheet( tmpCss.filter("QFrame") );

    tmpCss.clear();
        tmpCss.addStyle( stl.filter( "bottom" ) );
        tmpCss.replace( "bottom" , "QFrame" );
    p->frame.at(6)->setStyleSheet( tmpCss.filter("QFrame") );

    tmpCss.clear();
        tmpCss.addStyle( stl.filter( "bottomRight" ) );
        tmpCss.replace( "bottomRight" , "QFrame" );
    p->frame.at(7)->setStyleSheet( tmpCss.filter("QFrame") );
}

QWidget *SMenu::widget() const
{
    return p->Main;
}

QString SMenu::styleSheet() const
{
    return p->style;
}

bool SMenu::animationState() const
{
    return this->p->animState;
}

bool SMenu::menuIsHidden() const
{
    if( p->animState )
        return ( p->animation->isActive() && !p->on_showing_anim ) || this->isHidden();
    else
        return this->isHidden();
}

void SMenu::autoShowHide()
{
    if(this->isHidden())
        this->showMenu();
    else
        this->hideMenu();
}

void SMenu::showMenu()
{
    if( p->animation->isActive() || !this->isHidden() )
        return ;

    this->snapCheck();
    if(this->p->animState)
    {
        p->snapWidget.stt = false;
        QSize target(this->width(),this->height());
        this->resize(0,0);

        p->Main->hide();
        p->default_widget->show();
        p->on_showing_anim = true;

        p->animation->start(this,target,SAnimation::RESIZE,13);
    }
    this->show();
}

void SMenu::hideMenu()
{
    if( p->animation->isActive() || this->isHidden() )
        return ;

    this->snapCheck();
    if(this->p->animState)
    {
        p->snapWidget.stt = false;
        QSize target(0,0);
        this->show();

        p->Main->hide();
        p->default_widget->show();
        p->on_showing_anim = false;

        p->animation->start(this,target,SAnimation::RESIZE,13);
        return ;
    }

    this->hide();
}

void SMenu::finishAnim()
{
    if(this->height()==0)
        this->hide();
    else
        this->show();

    p->snapWidget.stt = true;

    p->Main->show();
    p->default_widget->hide();
}

void SMenu::snapCheck()
{
    if( p->snapWidget.widget != 0 )
        p->last_size = p->snapWidget.widget->size();
    else
        p->last_size = QSize(0,0);

    this->setGeometry(  p->snapWidget.left
                      , p->snapWidget.top
                      , p->last_size.width() - p->snapWidget.left - p->snapWidget.right
                      , p->last_size.height() - p->snapWidget.top - p->snapWidget.bottom  );

    //QObject::disconnect( timer , SIGNAL(timeout()) , this , SLOT(snapCheck()) );
}

SMenu::~SMenu()
{
    delete p;
}

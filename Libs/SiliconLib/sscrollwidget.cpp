/*
 * File:      sscrollwidget.cpp
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

#include <QScrollArea>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScrollBar>
#include <QFrame>

#include "sscrollwidget.h"

#include "sstylesheet.h"
#include "smastericons.h"



/*================================*
 |                                |
 |      SScrollWidgetPrivate      |
 |                                |
 *================================*/

class SScrollWidgetPrivate
{
public:
    QFrame *frame;
    QScrollArea *scrollWidget;
    QString style;

    QHBoxLayout *hLayout;
    QVBoxLayout *vLayout;
    QHBoxLayout *frameLayout;

    QPushButton *leftArrow;
    QPushButton *rightArrow;
    QPushButton *topArrow;
    QPushButton *bottomArrow;

    QScrollBar *hScroll;
    QScrollBar *vScroll;
};



/*================================*
 |                                |
 |         SScrollWidget          |
 |                                |
 *================================*/

SScrollWidget::SScrollWidget(QWidget *parent) : QWidget( parent )
{
    p = new SScrollWidgetPrivate;

    this->setContentsMargins(0,0,0,0);
    this->setupOptions();
}

void SScrollWidget::setupOptions()
{
    p->hScroll=new QScrollBar(Qt::Horizontal);
    p->vScroll=new QScrollBar(Qt::Vertical);

    p->frameLayout=new QHBoxLayout();
        p->frameLayout->setContentsMargins(0,0,0,0);
        p->frameLayout->setSpacing(0);

    p->frame = new QFrame();
        p->frame->setLayout( p->frameLayout );

    p->scrollWidget=new QScrollArea();
        p->scrollWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        p->scrollWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        p->scrollWidget->setFrameShadow(QFrame::Sunken);
        p->scrollWidget->setWidgetResizable(true);
        p->scrollWidget->setHorizontalScrollBar( p->hScroll );
        p->scrollWidget->setVerticalScrollBar( p->vScroll );
        p->scrollWidget->setFocusPolicy(Qt::NoFocus);
        p->scrollWidget->setWidget( p->frame );
        p->scrollWidget->viewport()->setAutoFillBackground( false );

    QFont font = QPushButton().font();
        font.setBold(true);

    p->leftArrow=new QPushButton();
        p->leftArrow->setFocusPolicy(Qt::NoFocus);
        p->leftArrow->setFont( font );
        p->leftArrow->setFlat( true );

    p->rightArrow=new QPushButton();
        p->rightArrow->setFocusPolicy(Qt::NoFocus);
        p->rightArrow->setFont( font );
        p->rightArrow->setFlat( true );

    p->topArrow=new QPushButton();
        p->topArrow->setFocusPolicy(Qt::NoFocus);
        p->topArrow->setFlat( true );

    p->bottomArrow=new QPushButton();
        p->bottomArrow->setFocusPolicy(Qt::NoFocus);
        p->bottomArrow->setFlat( true );

    setArrowsSize(QSize(20,30));

    QIcon left_arrow_icon( SMasterIcons::icon( QSize(48,48) , "arrow-left.png" ) );
    QIcon right_arrow_icon( SMasterIcons::icon( QSize(48,48) , "arrow-right.png" ) );

    p->leftArrow->setIcon( left_arrow_icon );
    p->rightArrow->setIcon( right_arrow_icon );

    if( left_arrow_icon.isNull() )
        p->leftArrow->setText("<");
    if( right_arrow_icon.isNull() )
        p->rightArrow->setText(">");


    p->hLayout=new QHBoxLayout();
        p->hLayout->addWidget(p->leftArrow);
        p->hLayout->addWidget(p->scrollWidget);
        p->hLayout->addWidget(p->rightArrow);
        p->hLayout->setContentsMargins(0,0,0,0);
        p->hLayout->setSpacing(0);

    p->vLayout=new QVBoxLayout(this);
        p->vLayout->addWidget(p->topArrow);
        p->vLayout->addLayout(p->hLayout);
        p->vLayout->addWidget(p->bottomArrow);
        p->vLayout->setAlignment( p->topArrow , Qt::AlignHCenter);
        p->vLayout->setAlignment( p->bottomArrow , Qt::AlignHCenter);
        p->vLayout->setContentsMargins(0,0,0,0);
        p->vLayout->setSpacing(0);

    QObject::connect( p->leftArrow   ,SIGNAL(clicked())       ,this        ,SLOT(leftArrowClick())   );
    QObject::connect( p->rightArrow  ,SIGNAL(clicked())       ,this        ,SLOT(rightArrowClick())  );
    QObject::connect( this           ,SIGNAL(setHScroll(int)) ,p->hScroll  ,SLOT(setValue(int))      );

    QObject::connect( p->topArrow    ,SIGNAL(clicked())       ,this        ,SLOT(topArrowClick())    );
    QObject::connect( p->bottomArrow ,SIGNAL(clicked())       ,this        ,SLOT(bottomArrowClick()) );
    QObject::connect( this           ,SIGNAL(setVScroll(int)) ,p->vScroll  ,SLOT(setValue(int))      );
}

void SScrollWidget::setWidget(QWidget *widget)
{
    p->frameLayout->addWidget( widget );
}

void SScrollWidget::setArrowsSize(QSize size)
{
    this->setLeftArrowSize(size);
    this->setRightArrowSize(size);
    this->setTopArrowSize(QSize(size.height(),size.width()));
    this->setBottomArrowSize(QSize(size.height(),size.width()));
}

void SScrollWidget::setLeftArrowSize(QSize size)
{
    p->leftArrow->setMinimumSize(size);
    p->leftArrow->setMaximumSize(size);
    p->leftArrow->setMinimumSize(size);
}

void SScrollWidget::setRightArrowSize(QSize size)
{
    p->rightArrow->setMinimumSize(size);
    p->rightArrow->setMaximumSize(size);
    p->rightArrow->setMinimumSize(size);
}

void SScrollWidget::setTopArrowSize(QSize size)
{
    p->topArrow->setMinimumSize(size);
    p->topArrow->setMaximumSize(size);
    p->topArrow->setMinimumSize(size);
}

void SScrollWidget::setBottomArrowSize(QSize size)
{
    p->bottomArrow->setMinimumSize(size);
    p->bottomArrow->setMaximumSize(size);
    p->bottomArrow->setMinimumSize(size);
}

void SScrollWidget::setHorizontalStep(int size)
{
    p->hScroll->setPageStep(size);
}

void SScrollWidget::setVerticalStep(int size)
{
    p->vScroll->setPageStep(size);
}

void SScrollWidget::setHorizontalValue( int value )
{
    p->hScroll->setValue( value );
}

void SScrollWidget::setVerticalValue( int value )
{
    p->vScroll->setValue( value );
}

void SScrollWidget::setEnableHorizontalArrow(bool stt)
{
    p->leftArrow->setHidden(!stt);
    p->rightArrow->setHidden(!stt);
}

void SScrollWidget::setEnableVerticalArrow(bool stt)
{
    p->topArrow->setHidden(!stt);
    p->bottomArrow->setHidden(!stt);
}

void SScrollWidget::setStyleSheet(const QString & styleSheet)
{
    p->style=styleSheet;

    SStyleSheet css;
        css.addStyle("TabBar",styleSheet);
        css.replace("Frame","QScrollArea");
        css.replace("Back","QFrame");
        css.replace("Arrow","QPushButton");

    p->scrollWidget->setStyleSheet( css.filter("QScrollArea") );
    p->frame->setStyleSheet( css.filter("QFrame") );

    SStyleSheet tmpCss;
        tmpCss.addStyle(css.style());
        tmpCss.replace("LeftArrow","QPushButton");
    p->leftArrow->setStyleSheet( tmpCss.filter("QPushButton") );

    tmpCss.clear();
        tmpCss.addStyle(css.style());
        tmpCss.replace("RightArrow","QPushButton");
    p->rightArrow->setStyleSheet( tmpCss.filter("QPushButton") );

    tmpCss.clear();
        tmpCss.addStyle(css.style());
        tmpCss.replace("TopArrow","QPushButton");
    p->topArrow->setStyleSheet( tmpCss.filter("QPushButton") );

    tmpCss.clear();
        tmpCss.addStyle(css.style());
        tmpCss.replace("BottomArrow","QPushButton");
    p->bottomArrow->setStyleSheet( tmpCss.filter("QPushButton") );
}



QSize SScrollWidget::leftArrowSize() const
{
    return p->leftArrow->maximumSize();
}

QSize SScrollWidget::rightArrowSize() const
{
    return p->rightArrow->maximumSize();
}

QSize SScrollWidget::topArrowSize() const
{
    return p->topArrow->maximumSize();
}

QSize SScrollWidget::bottomArrowSize() const
{
    return p->bottomArrow->maximumSize();
}

int SScrollWidget::horizontalStep() const
{
    return p->hScroll->pageStep();
}

int SScrollWidget::verticalStep() const
{
    return p->vScroll->pageStep();
}

int SScrollWidget::horizontalValue() const
{
    return p->hScroll->value();
}

int SScrollWidget::verticalValue() const
{
    return p->vScroll->value();
}

int SScrollWidget::maximumHorizontal() const
{
    return p->hScroll->maximum();
}

int SScrollWidget::maximumVertical() const
{
    return p->vScroll->maximum();
}



void SScrollWidget::leftArrowClick()
{
    int value= p->hScroll->value()
       ,sec= p->hScroll->pageStep()/2;

    emit this->setHScroll( (value>sec)?value-sec:0 );
}

void SScrollWidget::rightArrowClick()
{
    int value= p->hScroll->value()
       ,max= p->hScroll->maximum()
       ,sec= p->hScroll->pageStep()/2;

    emit this->setHScroll( (value<max)?value+sec:max );
}

void SScrollWidget::topArrowClick()
{
    int value= p->vScroll->value()
       ,sec= p->vScroll->pageStep()/2;

    emit this->setVScroll( (value>sec)?value-sec:0 );
}

void SScrollWidget::bottomArrowClick()
{
    int value= p->vScroll->value()
       ,max= p->vScroll->maximum()
       ,sec= p->vScroll->pageStep()/2;

    emit this->setVScroll( (value<max)?value+sec:max );
}

SScrollWidget::~SScrollWidget()
{
    delete p;
}

/*
 * File:      sinfowidget.h
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

#include "sinfowidget.h"

#include <QPalette>
#include <QColor>
#include <QFont>
#include <QSpacerItem>
#include <QPixmap>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTextBrowser>
#include <QFrame>
#include <QIcon>

#include "spicturewidget.h"
#include "sstylesheet.h"




/*================================*
 |                                |
 |       SInfoWidgetPrivate       |
 |                                |
 *================================*/

class SInfoWidgetPrivate
{
public:
    QHBoxLayout *baseLayout;
    QVBoxLayout *layout;

    QFrame *frame;

    SPictureWidget *picWidget;
    QPixmap pix;
    QLabel *titleLabel;
    QTextBrowser *information;
};




/*================================*
 |                                |
 |           SInfoWidget          |
 |                                |
 *================================*/

SInfoWidget::SInfoWidget(QWidget *parent) : QWidget( parent )
{
    p = new SInfoWidgetPrivate;

    QFont font = this->font() ;
        font.setBold(true);
        font.setItalic(true);

    p->titleLabel=new QLabel();
        p->titleLabel->setFont( font );

    p->picWidget=new SPictureWidget();
        p->picWidget->setFixedHeight(175);
        p->picWidget->setKeepProportion( true );

    QPalette palette=this->palette();
        palette.setColor(QPalette::Base,QColor(255,255,255,0));

    p->information=new QTextBrowser();
        p->information->setFrameShadow(QFrame::Plain);
        p->information->setPalette(palette);

    p->layout=new QVBoxLayout();
        p->layout->addWidget(p->titleLabel);
        p->layout->addWidget(p->picWidget);
        p->layout->addWidget(p->information);
        p->layout->setContentsMargins(4,13,4,4);
        p->layout->setSpacing(7);

    p->frame = new QFrame();
        p->frame->setLayout( p->layout );

    p->baseLayout=new QHBoxLayout(this);
        p->baseLayout->addWidget(p->frame);
        p->baseLayout->setContentsMargins(0,0,0,0);
        p->baseLayout->setSpacing(0);
}

void SInfoWidget::setData(const QString & picAddress,const QString & title,const QString & description)
{
    this->setPicture(picAddress);
    this->setTitle(title);
    this->setDescription(description);
}

void SInfoWidget::setPicture(const QPixmap & pix)
{
    p->picWidget->drawPixmap( pix );
    p->pix = pix;
}

void SInfoWidget::setTitle(const QString & title)
{
    this->p->titleLabel->setText(title);
}

void SInfoWidget::setDescription(const QString & description)
{
    p->information->setText( description );
}

void SInfoWidget::clear()
{
    this->setPicture( QPixmap() );
    this->setTitle( NULL );
    this->setDescription( NULL );
}

void SInfoWidget::setMaximumPictureSize(const QSize & size)
{
    p->picWidget->setMaximumSize(size);
}

void SInfoWidget::setMinimumPictureSize(const QSize & size)
{
    p->picWidget->setMinimumSize(size);
}

void SInfoWidget::setStyleSheet(const QString & style)
{

    SStyleSheet css;
        css.addStyle( style );
        css.replace( "SInfoWidget" , "QFrame" );

    p->frame->setStyleSheet( css.style() );

    p->titleLabel->setStyleSheet( "QFrame{background-color: solid;border-style: solid; background-image: url()}" );
    p->information->setStyleSheet( "QFrame{background-color: solid;border-style: solid; background-image: url()}" );
}

QPixmap SInfoWidget::picture() const
{
    return p->pix;
}

QString SInfoWidget::title() const
{
    return p->titleLabel->text();
}

SInfoWidget::~SInfoWidget()
{
    delete p;
}


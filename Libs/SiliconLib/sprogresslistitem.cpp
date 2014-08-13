/*
 * File:      sprogresslistitem.cpp
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

#include "sprogresslistitem.h"

#include <QSize>
#include <QStyle>
#include <QStyleOptionViewItem>
#include <QPalette>
#include <QColor>
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QSpacerItem>
#include <QFrame>

#include "sapplication.h"
#include "spage.h"
#include "sanimation.h"
#include "sstylesheet.h"
#include "scolor.h"
#include "smastericons.h"
#include "spicturewidget.h"



/*================================*
 |                                |
 |    SProgressListItemPrivate    |
 |                                |
 *================================*/

class SProgressListItemPrivate
{
public:
    QFrame *frame;
    QLabel *titleLabel;
    QPushButton *stopButton;
    QPushButton *moreButton;
    QProgressBar *progressBar;
    QLineEdit *infsLine;
    SPictureWidget *icon;

    QWidget *fake_extra_widget;
    QWidget *extra_widget;

    SAnimation *animation;
    SApplication *parent;

    QHBoxLayout *layout;
    QVBoxLayout *baseVLayout;
    QHBoxLayout *progressHLayout;

    QString style;
};



/*================================*
 |                                |
 |       SProgressListItem        |
 |                                |
 *================================*/

SProgressListItem::SProgressListItem( SPage *parent )
{
    SApplication *app = parent->parentApp();
        app->addProgress( this );

    setup();
    p->parent = app;
}

SProgressListItem::SProgressListItem( SApplication *parent )
{
    parent->addProgress( this );

    setup();
    p->parent = parent;
}

void SProgressListItem::setup()
{
    p = new SProgressListItemPrivate;

    setStyleSheet( "QFrame{ background-color: transparent; }" );

    p->animation = new SAnimation( this );
        p->animation->setFixOptions( SAnimation::WIDTH_FIX );
        p->animation->setFrameRate( 10 );

    QFont font = QWidget::font();
        font.setBold( true );
        font.setItalic( true );

    QColor color = palette().color( QPalette::Base );
        color.setAlpha( 0 );

    QPalette palette1 = palette();
        palette1.setColor( QPalette::Base , color );

    color.setAlpha(73);

    QPalette palette2 = palette();
        palette2.setColor( QPalette::Window , color );

    p->titleLabel = new QLabel();
        p->titleLabel->setFont( font );

    p->infsLine = new QLineEdit();
        p->infsLine->setReadOnly( true );

    p->fake_extra_widget = new QWidget();
        p->fake_extra_widget->setHidden( true );
        p->fake_extra_widget->setSizePolicy( QSizePolicy::Minimum ,QSizePolicy::Maximum );
        p->fake_extra_widget->setFixedHeight( 75 );

    p->extra_widget = new QWidget();
        p->extra_widget->setSizePolicy( QSizePolicy::Minimum ,QSizePolicy::Maximum );
        p->extra_widget->setHidden( true );
        p->extra_widget->setFixedHeight( 75 );

    p->progressBar = new QProgressBar();
        p->progressBar->setValue( 0 );
        p->progressBar->setPalette( palette2 );

    p->stopButton = new QPushButton();
        p->stopButton->setFixedSize( 24 , 24 );
        p->stopButton->setIcon( SMasterIcons::icon( QSize(48,48) , "process-stop.png" ) );

    p->moreButton = new QPushButton();
        p->moreButton->setFixedSize( 75 , 24 );
        p->moreButton->setText( tr("More") );
        p->moreButton->setIcon( SMasterIcons::icon( QSize(48,48) , "go-down.png" ) );

    p->progressHLayout = new QHBoxLayout();
        p->progressHLayout->addWidget( p->progressBar );
        p->progressHLayout->addWidget( p->stopButton );
        p->progressHLayout->addWidget( p->moreButton );
        p->progressHLayout->setContentsMargins(0,0,0,0);
        p->progressHLayout->setSpacing(0);

    p->baseVLayout = new QVBoxLayout();
        p->baseVLayout->addWidget( p->titleLabel );
        p->baseVLayout->addWidget( p->infsLine );
        p->baseVLayout->addWidget( p->fake_extra_widget );
        p->baseVLayout->addWidget( p->extra_widget );
        p->baseVLayout->addLayout( p->progressHLayout );
        p->baseVLayout->setContentsMargins(0,0,0,0);
        p->baseVLayout->setSpacing(0);

    p->frame = new QFrame();
        p->frame->setLayout( p->baseVLayout );

    p->icon = new SPictureWidget();
        p->icon->setFixedSize( 48 , 48 );

    p->layout = new QHBoxLayout(this);
    p->layout->addWidget( p->icon );
        p->layout->addWidget( p->frame );
        p->layout->setSpacing(0);
        p->layout->setContentsMargins( 0 , 0 , 0 , 0 );

    QObject::connect( p->moreButton , SIGNAL(clicked()) , this , SLOT(autoExpand())        );
    QObject::connect( p->animation , SIGNAL(finished()) , this , SLOT(animationFinished()) );

    registerItem();
}

void SProgressListItem::setTitle(const QString & name)
{
    p->titleLabel->setText( name );
}

void SProgressListItem::setInfoText(const QString & str)
{
    p->infsLine->setText( str );
}

void SProgressListItem::setIcon(const QIcon & icon)
{
    p->icon->drawIcon( icon , QSize(48,48) );
}

void SProgressListItem::showExtra(bool stt)
{
    if( stt && p->extra_widget->isHidden() )
    {
        p->moreButton->setText( tr("Less") );
        p->moreButton->setIcon( SMasterIcons::icon( QSize(48,48) , "go-up.png" ) );

        p->fake_extra_widget->setFixedHeight( 0 );
        p->fake_extra_widget->show();

        p->animation->start( p->fake_extra_widget , QSize(0,75) ,SAnimation::RESIZE , 13 , SAnimation::MIN_MAX );
    }
    else if( !stt && !p->extra_widget->isHidden() )
    {
        p->moreButton->setText( tr("More") );
        p->moreButton->setIcon( SMasterIcons::icon( QSize(48,48) , "go-down.png" ) );

        p->fake_extra_widget->show();
        p->extra_widget->hide();

        p->animation->start( p->fake_extra_widget , QSize(0,0) ,SAnimation::RESIZE , 13 , SAnimation::MIN_MAX );
    }
}

int SProgressListItem::value() const
{
    return p->progressBar->value();
}

QPushButton *SProgressListItem::cancelButton() const
{
    return p->stopButton;
}

QWidget *SProgressListItem::extraWidget() const
{
    return p->extra_widget;
}

void SProgressListItem::hideExtra(bool stt)
{
    showExtra( !stt );
}

void SProgressListItem::setValue(int value)
{
    p->progressBar->setValue( value );
    emit valueChanged( value );
}

void SProgressListItem::autoExpand()
{
    if(p->extra_widget->isHidden())
        showExtra( true );
    else
        hideExtra( true );
}

void SProgressListItem::animationFinished()
{
    bool ok = (p->fake_extra_widget->height() == 0);

    p->fake_extra_widget->setHidden( true );
    p->extra_widget->setHidden( ok );
}

SProgressListItem::~SProgressListItem()
{
    emit destroyed( this );
    delete p;
}

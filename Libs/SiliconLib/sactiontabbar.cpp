#include "sactiontabbar.h"

#include <QHBoxLayout>
#include <QList>
#include <QToolBar>

class SActionTabBarPrivate
{
public:
    QHBoxLayout *layout;
    QToolBar *toolbar;
    QList<QAction*> list;
    QAction *current_item;
};




SActionTabBar::SActionTabBar(QWidget *parent) :
    QWidget(parent)
{
    p = new SActionTabBarPrivate;

    QPalette palette = this->palette();
        palette.setColor( QPalette::Window , QColor(0,0,0,0) );
        palette.setColor( QPalette::WindowText , QColor(255,255,255) );

    p->current_item = 0;
    p->toolbar = new QToolBar();
        p->toolbar->setToolButtonStyle( Qt::ToolButtonIconOnly );
        p->toolbar->setIconSize( QSize(22,22) );
        p->toolbar->setAutoFillBackground( false );
        p->toolbar->setStyleSheet("QToolBar{border-style:solid}");
        p->toolbar->setPalette( palette );

    p->layout = new QHBoxLayout( this );
        p->layout->addWidget( p->toolbar );
        p->layout->setContentsMargins( 0 , 0 , 0 , 0 );

    QObject::connect( p->toolbar , SIGNAL(actionTriggered(QAction*)) , this , SLOT(actionTriggered(QAction*)) );
}

QAction *SActionTabBar::add( const QIcon & icon , const QString & name )
{
    QAction *action = new QAction( icon , name , this );

    this->add( action );
    return action;
}

void SActionTabBar::add( QAction *action )
{
    p->list << action;
    p->toolbar->addAction( action );

    if( p->current_item == 0 )
        this->setCurrentIndex( 0 );

    emit this->tabAdded();
}

QAction *SActionTabBar::insert( int index , const QIcon & icon , const QString & name )
{
    QAction *action = new QAction( icon , name , this );

    this->insert( index , new QAction( icon , name , this ) );
    return action;
}

void SActionTabBar::insert( int index , QAction *action )
{
    QAction *before = 0;
    if( index != 0 )
        before = p->list.at( index-1 );

    p->list.insert( index , action );
    p->toolbar->insertAction( before , action );

    if( p->current_item == 0 )
        this->setCurrentIndex( 0 );

    emit this->tabAdded();
}

void SActionTabBar::removeAt( int index )
{
    this->removeOne( p->list.at(index) );
}

void SActionTabBar::removeOne( QAction *action )
{
    if( p->current_item == action )
    {
        int index = p->list.indexOf(action);
        if( index > 0 )
            this->setCurrentIndex( index-1 );
        else if( p->list.count() > 1 )
            this->setCurrentIndex( index+1 );
        else
            p->current_item = 0;
    }

    p->list.removeOne( action );
    p->toolbar->removeAction( action );
}

int SActionTabBar::count() const
{
    return p->list.count();
}

void SActionTabBar::clear()
{
    p->list.clear();
    p->toolbar->clear();
}

bool SActionTabBar::isEmpty() const
{
    return p->list.isEmpty();
}

int SActionTabBar::currentIndex() const
{
    return p->list.indexOf( p->current_item );
}

QAction *SActionTabBar::currentItem() const
{
    return p->current_item;
}

QAction *SActionTabBar::itemAt( int index ) const
{
    return p->list.at(index);
}

int SActionTabBar::indexOf( QAction *action ) const
{
    return p->list.indexOf(action);
}

void SActionTabBar::setOrientation( Qt::Orientation orientation )
{
    p->toolbar->setOrientation( orientation );
}

Qt::Orientation SActionTabBar::orientation() const
{
    return p->toolbar->orientation();
}

void SActionTabBar::setCurrentIndex( int index )
{
    if( p->current_item != 0 )
        p->current_item->setChecked( false );

    p->current_item = p->list.at(index);
    p->current_item->setCheckable( true );
    p->current_item->setChecked( true );

    emit this->currentIndexChanged( index );
}

void SActionTabBar::actionTriggered( QAction *action )
{
    this->setCurrentIndex( p->list.indexOf(action) );
}

SActionTabBar::~SActionTabBar()
{
    delete p;
}

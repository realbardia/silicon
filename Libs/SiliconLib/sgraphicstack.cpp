#include "sgraphicstack.h"

#include <QList>
#include <QHBoxLayout>

class SGraphicStackPrivate
{
public:
    QList<SGraphicStackItem *> list;
    QHBoxLayout *layout;

    int current_index;
};

SGraphicStack::SGraphicStack( QWidget *parent ) :
    QWidget(parent)
{
    p = new SGraphicStackPrivate;
        p->current_index = -1;

    p->layout = new QHBoxLayout( this );
        p->layout->setContentsMargins( 0 , 0 , 0 , 0 );
}

void SGraphicStack::push( const QString & str )
{
    push( QIcon() , str );
}

void SGraphicStack::push( const QIcon & icon , const QString & str )
{
    SGraphicStackItem *item = new SGraphicStackItem( str );
        item->setIcon( icon );

    p->layout->addWidget( item );
    p->list << item;

    if( p->current_index != -1 )
        p->list[ p->current_index ]->setChecked( false );

    p->current_index = p->list.count()-1;
    recheck();

    connect( item , SIGNAL(clicked(SGraphicStackItem*)) , SLOT(clicked(SGraphicStackItem*)) );
}

QString SGraphicStack::pop()
{
    SGraphicStackItem *item = p->list.last();
    QString str = item->text();

    p->list.removeLast();
    p->current_index = p->list.count()-1;
    p->layout->removeWidget( item );

    recheck();

    delete item;
    return str;
}

QString SGraphicStack::top() const
{
    return p->list.last()->text();
}

QStringList SGraphicStack::toStringList() const
{
    QStringList result;
    for( int i=0 ; i<p->current_index+1 ; i++ )
        result << p->list.at(i)->text();

    return result;
}

QStringList SGraphicStack::allToStringList() const
{
    QStringList result;
    for( int i=0 ; i<p->list.count() ; i++ )
        result << p->list.at(i)->text();

    return result;
}

int SGraphicStack::count() const
{
    return p->list.count();
}

bool SGraphicStack::isEmpty() const
{
    return p->list.isEmpty();
}

QString SGraphicStack::at( int index )
{
    return p->list.at(index)->text();
}

int SGraphicStack::currentIndex() const
{
    return p->current_index;
}

QString SGraphicStack::current() const
{
    return p->list.at( p->current_index )->text();
}

void SGraphicStack::setCurrentIndex( int index , bool signal_on )
{
    if( p->current_index != -1 )
        p->list[ p->current_index ]->setChecked( false );

    p->current_index = index;
    recheck();

    if( !signal_on )
        return ;

    emit currentIndexChanged( p->current_index );
    if( p->current_index != -1 )
        emit currentChanged( p->list.at( p->current_index )->text() );
}

void SGraphicStack::clear()
{
    while( !p->list.isEmpty() )
        pop();

    p->current_index = -1;
}

void SGraphicStack::recheck()
{
    if( p->current_index == -1 )
        return ;

    p->list[ p->current_index ]->setChecked( true );
}

void SGraphicStack::clicked( SGraphicStackItem *item )
{
    int index = p->list.indexOf( item );
    setCurrentIndex( index );
}

SGraphicStack::~SGraphicStack()
{
    delete p;
}





SGraphicStackItem::SGraphicStackItem( const QString & text , QWidget *parent )
    : QToolButton( parent )
{
    setText( text );
    setAutoRaise( true );
    setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    setCheckable( true );
    setFixedHeight( 28 );
    //setFixedWidth( 73 );
    connect( this , SIGNAL(clicked()) , SLOT(click_on()) );
}

void SGraphicStackItem::click_on()
{
    emit clicked( this );
}

SGraphicStackItem::~SGraphicStackItem()
{
}

#include "converterline.h"

#include <SConverter>
#include <SColor>

#include <QComboBox>
#include <QHBoxLayout>
#include <QPainter>
#include <QFont>
#include <QPainterPath>
#include <QLinearGradient>
#include <QDebug>

class ConverterLinePrivate
{
public:
    QStringList way;
    SConverter *converter;

    QComboBox *src_combo;
    QComboBox *dst_combo;

    QHBoxLayout *layout;
};

ConverterLine::ConverterLine( SConverter *converter , QWidget *parent ) :
    QWidget(parent)
{
    p = new ConverterLinePrivate;
    p->converter = converter;

    p->src_combo = new QComboBox();
    p->dst_combo = new QComboBox();

    p->layout = new QHBoxLayout( this );
        p->layout->addWidget( p->src_combo );
        p->layout->addStretch();
        p->layout->addWidget( p->dst_combo );

    connect( p->src_combo , SIGNAL(currentIndexChanged(int)) , SLOT(src_changed()) );
    connect( p->dst_combo , SIGNAL(currentIndexChanged(int)) , SLOT(update_slt())  );

    refresh();
}

QString ConverterLine::sourceType() const
{
    return p->src_combo->currentText();
}

QString ConverterLine::destinationType() const
{
    return p->dst_combo->currentText();
}

void ConverterLine::setSourceType( const QString & type )
{
    for( int i=0 ; i<p->src_combo->count() ; i++ )
        if( p->src_combo->itemText(i).compare(type,Qt::CaseInsensitive) == 0 )
        {
            p->src_combo->setCurrentIndex( i );
            break;
        }
}

void ConverterLine::setDestinationType( const QString & type )
{
    for( int i=0 ; i<p->dst_combo->count() ; i++ )
        if( p->dst_combo->itemText(i).compare(type,Qt::CaseInsensitive) == 0 )
        {
            p->dst_combo->setCurrentIndex( i );
            break;
        }
}

void ConverterLine::update_slt()
{
    emit updated();
    repaint();
}

void ConverterLine::refresh()
{
    p->src_combo->clear();
    const QStringList & keys = p->converter->availableWays().uniqueKeys();
    for( int i=0 ; i<keys.count() ; i++ )
        p->src_combo->addItem( keys.at(i) );
}

void ConverterLine::src_changed()
{
    p->dst_combo->clear();
    const QList<QString> & values = p->converter->availableWays().values( p->src_combo->currentText() );
    for( int i=0 ; i<values.count() ; i++ )
        p->dst_combo->addItem( values.at(i) );

    repaint();
}

void ConverterLine::paintEvent( QPaintEvent * )
{
    if( p->src_combo->count() == 0 || p->dst_combo->count() == 0 )
        return;

    QStringList way = p->converter->wayOf( p->src_combo->currentText() , p->dst_combo->currentText() );
    if( way.isEmpty() )
        return;

    for( int i=0 ; i<way.count() ; i++ )
        if( i%2 == 1 )
            way.insert( i , ">>>" );

    const int ways_count = way.count()-2;

    QFont font;
        font.setBold( true );

    QRect rct = rect();
        rct.setLeft( p->src_combo->width() + 10 );
        rct.setWidth( rct.width() - p->dst_combo->width() - 11 );
        rct.setTop( (rct.height() - p->src_combo->height())/2 );
        rct.setHeight( p->src_combo->height() );

    QPainter painter( this );
        painter.setRenderHint( QPainter::Antialiasing , true );
        painter.setPen( palette().highlightedText().color() );
        painter.setFont( font );

    SColor color1( palette().highlight().color() );
    SColor color2( palette().highlight().color() );
        color2 = color2 * 0.3;

    QLinearGradient gradiant(QPointF(0, 0), QPointF(0,rct.height()));
        gradiant.setColorAt(0, color1);
        gradiant.setColorAt(1, color2);

    for( int i=0 ; i<ways_count ; i++ )
    {
        QRect temp_rect = rct;
            temp_rect.setLeft( rct.left() + i*rct.width() / ways_count + 1*i );
            temp_rect.setWidth( rct.width() / ways_count - 1 );

        QPainterPath path;
            path.setFillRule( Qt::WindingFill );
            path.addRoundRect( temp_rect  , 37 );

        painter.fillPath( path , gradiant );
        painter.drawText( temp_rect , Qt::AlignCenter|Qt::AlignVCenter , way.at(i+1) );
    }
}

ConverterLine::~ConverterLine()
{
    delete p;
}

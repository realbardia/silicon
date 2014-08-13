#define SPLITTER_STR "->"

#include "sconverter.h"
#include "sabstractconverterengine.h"
#include "siditools.h"

#include <QHash>
#include <QMultiHash>
#include <QList>
#include <QDebug>

#include <Silicon>

QList<QStringList> steps_list;
QMultiHash<QString,SAbstractConverterEngine*> converters_hash;
QMultiHash<QString,QString> finded_ways;


/*================================*
 |                                |
 |         Extra Functions        |
 |                                |
 *================================*/

void supportedConverts( const QList<QStringList> & two_steps_ways , const QList<QStringList> & before )
{
    if( before.isEmpty() )
        return;

    QList<QStringList> pass_to_next;

    for( int i=0 ; i<before.count() ; i++ )
    {
        const QStringList & left = before.at(i);
        for( int j=0 ; j<two_steps_ways.count() ; j++ )
        {
            const QStringList & right = two_steps_ways.at(j);
            if( left.last() == right.first() && !left.contains(right.last()) && !finded_ways.contains(left.first(),right.last()) )
            {
                pass_to_next << left + QStringList(right.last());
                steps_list   << left + QStringList(right.last());
                finded_ways.insert( left.first() , right.last() );
            }
        }
    }

    supportedConverts( two_steps_ways , pass_to_next );
}

void refresh()
{
    const QStringList & source = converters_hash.uniqueKeys();

    /*! -------- Pre Start: Find ways with 2 steps --------
        --------            Find available ways    -------- */
    steps_list.clear();
    finded_ways.clear();
    for( int i=0 ; i<source.count() ; i++ )
        steps_list << source.at(i).split( SPLITTER_STR );

    QList<QStringList> two_steps_ways;
    for( int i=0 ; i<steps_list.count() ; i++ )
    {
        QString src = steps_list.at(i).first();
        for( int j=0 ; j<steps_list.count() ; j++ )
            if( steps_list.at(j).last() == src )
            {
                two_steps_ways << steps_list.at(i);
                break;
            }

        finded_ways.insert( src , steps_list.at(i).last() );
    }


    /*! -------- Start: Find ways with more than 2 steps -------- */
    supportedConverts( two_steps_ways , two_steps_ways );

    /*! ----------- Clear extra created datas ------------ */
    finded_ways.clear();

    /*! ----------- Reset finded ways ------------ */
    for( int i=0 ; i<steps_list.count() ; i++ )
        finded_ways.insert( steps_list.at(i).first() , steps_list.at(i).last() );
}

QStringList findWay( const QString & src_type , const QString & dst_type )
{
    for( int i=0 ; i<steps_list.count() ; i++ )
    {
        const QStringList & steps = steps_list.at(i);
        if( steps.first() == src_type.toLower() && steps.last() == dst_type.toLower() )
            return steps;
    }

    return QStringList();
}





/*================================*
 |                                |
 |        SConverterPrivate       |
 |                                |
 *================================*/

class SConverterPrivate
{
public:
    QString source;
    QString destination;
    QString temp;

    QString src_type;
    QString dst_type;

    QString logs;
    QString used_command;

    SAbstractConverter *converter;
    int way_lenght;
    QStringList way;

    bool passUp;
};




/*================================*
 |                                |
 |           SConverter           |
 |                                |
 *================================*/

SConverter::SConverter(QObject *parent) :
    QObject(parent)
{
    p = new SConverterPrivate;
    p->converter = 0;
    p->passUp = false;
}

SAbstractConverter *SConverter::converter( const QString & source_type , const QString & destination_type , QObject *parent )
{
    SAbstractConverterEngine *engine = converters_hash.value( source_type + SPLITTER_STR + destination_type );
    if( engine == 0 )
        return 0;

    return static_cast<SAbstractConverter*>( engine->create(parent) );
}

void SConverter::addConverterEngine( SAbstractConverterEngine *engine , const QMultiHash<QString,QString> & t )
{
    QMultiHash<QString,QString> types = t;
    const QList<QString> & list = types.keys();

    for( int i=0 ; i<list.count() ; i++ )
    {
        const QString & str = list.at(i);

        QString key = str + SPLITTER_STR + types.value(str);
        types.remove( str , types.value(str) );

        converters_hash.insertMulti( key , engine );
    }

    refresh();
}

void SConverter::removeConverterEngine( SAbstractConverterEngine *engine , const QMultiHash<QString,QString> & types )
{
    const QList<QString> & list = types.keys();
    for( int i=0 ; i<list.count() ; i++ )
    {
        QString key = list.at(i) + SPLITTER_STR + types.value(list.at(i));

        if( converters_hash.contains(key,engine) )
            converters_hash.remove( key , engine );
    }

    refresh();
}

void SConverter::setSource( const QString & source , const QString & type )
{
    p->source = source;
    p->src_type = type;
}

const QString & SConverter::source() const
{
    return p->source;
}

const QString & SConverter::sourceType() const
{
    return p->src_type;
}

void SConverter::setDestination( const QString & destination , const QString & type )
{
    p->destination = destination;
    p->dst_type = type;
}

const QString & SConverter::destination() const
{
    return p->destination;
}

const QString & SConverter::destinationType() const
{
    return p->dst_type;
}

QStringList SConverter::wayOf( const QString & src_type , const QString & dst_type ) const
{
    return findWay( src_type , dst_type );
}

const QMultiHash<QString,QString> & SConverter::availableWays() const
{
    return finded_ways;
}

QHash<QString,QVariant> SConverter::optionsOf( const QString & src_type , const QString & dst_type ) const
{
    const QStringList & way = findWay( src_type , dst_type );
    if( way.count() < 2 )
        return QHash<QString,QVariant>();

    return converters_hash.value( way.at( way.count()-2 ) + SPLITTER_STR + way.last() )->optionsOf( src_type , dst_type );
}

void SConverter::setDefaultOption( const QString & src_type , const QString & dst_type , const QHash<QString,QVariant> & options )
{
    const QStringList & way = findWay( src_type , dst_type );
    if( way.count() != 2 )
        return ;

    converters_hash.value( way.at( way.count()-2 ) + SPLITTER_STR + way.last() )->setDefaultOption( src_type , dst_type , options );
}

QString SConverter::logs() const
{
    return p->logs;
}

QString SConverter::usedCommand() const
{
    return p->used_command;
}

bool SConverter::isFinished() const
{
    return !isStarted();
}

bool SConverter::isStarted() const
{
    return p->converter != 0;
}

STime SConverter::elapsedTime() const
{
    if( isFinished() )
        return STime();

    return p->converter->elapsedTime();
}

STime SConverter::remainingTime() const
{
    if( isFinished() )
        return STime();

    return p->converter->remainingTime();
}

QString SConverter::application() const
{
    if( isFinished() )
        return QString();

    return p->converter->application();
}

bool SConverter::passupAndWait() const
{
    return p->passUp;
}

void SConverter::setPassupAndWait( bool stt )
{
    p->passUp = stt;
}

void SConverter::start()
{
    if( p->converter != 0 )
    {
        qDebug( "void SConverter::start() : Convert process started before." );
        return;
    }

    p->way = findWay( p->src_type , p->dst_type );
    p->way_lenght = p->way.count()-1;

    p->logs.clear();
    start_prev();
}

void SConverter::start_prev()
{
    if( p->converter != 0 )
        delete p->converter;

    if( p->way.count() < 2 )
    {
        p->way.clear();
        p->converter = 0;

        emit finished( 0 );
        return ;
    }

    const QString & src = p->way.takeFirst();
    const QString & dst = p->way.first();


    /*! -------- Settings Up Source and Dest Address -------- */
    QString destination;
    if( p->way.count() > 1 )
        destination = Silicon::requestTempFile();
    else
        destination = p->destination;

    QString source;
    if( p->way.count() != p->way_lenght )
        source = p->temp;
    else
        source = p->source;
    /*! ----------------------------------------------------- */


    p->converter = static_cast<SAbstractConverter*>(converters_hash.value( src + SPLITTER_STR + dst )->create( this ));
        p->converter->setSource( source );
        p->converter->setDestination( destination );
        p->converter->setPassupAndWait( passupAndWait() );

    connect( p->converter , SIGNAL(finished(int))       , SLOT(start_prev())             , Qt::QueuedConnection );
    connect( p->converter , SIGNAL(logChanged(QString)) , SLOT(logChanged_slot(QString)) );
    connect( p->converter , SIGNAL(percentChanged(int)) , SLOT(percentChanged_slot(int)) );

    connect( p->converter , SIGNAL(elapsedTimeChanged(QString))   , SIGNAL(elapsedTimeChanged(QString))   );
    connect( p->converter , SIGNAL(elapsedTimeChanged(STime))     , SIGNAL(elapsedTimeChanged(STime))     );
    connect( p->converter , SIGNAL(error(QString))                , SIGNAL(error(QString))                );
    connect( p->converter , SIGNAL(goEventSignal(SProcessEvent*)) , SIGNAL(goEventSignal(SProcessEvent*)) );
    connect( p->converter , SIGNAL(preview(QByteArray))           , SIGNAL(preview(QByteArray))           );
    connect( p->converter , SIGNAL(preview(QVariant))             , SIGNAL(preview(QVariant))             );
    connect( p->converter , SIGNAL(remainingTimeChanged(QString)) , SIGNAL(remainingTimeChanged(QString)) );
    connect( p->converter , SIGNAL(remainingTimeChanged(STime))   , SIGNAL(remainingTimeChanged(STime))   );
    connect( p->converter , SIGNAL(stopSignal())                  , SIGNAL(stopSignal())                  );
    connect( p->converter , SIGNAL(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) ,
                            SIGNAL(itemicLogAdded(SAbstractProcess::ItemicLogType,QString)) );


    p->temp = destination;
    p->converter->start( destination );
}

void SConverter::stop()
{
    p->way.clear();
    p->converter->stop();
}

void SConverter::logChanged_slot( const QString & log )
{
    p->logs = log;
    emit logChanged( p->logs );
}

void SConverter::percentChanged_slot( int percent )
{
    emit percentChanged( ( p->way_lenght - p->way.count() + 1 ) * percent / p->way_lenght );
}

SConverter::~SConverter()
{
    delete p;
}

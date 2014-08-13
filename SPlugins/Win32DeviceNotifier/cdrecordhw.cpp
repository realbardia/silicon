#include "cdrecordhw.h"

#include <QDir>
#include <QRegExp>
#include <QFile>
#include <QProcess>
#include <QTimer>
#include <QQueue>
#include <QHash>

#include <limits.h>
#include <SVariableConverter>
#include <SDesktopFunctions>


class CdRecordHWPrivate
{
public:
    QProcess *bus_process;
    QProcess *devices_process;
    QProcess *prcap_process;

    QString command;


    QList<SDeviceItem> devices;
};

CdRecordHW::CdRecordHW( QObject *parent ) : QObject(parent)
{
    p = new CdRecordHWPrivate;

    p->bus_process     = new QProcess( this );
    p->devices_process = new QProcess( this );
    p->prcap_process   = new QProcess( this );

    connect( p->bus_process     , SIGNAL(finished(int))             , this , SLOT(scanbus_priv())     );
    connect( p->devices_process , SIGNAL(finished(int))             , this , SLOT(scanDevices_priv()) );
    connect( p->prcap_process   , SIGNAL(finished(int))             , this , SLOT(prcap_prev())       );

    this->init();
}

void CdRecordHW::init()
{
    QStringList list_of_programs;
        list_of_programs << "cdrecord" << "wodim";

    for( int i=0 ; i<list_of_programs.count() ; i++ )
    {
#ifdef Q_OS_WIN32
        QDir dir( SDesktopFunctions::binaryPath() );
            dir.cdUp();

        p->command = dir.path() + "/cdrtools/" + list_of_programs.at(i) + ".exe";
        p->command.replace( "/" , "\\" );
#else
        p->command = "/usr/bin/" + list_of_programs.at(i);
#endif

        if( QFile::exists( p->command ) )
            break;
    }
}

void CdRecordHW::scanbus()
{
    p->devices.clear();
    p->bus_process->start( p->command , QStringList()<<"-scanbus" );
}

void CdRecordHW::scanbus_priv()
{
    QStringList list = QString(p->bus_process->readAll()).split('\n');

    QString str
           ,name;

    for( int i=0 ; i<list.count()-1 ; i++ )
    {
        str = list.at(i);

        QString tmp;
        for( int i=0 ; i<str.size() ; i++ )
            if( str[i] == ',' || str[i] == ')' || str[i] == '\'' )
                tmp.append( str[i] );

        if( str.right(1) == "*" || tmp.left(9) != ",,)''''''" )
            continue;
/*
        if( !str.contains( "CD-ROM" , Qt::CaseInsensitive ) );
            continue;
*/
        for( int j=0 ; j<str.size() ; j++ )
            if( str[j] == ')' )
            {
                name = str.right( str.size() - j - 1 );
                break;
            }

        SDeviceItem item;
            item.setName( name.replace( QRegExp("\\s+") , " " ) );
            item.setString( str.split(' ',QString::SkipEmptyParts).at(0) );

        p->devices << item;
    }

    emit this->busScaned();
    p->devices_process->start( p->command , QStringList()<<"-devices" );
}

void CdRecordHW::scanDevices_priv()
{
    QStringList list = QString(p->devices_process->readAll()).split('\n');

    QString str;
    QRegExp reg;
    int pos;

    int defines=0;
    for( int i=0 ; i<list.count()-1 ; i++ )
    {
        str = list.at(i);
        reg.setPattern( "\\d\\s+dev\\=\\'(\\D*\\d*)\\'\\s+.+" );
        pos = reg.indexIn( str );
        if( pos > -1 && defines < p->devices.count() )
        {
            //p->devices[defines].setAddress( reg.cap(1) );
            defines++;
        }
    }

    emit this->devicesScaned();
}

void CdRecordHW::prcap( SDeviceItem item )
{
    delete p->prcap_process;
    p->prcap_process = new QProcess( this );
    connect( p->prcap_process , SIGNAL(finished(int)) , this , SLOT(prcap_prev()) );

    p->prcap_process->start( p->command , QStringList()<< "-prcap" << "dev="+item.toQString() );
}

void CdRecordHW::prcap_prev()
{
    QString str = p->prcap_process->readAll();
    QStringList str_list = str.split( "\n" , QString::SkipEmptyParts );

    for( int i=0 ; i<str_list.count() ; i++ )
    {
        QRegExp reg;
        int pos;
        bool ok;

        reg.setPattern( "\\s+Number of supported write speeds\\:\\s+(\\d+)" );
        pos = reg.indexIn( str_list.at(i) );
        if( pos > -1 )
        {
            emit this->deviceSpeedsDetected( this , detectSpeedFromString( str_list.mid(i+1 , reg.cap(1).toInt(&ok)) ) );
            break;
        }
    }

    emit this->prcapFinished( this , str );
}

QList<int> CdRecordHW::detectSpeedFromString( const QStringList & list )
{
    QList<int> CD;
    QList<int> DVD;
    QList<int> BD;
    QRegExp reg;
    reg.setPattern( "\\s*Write\\sspeed\\s\\#\\s\\d+\\:\\s*\\d+\\s+kB\\/s\\s+CLV\\/PCAV\\s+\\(CD\\s*(\\d+)x\\,\\s*DVD\\s*(\\d+)x(\\,\\s*BD\\s*)?(\\d+)?(\\x)?\\)\\s*" );
    //reg.setPattern( "\\s*Write\\sspeed\\s\\#\\s\\d+\\:\\s*\\d+\\s+kB\\/s\\s+CLV\\/PCAV\\s+\\(CD\\s*(\\d+)x\\,\\s*DVD\\s*(\\d+)x\\)\\s*" );

    for( int i=0 ; i<list.count() ; i++ )
    {
        int pos;
        bool ok;

        pos = reg.indexIn( list.at(i) );
        if( pos > -1 )
        {
            CD << reg.cap(1).toInt(&ok);
            DVD << reg.cap(2).toInt(&ok);
            BD << reg.cap(4).toInt(&ok);
        }
    }

    bool ok;
    int pointer;

    ok = true;
    pointer = 0;
    for( int i=0 ; i<CD.count() ; i++ )
        if( CD.at(i) % 2 != 0 || ( CD.at(i) == CD.at(i-(i>=1)) && i > 0 ) )
        {
            ok = false;
            break;
        }

    if( ok )
        return CD;

    ok = true;
    pointer = 0;
    for( int i=0 ; i<DVD.count() ; i++ )
        if( DVD.at(i) % 2 != 0 || ( CD.at(i) == CD.at(i-(i>=1)) && i > 0 ) )
        {
            ok = false;
            break;
        }

    if( ok )
        return DVD;

    ok = true;
    pointer = 0;
    for( int i=0 ; i<BD.count() ; i++ )
        if( BD.at(i) % 2 != 0 || ( CD.at(i) == CD.at(i-(i>=1)) && i > 0 ) )
        {
            ok = false;
            break;
        }

    if( ok )
        return BD;


    return QList<int>();
}

const QList<SDeviceItem> & CdRecordHW::devicesList() const
{
    return p->devices;
}

CdRecordHW::~CdRecordHW()
{
    delete p;
}

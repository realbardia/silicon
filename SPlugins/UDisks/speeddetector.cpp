#include "speeddetector.h"

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


class SpeedDetectorPrivate
{
public:
    QProcess *prcap_process;
    QString command;
};

SpeedDetector::SpeedDetector( QObject *parent ) : QObject(parent)
{
    p = new SpeedDetectorPrivate;
        p->prcap_process = 0;

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

void SpeedDetector::prcap( const SDeviceItem & item )
{
    if( p->prcap_process != 0 )
        delete p->prcap_process;

    p->prcap_process = new QProcess( this );

    connect( p->prcap_process , SIGNAL(finished(int)) , this , SLOT(prcap_prev()) );

    p->prcap_process->start( p->command , QStringList()<< "-prcap" << "dev="+item.toQString() );
}

void SpeedDetector::prcap_prev()
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
            emit this->deviceSpeedsDetected( this->detectSpeedFromString( str_list.mid(i+1 , reg.cap(1).toInt(&ok)) ) );
            break;
        }
    }

    emit this->prcapFinished( str );
    emit this->prcapFinished( this , str );
}

QList<int> SpeedDetector::detectSpeedFromString( const QStringList & list )
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


SpeedDetector::~SpeedDetector()
{
    delete p;
}

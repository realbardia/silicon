#include "sbuffer.h"

#include <QFile>
#include <QRegExp>
#include <QTextStream>
#include <QDebug>

SBuffer::SBuffer()
{
    this->opened=false;
}

SBuffer::SBuffer(const QString & path)
{
    this->opened=false;
    this->setInput(path);
    this->setOutput(path);
}

SBuffer::SBuffer(const QString & iPath,const QString &oPath)
{
    this->opened=false;
    this->setInput(iPath);
    this->setOutput(oPath);
}

void SBuffer::open()
{
    this->openFrom(this->iPath);
}

void SBuffer::openFrom(const QString & path)
{
    this->setInput(path);

    QFile input(this->iPath);
    QTextStream iStream(&input);

    if(input.open(QIODevice::ReadOnly))
    {
        this->buffer=iStream.readAll().split(QRegExp("\\n"));
        this->resetLine();
    }
    else
        qCritical() << QString("SBuffer::open(const QString & path) : Can't Open \"%1\" as ReadOnly").arg( iPath );

    input.close();
    this->opened=true;
}

void SBuffer::setInput(const QString & path)
{
    if(!QFile::exists(path))
        qCritical() << QString("SBuffer::setInput(const QString & path) : \"%1\" No such file").arg( iPath );

    this->iPath=path;
}

bool SBuffer::isOpen()
{
    return this->opened;
}

void SBuffer::setBuffer(const QList<QString> & buffer)
{
    this->buffer=buffer;
}

void SBuffer::save()
{
    this->saveTo(this->oPath);
}

void SBuffer::saveTo(const QString & path)
{
    this->setOutput(path);

    QFile output(this->oPath);
    QTextStream oStream(&output);

    if(output.open(QFile::ReadWrite | QFile::Truncate))
    {
        for(int i=0;i<this->buffer.count()-1;i++)
            oStream<<this->buffer.at(i)<<"\n";

        if( this->buffer.count()-1 >= 0 )
            oStream<<this->buffer.at(this->buffer.count()-1);
    }
    else
        qCritical() << QString("SBuffer::save(const QString & path) : Can't Open \"%1\" as ReadWrite").arg( oPath );

    output.close();
}

void SBuffer::setOutput(const QString & path)
{
    if(!QFile::exists(path))
        qCritical() << QString("SBuffer::setOutput(const QString & path) : \"%1\" No such file").arg( path );

    oPath=path;
}

QString SBuffer::readLine()
{
    if(this->atEnd())
        return NULL;

    this->currentLine++;
    return this->buffer.at(this->currentLine-1);
}

QString SBuffer::readLine(const unsigned int & i)
{
    return this->buffer.at(i);
}

void SBuffer::resetLine()
{
    this->currentLine=0;
}

bool SBuffer::atEnd()
{
    return (this->currentLine>=this->buffer.count());
}

void SBuffer::writeSpace()
{
    this->writeLine("");
}

void SBuffer::writeSpace(const unsigned int & i)
{
    this->writeLine(i,"");
}

void SBuffer::writeLine(const QString & str)
{
    this->buffer<<str;
}

void SBuffer::writeLine(const unsigned int & i,const QString & str)
{
    this->buffer.insert(i,str);
}

void SBuffer::replace(const unsigned int & i,const QString & str)
{
    this->buffer.replace(i,str);
}

void SBuffer::remove(const unsigned int & i)
{
    this->buffer.removeAt(i);
}

void SBuffer::removeArea(const unsigned int & i,const unsigned int & len)
{
    for(unsigned int j=0;j<len;j++)
        buffer.removeAt(i);
}

int SBuffer::count()
{
    return this->buffer.count();
}

void SBuffer::operator <<(const QString & str)
{
    this->writeLine(str);
}

SBuffer::~SBuffer()
{
    this->save();
}

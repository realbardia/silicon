#include "sdatabase.h"

#include <QTextStream>
#include <QFile>
#include <QDir>

SDataBase::SDataBase()
{
    this->setReadOnly(false);
}

SDataBase::SDataBase(const QString & path)
{
    this->setReadOnly(false);
    this->setDataBase(path);
}

void SDataBase::open()
{
    buffer.open();
}

int SDataBase::findHead(QString head)
{
    head="["+head+"]";

    for(int i=0;i<buffer.count();i++)
        if(buffer.readLine(i)==head)
            return i;

    return -1;
}

int SDataBase::findChild(QString head,QString child)
{
    child=child+"=";
    head="["+head+"]";

    for(int i=0;i<buffer.count();i++)
        if(buffer.readLine(i)==head)
        {
            for(int j=i+1;j<buffer.count() && buffer.readLine(j)[0]!='[';j++)
                if(buffer.readLine(j).left(child.size())==child)
                    return j;
        }

    return -1;
}

void SDataBase::setDataBase(const QString & path)
{
    this->databasePath=path;
    buffer.setInput(this->databasePath);
    buffer.setOutput(this->databasePath);
    this->open();
}

void SDataBase::setBuffer(const QList<QString> & buffer)
{
    this->buffer.setBuffer(buffer);
}

bool SDataBase::addHead(const QString & Head)
{
    if(this->databasePath.isEmpty())
        return false;

    if(this->checkHeadExist(Head))
        return false;

    QString str;
    str=str+"["+Head+"]";
    buffer.writeSpace();
    buffer.writeLine(str);

    return true;
}

bool SDataBase::addChild(const QString & head,const QString & child)
{
    if(this->databasePath.isEmpty())
        return false;

    int headRow=this->findHead(head);
    if(headRow==-1)
        return false;

    if(this->checkChildExist(head,child))
        return false;

    buffer.writeLine(this->findHead(head)+1,child+"=");
    return true;
}

bool SDataBase::checkHeadExist(QString item)
{
    item="["+item+"]";

    for(int i=0;i<buffer.count();i++)
        if(buffer.readLine(i)==item)
            return true;

    return false;
}

bool SDataBase::checkChildExist(QString head,QString child)
{
    child=child+"=";
    head="["+head+"]";

    for(int i=0;i<buffer.count();i++)
        if(buffer.readLine(i)==head)
        {
            for(int j=i+1;j<buffer.count() && buffer.readLine(j)[0]!='[';j++)
                if(buffer.readLine(j).left(child.size())==child)
                    return true;
        }

    return false;
}

QString SDataBase::read(const QString & head,const QString & child)
{
    int row= findChild(head,child);
    if(row==-1)
        return NULL;
    else
    {
        QString str=buffer.readLine(row);
        return str.right(str.size()-child.size()-1);
    }
}

void SDataBase::set(const QString & head,const QString & child,const QString & newStr)
{
    int row=this->findChild(head,child);
    if(row==-1)
        return ;

    buffer.replace(row,child+"="+newStr);
}

void SDataBase::remove(const QString & head)
{
    int row=this->findHead(head);
    if(row==-1)
        return ;

    row++;
    int len=0;
    while(len+row<buffer.count() && buffer.readLine(len+row)[0]!='[')
        len++;

    buffer.removeArea(row-1,len+1);
    if(row-1==buffer.count())
        buffer.remove(row-2);
}

void SDataBase::remove(const QString & head,const QString & child)
{
    int row=this->findChild(head,child);
    if(row==-1)
        return ;
    
    buffer.remove(row);
}

void SDataBase::rename(const QString & head,const QString & name)
{
    int row=this->findHead(head);
    if(row==-1)
        return ;

    QString str;
        str=str+"["+name+"]";

    buffer.replace(row,str);
}

void SDataBase::rename(const QString & head,const QString & child,const QString & name)
{
    int row=this->findChild(head,child);
    if(row==-1)
        return ;

    QString str;
        str=buffer.readLine(row);
        str=name+str.right(str.size()-child.size());

    buffer.replace(row,str);
}

QString SDataBase::head(const unsigned int & num)
{
    unsigned int count=0;
    int row;

    for(row=0;row<buffer.count() && count<num+1;row++)
        if(buffer.readLine(row)[0]=='[')
            count++;

    if(row==buffer.count() && count<num)
        return NULL;

    QString str;
        str=buffer.readLine(row-1);
        str=str.right(str.size()-1);
        str=str.left(str.size()-1);
    return str;
}

QString SDataBase::child(const QString & head,const unsigned int & num)
{
    int row=this->findHead(head);
    if(row==-1)
        return NULL;

    unsigned int count=0;

    for(row++;count<num;row++,count++)
        if(row<buffer.count() && buffer.readLine(row)[0]=='[')
            return NULL;

    QString str;
        str=buffer.readLine(row);
    while(str[0]!='=')
        str.right(str.size()-1);
    str.right(str.size()-1);

    return str;
}

bool SDataBase::isChild(const QString & str)
{
    bool firstCheck=false,
         secondCheck=false;

    if(str[0]!='[')
        firstCheck=true;

    for(int i=0;i<str.size() && !secondCheck;i++)
        if(str[i]=='=')
            secondCheck=true;

    if(firstCheck && secondCheck)
        return true;
    else
        return false;
}

unsigned int SDataBase::headsNumber()
{
    int count=0;
    for(int i=0;i<buffer.count();i++)
        if(buffer.readLine(i)[0]=='[')
            count++;

    return count;
}

unsigned int SDataBase::childsNumber(QString head)
{
    int row=this->findHead(head);
    if(row==-1)
        return (unsigned int)0;

    unsigned int count=0;

    for(row++;row<buffer.count() && buffer.readLine(row)[0]!='[';row++)
        count++;

    if(row<buffer.count() && buffer.readLine(row)[0]=='[')
        count--;

    return count;
}

void SDataBase::reset()
{
    buffer.open();
}

void SDataBase::save()
{
    if(!buffer.isOpen() || this->readOnly)
        return ;

    buffer.save();
}

void SDataBase::setReadOnly(bool stt)
{
    this->readOnly=stt;
}

void SDataBase::setReadWrite(bool stt)
{
    this->readOnly=!stt;
}

bool SDataBase::isReadOnly()
{
    return this->readOnly;
}

bool SDataBase::isReadWrite()
{
    return !this->readOnly;
}

void SDataBase::findItemsNumber()
{
    this->itemsNumber=this->headsNumber();
}

SDataBase::~SDataBase()
{
    this->save();
}

#include "sdynamicstorage.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

SDynamicStorage::SDynamicStorage(QWidget *parent)
{
    this->setParent(parent);
}

int SDynamicStorage::registerChest(const QString & file,const QString & pass)
{
    int index=this->getAddress();

    list()<<new SDChest(index,file,pass);
    this->update(index);

    return index;
}

int SDynamicStorage::registerChest(const QString & pass)
{
    int index=this->getAddress();

    list()<<new SDChest(index,pass);

    return index;
}

void SDynamicStorage::unRegisterChest(int chestNumber,const QString & pass)
{
    int index=this->chestIndex(chestNumber);
    if(!this->checkPassword(index,pass))
        return ;

    delete list().at(index);
    list().removeAt(index);
    return ;
}

void SDynamicStorage::updateAll()
{
    for(int i=0;i<list().count();i++)
        this->update(list().at(i)->chestNumber);
}

void SDynamicStorage::update(int chestNumber)
{
    int index=this->chestIndex(chestNumber);
    if(list().at(index)->fileAddress.isEmpty())
        return ;

    QFile file(list().at(index)->fileAddress);
    QTextStream iStream(&file);

    if(file.open(QIODevice::ReadOnly))
        list().at(index)->data=iStream.readAll();
    else
        qCritical() << QString("SDynamicStorage::update(int chestNumber) : Can't Open \"%1\" as ReadOnly").arg( file.fileName() );

    file.close();
}

QString SDynamicStorage::chestData(int chestNumber,const QString & pass)
{
    int index=this->chestIndex(chestNumber);
    if(!this->checkPassword(index,pass))
        return NULL;

    return list().at(index)->data;
}

int SDynamicStorage::chestIndex(int chestNumber)
{
    for(int i=0;i<list().count();i++)
        if(list().at(i)->chestNumber==chestNumber)
            return i;

    return -1;
}

int SDynamicStorage::getAddress()
{
    bool found;
    for(int i=0;i<=list().count();i++)
    {
        found=false;
        for(int j=0;j<list().count() && !found;j++)
            if(list().at(j)->chestNumber==i)
                found=true;

        if(!found)
            return i;
    }

    return -1;
}

bool SDynamicStorage::checkPassword(int chestIndex,const QString & pass)
{
    if(list().at(chestIndex)->password==pass)
        return true;

    return false;
}

QList<SDChest *> &SDynamicStorage::list()
{
    static QList<SDChest *> list;
    return list;
}

SDynamicStorage::~SDynamicStorage()
{
}

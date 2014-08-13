#include "sstaticstorage.h"

#include <QDebug>

SStaticStorage::SStaticStorage()
{
}

void SStaticStorage::registerChest(const QString & name,const QString & data)
{
    int index=this->findChest(name);
    if(index!=-1)
        qCritical() << QString("SStaticStorage::registerChest(const QString & name,const QString & data) : Chest %1 Already Exist").arg(name);

    list()<<new SSChest(name);
    list().last()->data=data;
}

void SStaticStorage::unregisterChest(const QString & name)
{
    int index=this->findChest(name);
    if(index==-1)
    {
        qCritical() << QString("SStaticStorage::registerChest(const QString & name,const QString & data) : Chest %1 don't Exist").arg(name);
        return ;
    }


    delete list().at(index);
}

void SStaticStorage::setData(const QString & name,const QString & data)
{
    int index=this->findChest(name);
    if(index==-1)
    {
        qCritical() << QString("SStaticStorage::registerChest(const QString & name,const QString & data) : Chest %1 don't Exist").arg(name);
        return ;
    }

    list().at(index)->data=data;
}

int SStaticStorage::findChest(const QString & name)
{
    for(int i=0;i<list().count();i++)
        if(list().at(i)->name==name)
            return i;

    return -1;
}

QList<SSChest *> &SStaticStorage::list()
{
    static QList<SSChest *> list;
    return list;
}

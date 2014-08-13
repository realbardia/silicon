#ifndef SSTATICSTORAGE_H
#define SSTATICSTORAGE_H

#include <QString>
#include <QList>

#include "sdatabase_global.h"

struct SSChest
{
    SSChest(const QString & name){ this->name=name; }

    QString name;
    QString data;
};

class SDATABASESHARED_EXPORT SStaticStorage
{
public:
    SStaticStorage();

    void registerChest(const QString & name,const QString & data);
    void unregisterChest(const QString & name);
    void setData(const QString & name,const QString & data);

protected:
    int findChest(const QString & name);
    QList<SSChest *> &list();
};

#endif // SSTATICSTORAGE_H

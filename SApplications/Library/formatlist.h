#ifndef FORMATLIST_H
#define FORMATLIST_H

#include <QStringList>
#include <QString>
#include <QObject>
#include <QWidget>

#include <sfilterlist.h>

class FormatList : public SFilterList
{
    Q_OBJECT
public:
    FormatList(QWidget *parent = 0);

protected:
    QStringList filtersNames( const QString & path );
};

#endif // FORMATLIST_H

#ifndef SBUFFER_H
#define SBUFFER_H

#include <QString>
#include <QStringList>
#include <QList>

#include "sdatabase_global.h"

class SDATABASESHARED_EXPORT SBuffer
{
public:
    SBuffer();
    SBuffer(const QString & path);
    SBuffer(const QString & iPath,const QString &oPath);
    ~SBuffer();

    void open();
    void openFrom(const QString & path="");
    void setInput(const QString & path);
    bool isOpen();

    void setBuffer(const QList<QString> & buffer);

    void save();
    void saveTo(const QString & path="");
    void setOutput(const QString & path);

    QString readLine();
    QString readLine(const unsigned int & i);
    void resetLine();
    bool atEnd();

    void writeSpace();
    void writeSpace(const unsigned int & i);

    void writeLine(const QString & str);
    void writeLine(const unsigned int & i,const QString & str);

    void replace(const unsigned int & i,const QString & str);
    void remove(const unsigned int & i);
    void removeArea(const unsigned int & i,const unsigned int & len);
    int count();

    void operator<<(const QString & str);

private:
    QList<QString> buffer;

    QString iPath;
    QString oPath;

    int currentLine;
    bool opened;
};

#endif // SBUFFER_H

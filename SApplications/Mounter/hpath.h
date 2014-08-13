#ifndef HPATH_H
#define HPATH_H

#include <QApplication>

class Hpath
{
public:
    Hpath();
    Hpath(const QString & path);

    void setPath(const QString & path);

    void setFixPath();
    void setFullpath();
    void setFullAct();
    void setFixCotation();
    void setFindName();
    void setRemoveFirstSlash();
    void setRemoveDotIso();
    void setSystem();
    void setWrap(const int & size);

    bool error();

    QString path();
    QString fullPath();
private:
    QString filePath;
};

#endif // HPATH_H

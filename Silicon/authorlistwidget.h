#ifndef AUTHORLISTWIDGET_H
#define AUTHORLISTWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QIcon>
#include <QString>
#include <QList>
#include <QSize>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include <sauthorinfo.h>

#include "authorwidget.h"

class AuthorListWidget : public QWidget
{
    Q_OBJECT
public:
    AuthorListWidget(QWidget *parent = 0);

    void setAuthors( const QList<SAuthorInfo> & author_list );
    void addAuthors( const QList<SAuthorInfo> & author_list );
    void addAuthor( const SAuthorInfo & author );

    void clear();

    int count();
    int isEmpty();

    const SAuthorInfo & at( int index );

    void setIconSize( const QSize & size );

signals:

public slots:
    void setCurrentAuthor( int index );

private:
    QHBoxLayout *base_layout;

    QListWidget *author_list_widget;
    AuthorWidget *author_widget;

    QList<SAuthorInfo> list;
};

#endif // AUTHORLISTWIDGET_H

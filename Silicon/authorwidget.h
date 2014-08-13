#ifndef AUTHORWIDGET_H
#define AUTHORWIDGET_H

#include <QWidget>
#include <QToolBar>
#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <sauthorinfo.h>
#include <spicturewidget.h>

#include "ui_authorwidget.h"

class AuthorWidget : public QWidget
{
    Q_OBJECT
public:
    AuthorWidget(QWidget *parent = 0);

    void setAuthor( const SAuthorInfo & author );
    const SAuthorInfo & currentAuthor();

    void clear();

signals:

public slots:
    void setBasic();
    void setContact();
    void setAbout();

private:
    QHBoxLayout *base_layout;

    QAction *basic_info_action;
    QAction *contact_action;
    QAction *about_action;
    QToolBar *top_cat_widget;

    SPictureWidget *avatar;

    Ui::AuthorWidget *ui;
    QWidget *ui_widget;

    SAuthorInfo current_author;
};

#endif // AUTHORWIDGET_H

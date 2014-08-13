#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QList>
#include <QSpacerItem>
#include <QHash>

#include <saboutdata.h>

#include "appinfowidget.h"

class About : public QWidget
{
    Q_OBJECT
public:
    About(QWidget *parent = 0);

    void addListAbout( const QList<SAboutData> & l );
    void setAboutList( const QList<SAboutData> & l );

signals:

public slots:
    void showItem( QListWidgetItem *item );
    void showFirstPage();

protected:
    void resizeEvent( QResizeEvent *event );

private:
    QHBoxLayout *base_layout;
    QStackedWidget *stacked_widget;

    QListWidget *list_widget;

    QWidget *about_widget;
    QVBoxLayout *about_layout;
    QHBoxLayout *about_panel_layout;
    QPushButton *back_button;

    AppInfoWidget *app_info_widget;

    QList<SAboutData> list;
    QHash<QListWidgetItem*,SAboutData> hash;
};

#endif // ABOUT_H

#ifndef APPINFOWIDGET_H
#define APPINFOWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QTreeWidgetItem>

#include <saboutdata.h>
#include <spicturewidget.h>

#include "authorlistwidget.h"
#include "ui_appinfowidget.h"

class AppInfoWidget : public QWidget
{
    Q_OBJECT
public:
    AppInfoWidget(QWidget *parent = 0);

    void setAboutData( const SAboutData & about );
    const SAboutData & currentAboutData();

    void setBinaryDepends( const QStringList & list );
    void setSAppDepends( const QStringList & list );

public slots:
    void setInfo();
    void setAbout();
    void setDetails();
    void setAuthors();
    void setContact();
    void setLicense();
    void setTranslate();

private:
    Ui::AppInfoWidget *ui;
    QWidget *info_widget;

    QAction *info_action;
    QAction *about_action;
    QAction *details_action;
    QAction *authors_action;
    QAction *contact_action;
    QAction *license_action;
    QAction *translate_action;
    QToolBar *top_panel_widget;

    QVBoxLayout *base_layout;

    AuthorListWidget *author_widget;
    SPictureWidget *app_picture_widget;

    SAboutData current;

    void clearItem( QTreeWidgetItem *item );
    void addChilds(  QTreeWidgetItem *item , const QStringList & list );
};

#endif // APPINFOWIDGET_H

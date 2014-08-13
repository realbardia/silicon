#ifndef CONFIGURE_H
#define CONFIGURE_H

#include <QWidget>
#include <QObject>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QIcon>
#include <QString>
#include <QSize>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDir>
#include <QHash>
#include <QTranslator>

#include <SOptionBox>

#include "ui_appearanceUI.h"
#include "sdefaults.h"
#include "perconf.h"
#include "scolorbutton.h"

class Configure : public QWidget
{
    Q_OBJECT
public:
    Configure( perConf *conf , QWidget *parent = 0);
    ~Configure();

    void addItem(const QIcon & icon, const QString & name, QWidget *widget);

public slots:
    void setTheme(QListWidgetItem *item);

    void setMenuAnimation(bool stt);
    void setSearchBarAnimation(bool stt);
    void setSearchingAnimation(bool stt);
    void setTabBarAnimation(bool stt);
    void setToolBarAnimation(bool stt);

    void setRaster( bool stt );
    void setNative( bool stt );
    void setOpenGl( bool stt );
    void setAuto( bool stt );

    void setGradiantBack( bool stt );
    void setColorizedMenu( bool stt );

private slots:
    void languageIndexChanged( int index );
    void colorChanged( SColorButton *button );
    void setColors( int type , const QString & color );
    void widgetStyleChanged( const QString & str );
    void close_slot();

signals:
    void themeChanged();
    void animationChenged();
    void graphicEffectsChanged();
    void closed();

private:
    void setGraphicSystem( const QString & str );
    void setupItems();
    bool stringToBoolian( const QString & str );
    QString boolianToString( bool stt );

    void setupAppearance();
    void setupStyle();
    void setupColors();
    void setupLanguages();

    void resetRequest();
    void setLanguage( int id );


protected:
    void changeEvent( QEvent *e ); //language Supporting by Milad Kakoli(Kitten)

private:
    SOptionBox *optionBox;

    QHash<int,SColorButton *> colors_hash;
    QHash<int,QPalette::ColorRole> colors_role;

    QVBoxLayout *layout;
    QDialogButtonBox *buttonBox;

    Ui::Appearance *appearUI;

    SDefaults defaults;
    perConf *prc;

    QPalette default_palette;

    QTranslator *translator;  //language Supporting by Milad Kakoli(Kitten)
};

#endif // CONFIGURE_H

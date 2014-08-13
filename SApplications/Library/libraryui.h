#ifndef LIBRARYUI_H
#define LIBRARYUI_H

#include <QWidget>
#include <QObject>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QPoint>

#include <SApplication>
#include <SPage>
#include <SIsoInfo>
#include <SAbstractImageMounter>

#include "charlist.h"
#include "formatlist.h"
#include "tagslist.h"
#include "librarydb.h"
#include "itemlist.h"
#include "itempreview.h"

class LibraryUI : public SPage
{
    Q_OBJECT
public:
    LibraryUI( const QString & name , SApplication *parent );
    ~LibraryUI();

    void setupActions();

signals:

public slots:
    void reload();
    void addToLibrary();
    void addToLibrary( const QStringList & paths );
    void addToLibrary( const QString & path , const QString & name , const QStringList & tags = QStringList() );

    void addRuntimeArgs( const QVariantList & args );

private slots:
    void itemChanged( int row );
    void showMenu( const QPoint & point );

    void burnCurrentItem();
    void mountCurrentItem();
    void deleteCurrentItem();
    void renameCurrentItem();
    void editCurrentItemTags();

protected:
    void dropEvent( QDropEvent *event );
    void dragEnterEvent( QDragEnterEvent *event );


private:
    QHBoxLayout *lists_layout;
    QVBoxLayout *base_layout;

    QHBoxLayout *filters_layout;
    QVBoxLayout *list_widget_layout;

    QGroupBox *filters_box;
    QGroupBox *list_widget_box;

    QToolBar *toolbar;
    QAction *add_to_library_action;
    QAction *reload_action;

    QMenu *menu;
    QAction *burn_act;
    QAction *mount_act;
    QAction *delete_act;
    QAction *edit_tags_act;
    QAction *rename_act;

    Librarydb *library;

    FormatList *format_list;
    CharList *char_list;
    TagsList *tags_list;
    ItemList *item_list;
    ItemPreview *item_preview;

    SIsoInfo *isoinfo;
    SAbstractImageMounter *mounter;
};

#endif // LIBRARYUI_H

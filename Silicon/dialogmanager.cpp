#include "dialogmanager.h"

#include <QDialog>
#include <SPage>
#include <QStack>
#include <QHash>

#include <SDialogMethod>
#include <SDialog>

class DialogManagerPrivate
{
public:
    SDialogMethod *method;
    QObject *methods_parent;

    QHash<SPage*,QStack<SDialog*> > dialogs;
    QHash<SDialog*,SPage*> index;

    SPage *current_page;
};

DialogManager::DialogManager( QWidget *parent , QObject *methods_parent) :
    QObject(parent)
{
    p = new DialogManagerPrivate;
        p->methods_parent = methods_parent;

    setMethod( 0 );
}

void DialogManager::setMethod( SDialogMethod *method )
{
    if( method == 0 )
        method = new SDialogMethod();

    p->method = method;
    p->method->setParent( p->methods_parent );

    connect( method , SIGNAL(destroyed()) , SLOT(setMethod()) );
}

void DialogManager::insert( SDialog *dialog )
{
    if( p->index.contains(dialog) )
        return ;

    SPage *page = dialog->pageParent();
    if( !p->dialogs.contains(page) )
        p->dialogs.insert( page , QStack<SDialog*>() );

    p->index.insert( dialog , page );
    p->dialogs[page].push( dialog );

    connect( dialog , SIGNAL(destroyed(SDialog*)) , SLOT(remove(SDialog*)) );

    p->method->insert( dialog );

    if( dialog->pageParent() == p->current_page || dialog->pageParent()->pageType() == SPage::WindowedPage )
        p->method->currentDialogChanged( dialog );
}

void DialogManager::remove( SDialog *dialog )
{
    if( !p->index.contains(dialog) )
        return ;

    SPage *page = dialog->pageParent();
    QStack<SDialog*> & stack = p->dialogs[ page ];

    stack.remove( stack.indexOf(dialog) );
    p->index.remove( dialog );

    if( stack.isEmpty() )
        p->dialogs.remove( page );

    p->method->remove( dialog );

    if( p->current_page != dialog->pageParent() )
        return ;

    SDialog *current_new_dialog = 0;
    if( !stack.isEmpty() )
        current_new_dialog = stack.top();

    p->method->currentDialogChanged( current_new_dialog );
}

void DialogManager::pageTypeChanged( SPage::PageType type )
{
    p->method->pageTypeChanged( type );
}

void DialogManager::remove( SPage *page )
{
    if( !p->dialogs.contains(page) )
        return ;

    p->method->remove( page );
}

void DialogManager::currentTabChanged( SPage *page )
{
    SDialog *dialog = 0;
    p->current_page = page;

    if( page != 0 )
        dialog = p->index.key( page );

    p->method->currentTabChanged( page );
    p->method->currentDialogChanged( dialog );
}

void DialogManager::dialogVisibilityChanged( SDialog *dialog , bool stt )
{
    if( dialog->isVisible() == stt )
        return;

    p->method->dialogVisibilityChanged( dialog , stt );

    if( !dialogIsOnTop(dialog) )
        return ;

    if( stt )
    {
        p->method->currentDialogChanged( dialog );
    }
    else
    {
        SDialog *d = 0;
        QStack<SDialog*> & stack = p->dialogs[ dialog->pageParent() ];
        if( stack.count() > 1 )
            d = stack.at( stack.count()-2 );

        p->method->currentDialogChanged( d );
    }
}

bool DialogManager::dialogIsOnTop( SDialog *dialog )
{
    SPage *page = dialog->pageParent();
    if( p->current_page != page )
        return false;

    QStack<SDialog*> & stack = p->dialogs[ page ];
    if( stack.isEmpty() )
        return false;

    SDialog *current_new_dialog = stack.top();
    return dialog == current_new_dialog;
}

DialogManager::~DialogManager()
{
    delete p;
}

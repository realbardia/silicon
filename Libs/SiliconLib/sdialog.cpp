#include "sdialog.h"

#include "silicon.h"

class SDialogPrivate
{
public:
    SPage *page_parent;
    SDialog *dialog_parent;
    QList<SDialog *> dialogs;
};


SDialog::SDialog( SPage *parent , int width , int height ) :
    QWidget(parent)
{
    if( parent == 0 )
    {
        Silicon::showProcessess();
        parent = Silicon::processPage();
        setParent( parent );
    }

    p = 0;
    setPageParent( parent );
    setFixedSize( width , height );
    init();
}

SDialog::SDialog( SDialog *parent , int width , int height ) :
    QWidget(parent)
{
    p = 0;
    setDialogParent( parent );
    setFixedSize( width , height );
    init();
}

SDialog::SDialog( int width , int height ) :
    QWidget( Silicon::processPage() )
{
    Silicon::showProcessess();

    p = 0;
    setPageParent( Silicon::processPage() );
    setFixedSize( width , height );
    init();
}

void SDialog::setPageParent( SPage *parent )
{
    if( p != 0 )
        return;

    p = new SDialogPrivate;
        p->page_parent = parent;
        p->dialog_parent = 0;

    parent->addDialog( this );
}

void SDialog::setDialogParent( SDialog *parent )
{
    if( p != 0 )
        return;

    p = new SDialogPrivate;
        p->page_parent = 0;
        p->dialog_parent = parent;

    parent->addDialog( this );
}

SPage *SDialog::pageParent()
{
    if( parentType() == SDialog::Page )
        return p->page_parent;

    SDialog *dialog = dialogParent();
    while( dialog->parentType() != SDialog::Page )
        dialog = dialog->dialogParent();

    return dialog->pageParent();
}

SDialog *SDialog::dialogParent()
{
    return p->dialog_parent;
}

SDialog::ParentType SDialog::parentType()
{
    if( p->dialog_parent == 0 )
        return SDialog::Page;
    else
        return SDialog::Dialog;
}

void SDialog::addDialog( SDialog *dialog )
{
    if( p->dialogs.contains(dialog) )
        return ;

    p->dialogs << dialog;
    QObject::connect( dialog , SIGNAL(destroyed(SDialog*)) , SLOT(dialogDestroyed(SDialog*)) );
}

const QList<SDialog *> & SDialog::dialogs()
{
    return p->dialogs;
}

void SDialog::exec( QObject *receiver, const char *member )
{
    connect( this , SIGNAL(accepted(bool)) , receiver , member );
}

void SDialog::accept()
{
    setAccept( true );
}

void SDialog::reject()
{
    setAccept( false );
}

void SDialog::setAccept( bool stt )
{
    if( stt )
    {
        emit accepted( true );
        emit accepted();
    }
    else
    {
        emit accepted( false );
        emit rejected();
    }

    close();
}

void SDialog::dialogDestroyed( SDialog *dialog )
{
    p->dialogs.removeOne( dialog );
}

SDialog::~SDialog()
{
    emit destroyed( this );

    while( !p->dialogs.isEmpty() )
        delete p->dialogs.at(0);

    delete p;
}

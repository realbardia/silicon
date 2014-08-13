#include "dmanager.h"
#include "dialogwidget.h"

class DManagerPrivate
{
public:
    DialogWidget *widget;
};

DManager::DManager()
    : SDialogMethod()
{
    p = new DManagerPrivate;
        p->widget = new DialogWidget( static_cast<QWidget*>(parent()) );
}

void DManager::insert( SDialog * )
{
}

void DManager::remove( SDialog * )
{
}

void DManager::remove( SPage * )
{

}

void DManager::pageTypeChanged( SPage::PageType  )
{

}

void DManager::currentTabChanged( SPage * )
{

}

void DManager::currentDialogChanged( SDialog *dialog )
{
    if( dialog != 0 )
        dialog->setWindowFlags( Qt::Widget );

    p->widget->set( dialog );
}

DManager::~DManager()
{
    //delete p->widget;
    delete p;
}

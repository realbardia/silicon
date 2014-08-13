#include "sdialogmethod.h"

SDialogMethod::SDialogMethod() :
    QObject(0)
{
    init();
}

void SDialogMethod::insert( SDialog *dialog )
{
    dialog->setWindowFlags( Qt::Dialog );
    dialog->show();
}

void SDialogMethod::remove(  SDialog * )
{

}

void SDialogMethod::remove( SPage * )
{

}

void SDialogMethod::pageTypeChanged( SPage::PageType )
{

}

void SDialogMethod::currentTabChanged( SPage * )
{

}

void SDialogMethod::currentDialogChanged( SDialog * )
{

}

void SDialogMethod::dialogVisibilityChanged( SDialog * , bool )
{

}

SDialogMethod::~SDialogMethod()
{
    kill();
}

#include "depends.h"
#include "ui_depends.h"

#include <smastericons.h>

Depends::Depends(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Depends)
{
    ui->setupUi(this);
    ui->treeWidget->topLevelItem(0)->setIcon( 0, SMasterIcons::icon( QSize(48,48) , "application-x-executable.png" ) );
}

Depends::~Depends()
{
    delete ui;
}

void Depends::setBinaryDepends( const QStringList & list )
{
    QTreeWidgetItem *item = ui->treeWidget->topLevelItem(0);
    this->clearItem( item );
    this->addChilds( item , list );
}

void Depends::setSAppDepends( const QStringList & list )
{
    QTreeWidgetItem *item = ui->treeWidget->topLevelItem(1);
    this->clearItem( item );
    this->addChilds( item , list );
}

void Depends::addChilds(  QTreeWidgetItem *item , const QStringList & list )
{
    QTreeWidgetItem *temp;

    for( int i=0 ; i<list.count() ; i++ )
    {
        temp = new QTreeWidgetItem( QStringList()<<list.at(i) , QTreeWidgetItem::Type );
        item->addChild( temp );
    }
}

void Depends::clearItem( QTreeWidgetItem *item )
{
    QTreeWidgetItem *temp;

    int cnt = item->childCount();
    for( int i=0 ; i<cnt ; i++ )
    {
        temp = item->child(0);
        item->removeChild( temp );
    }
}

void Depends::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

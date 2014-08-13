#include "authorlistwidget.h"

#include <QListWidgetItem>

AuthorListWidget::AuthorListWidget(QWidget *parent) :
    QWidget(parent)
{
    author_list_widget = new QListWidget();
        author_list_widget->setFixedWidth( 173 );
        author_list_widget->setIconSize( QSize(22,22) );

    author_widget = new AuthorWidget();

    base_layout = new QHBoxLayout( this );
        base_layout->addWidget( author_list_widget );
        base_layout->addWidget( author_widget );

    QObject::connect( author_list_widget  , SIGNAL(currentRowChanged(int)) , this , SLOT(setCurrentAuthor(int)) );
}

void AuthorListWidget::setAuthors( const QList<SAuthorInfo> & author_list )
{
    this->clear();
    this->addAuthors( author_list );
}

void AuthorListWidget::addAuthors( const QList<SAuthorInfo> & author_list )
{
    for( int i=0 ; i<author_list.count() ; i++ )
        this->addAuthor( author_list.at(i) );
}

void AuthorListWidget::addAuthor( const SAuthorInfo & author )
{
    list << author;

    QString name = author.firstName() + " " + author.lastName();
    if( name == " " )
        name = author.nickName();

    QListWidgetItem *item = new QListWidgetItem( author.authorIcon() , name );

    author_list_widget->addItem( item );
}

void AuthorListWidget::clear()
{
    list.clear();
    author_list_widget->clear();
    author_widget->clear();
}

int AuthorListWidget::count()
{
    return list.count();
}

int AuthorListWidget::isEmpty()
{
    return list.isEmpty();
}

const SAuthorInfo & AuthorListWidget::at( int index )
{
    return list.at( index );
}

void AuthorListWidget::setIconSize( const QSize & size )
{
    author_list_widget->setIconSize( size );
}

void AuthorListWidget::setCurrentAuthor( int index )
{
    if( index == -1 )
        return ;

    if( list.count() <= index )
        return ;

    author_list_widget->setCurrentRow( index );
    author_widget->setAuthor( list.at(index) );
}

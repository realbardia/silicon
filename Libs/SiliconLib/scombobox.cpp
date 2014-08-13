#include "scombobox.h"

#include <QAction>
#include <QActionEvent>
#include <QEvent>

class SComboBoxPrivate
{
public:
    int pointer;
    QList<QAction *> list;
};

SComboBox::SComboBox( QWidget *parent ) :
    QComboBox(parent)
{
    p = new SComboBoxPrivate;
    p->pointer = 0;

    connect( this , SIGNAL(currentIndexChanged(int)) , SLOT(indexChanged(int)) );
}

void SComboBox::indexChanged( int index )
{
    if( index < count() - p->list.count() )
        return ;

    index = index - ( count() - p->list.count() );
    p->list[ index ]->trigger();
}

void SComboBox::actionEvent( QActionEvent *event )
{
    QAction *action;
    int tmp;
    QEvent::Type type = event->type();

    disconnect( this , SIGNAL(currentIndexChanged(int)) , this , SLOT(indexChanged(int)) );

    switch( static_cast<int>(type) )
    {
    case QEvent::ActionAdded :
        action = event->action();
        addItem( action->icon() , action->text() , action->data() );
        p->list << action;
        p->pointer++;
        break;

    case QEvent::ActionChanged :
        action = event->action();
        tmp = p->list.indexOf(event->before());
        if( tmp < 0 )
            tmp = p->list.indexOf(event->action());

        setItemText( count() - p->list.count() + tmp , action->text() );
        setItemIcon( count() - p->list.count() + tmp , action->icon() );
        setItemData( count() - p->list.count() + tmp , action->data() );

        p->list.replace( tmp , action );
        break;

    case QEvent::ActionRemoved :
        p->pointer--;
        break;
    }

    connect( this , SIGNAL(currentIndexChanged(int)) , SLOT(indexChanged(int)) );
}

SComboBox::~SComboBox()
{
    delete p;
}

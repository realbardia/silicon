#include "dialogwidget.h"

#include <QDeclarativeView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGraphicsWidget>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QMainWindow>

#include <Silicon>

void resizeObject( QObject *object , int width , int height )
{
    object->setProperty( "width"  , width  );
    object->setProperty( "height" , height );
}

void resizeObject( QObject *object , const QSize & size )
{
    resizeObject( object , size.width() , size.height() );
}


class DialogWidgetPrivate
{
public:
    QDeclarativeView *viewer;
    QHBoxLayout *layout;

    QGraphicsWidget *root;
    QObject *dialog;
    QObject *background;

    QWidget *widget;
};

DialogWidget::DialogWidget(QWidget *parent) :
    QWidget(parent)
{
    p = new DialogWidgetPrivate;
        p->widget = 0;

    p->viewer = new QDeclarativeView();
        p->viewer->setSource(QUrl("qrc:/Plugins/SingleInnerDialogQML/qml/main.qml"));
        p->viewer->setAttribute(Qt::WA_TranslucentBackground);
        p->viewer->setStyleSheet(QString("background: transparent"));

    p->root       = static_cast<QGraphicsWidget*>( p->viewer->rootObject() );
    p->background = p->root;
    p->dialog     = p->root->findChild<QObject *>( QString("dialog") );

    p->layout = new QHBoxLayout( this );
        p->layout->addWidget( p->viewer );
        p->layout->setContentsMargins( 0 , 0 , 0 , 0 );

    connect( Silicon::mainWindow() , SIGNAL(resized(QSize)) , SLOT(main_resized()) );

    main_resized();
    hide();
}

void DialogWidget::showWidget( QWidget *widget )
{
    if( widget == 0 )
    {
        if( p->widget == 0 )
            return;

        p->widget->setParent( 0 );
        p->widget->hide();
        p->widget = 0;

        unexpand();
        return;
    }

    if( p->widget != 0 )
    {
        p->widget->setParent( 0 );
        p->widget->hide();
    }

    p->widget = widget;
    p->widget->setParent( this );
    p->widget->hide();

    update_widget();
    expand();
}

void DialogWidget::update_widget()
{
    if( p->widget == 0 )
        return;

    const QSize & size = p->widget->maximumSize();

    p->widget->move( (width()-size.width())/2 , 35 );

    p->dialog->setProperty( "temp_height" , size.height()+50 );

    if( !expanded() )
    {
        p->dialog->setProperty( "width" , size.width()+25 );
        return;
    }

    resizeObject( p->dialog , size.width()+25 , size.height()+50 );
}

void DialogWidget::main_resized()
{
    const QSize & size = static_cast<QWidget*>(parent())->size();
    move(0,0);
    resize(size);
}

void DialogWidget::unexpand()
{
    QTimer::singleShot( 337 , this , SLOT(hide()) );
    if( p->widget != 0 )
        p->widget->hide();

    p->dialog->setProperty( "state" , "Closed" );
}

void DialogWidget::expand()
{
    show();
    if( p->widget != 0 )
        QTimer::singleShot( 337 , p->widget , SLOT(show()) );

    p->dialog->setProperty( "state" , "Opened" );
}

bool DialogWidget::expanded() const
{
    return p->dialog->property( "state" ) == "Opened";
}

bool DialogWidget::unexpanded() const
{
    return p->dialog->property( "state" ) == "Closed";
}

void DialogWidget::resizeEvent( QResizeEvent *event )
{
    update_widget();
    resizeObject( p->background , event->size() );
    QWidget::resizeEvent( event );
}

DialogWidget::~DialogWidget()
{
    delete p;
}

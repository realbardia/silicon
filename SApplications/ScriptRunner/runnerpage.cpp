#include "runnerpage.h"

#include <SMasterIcons>

#include <QTextEdit>
#include <QToolBar>
#include <QAction>
#include <QHBoxLayout>

class RunnerPagePrivate
{
public:
    QToolBar *toolbar;
    QAction  *start_action;
    QAction  *new_action;
    QAction  *example_action;

    QTextEdit   *text_edit;
    QHBoxLayout *layout;
};

RunnerPage::RunnerPage( SApplication *parent ) :
    SPage( tr("Script Runner") , parent , SPage::WindowedPage )
{
    p = new RunnerPagePrivate;

    p->toolbar = new QToolBar();
        p->toolbar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
        p->toolbar->setIconSize( QSize(22,22) );

    p->text_edit = new QTextEdit();
        p->text_edit->setFontFamily( "monospace" );
        p->text_edit->setText( "# It's under construction and support simple commands currently.\n"
                               "# To see commands details, go to About Page, Select an application and then go to commands page.\n"
                               "\n" );

    p->layout = new QHBoxLayout( this );
        p->layout->addWidget( p->text_edit );

    init_actions();
    setToolBar( p->toolbar );
}

QString RunnerPage::script() const
{
    return p->text_edit->toPlainText();
}

void RunnerPage::init_actions()
{
    p->start_action   = new QAction( SMasterIcons::icon(QSize(48,48),"media-playback-start.png") , tr("Start"   ) , this );
    p->new_action     = new QAction( SMasterIcons::icon(QSize(48,48),"document-new.png")         , tr("New"     ) , this );
    p->example_action = new QAction( SMasterIcons::icon(QSize(48,48),"help-hint.png")            , tr("Example" ) , this );

    p->start_action->setShortcut( Qt::Key_F5 );
    p->new_action->setShortcut(   Qt::CTRL + Qt::Key_N );

    menuPanel()->addAction( p->start_action   );
    menuPanel()->addSeparator();
    menuPanel()->addAction( p->new_action     );
    menuPanel()->addAction( p->example_action );

    p->toolbar->addAction( p->new_action     );
    p->toolbar->addAction( p->example_action );
    p->toolbar->addSeparator();
    p->toolbar->addAction( p->start_action   );

    connect( p->start_action   , SIGNAL(triggered()) , SIGNAL(started()) );
    connect( p->example_action , SIGNAL(triggered()) , SLOT(example())   );
}

void RunnerPage::start_prev()
{

}

void RunnerPage::example()
{
    p->text_edit->setText( "# This Example erase rewritable disc on sr0, copy sr1 disc to hard, and last, burn\n"
                           "# image from hard to erased rewritable disc and mount image.\n\n"
                           "variable1=DiscEraser -force dev=/dev/sr0 -silent\n"
                           "variable2=DiscImaging dev=/dev/sr1 file=/tmp/silicon_tmp_file.iso -silent\n"
                           "wait(variable1)\n"
                           "wait(variable2)\n"
                           "variable2=ImageBurner file=/tmp/silicon_tmp_file.iso dev=/dev/sr0 speed=16 -eject -silent\n"
                           "Mounter /tmp/silicon_tmp_file.iso\n"
                           "wait(variable2)\n");
}

RunnerPage::~RunnerPage()
{
    delete p;
}

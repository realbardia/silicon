#define WAIT_COMMAND QString("wait")

#include "scriptrunner.h"
#include "runnerpage.h"

#include <QHash>
#include <QDebug>
#include <QMessageBox>
#include <QMainWindow>

#include <Silicon>

class ScriptRunnerPrivate
{
public:
    RunnerPage *page;

    QHash<QString,SApplication*> variables;
    QList<SApplication*> runned_apps;

    QStringList current_script;
};

ScriptRunner::ScriptRunner( const SAboutData & parent ) :
    SApplication(parent)
{
    p = new ScriptRunnerPrivate;
    p->page = new RunnerPage( this );

    connect( p->page , SIGNAL(started()) , SLOT(start()) );
}

void ScriptRunner::start()
{
    if( p->page->pageType() == SPage::WindowedPage )
        p->page->hide();
    else
        p->page->setDisabled( true );

    p->current_script = p->page->script().split("\n",QString::SkipEmptyParts);

    start_prev();
}

void ScriptRunner::start_prev()
{
    if( p->current_script.isEmpty() )
    {
        finish();
        return;
    }

    // Define Wait() command =====================================//
    QString command;
    do{
        if( p->current_script.isEmpty() )
        {
            finish();
            return;
        }

        command = p->current_script.takeFirst();
    } while( (!command.isEmpty() && command.at(0) == '#') || command.isEmpty() );

    if( command.left(WAIT_COMMAND.size()+1) == WAIT_COMMAND + "(" && command.right(1) == ")" )
    {
        command.remove(" ");
        command.remove(0,WAIT_COMMAND.size()+1);
        command.remove(command.size()-1,1);

        SApplication *app = p->variables.value(command);
        if( p->runned_apps.contains(app) )
        {
            connect( app , SIGNAL(destroyed()) , SLOT(start_prev()) );
            return;
        }
        else
        {
            start_prev();
            return;
        }
    }


    // Define Commands ==========================================//
    QString variable;

    bool variable_flag = false;
    bool command_flag  = false;

    for( int i=0 ; i<command.size() ; i++ )
    {
        if( command[i] == ' ' && !variable_flag && !command_flag )
            command_flag  = true;
        if( command[i] == '=' && !variable_flag && !command_flag )
        {
            variable_flag = true;
            variable = command.left(i);
        }

        if( !variable_flag && !command_flag && i!=command.size()-1  )
            continue;

        SApplication *app = run( command.mid(variable_flag*(i+1)) );

        if( variable_flag )
            defineVariable( variable , app );
        if( !app )
        {
            int ret = QMessageBox::warning( Silicon::mainWindow() , tr("Command faild") , tr("Faild to run:\n%1\nDo you want to continue?").arg(command) , QMessageBox::Yes|QMessageBox::No , QMessageBox::No );
            switch( ret )
            {
            case QMessageBox::No:
                finish();
                return;
                break;

            case QMessageBox::Yes:
                break;
            }

            break;
        }

        connect( app , SIGNAL(destroyed(SApplication*)) , SLOT(appDestroyed(SApplication*)) );
        p->runned_apps.append( app );
        break;
    }

    start_prev();
}

void ScriptRunner::finish()
{
    p->runned_apps.clear();
    p->variables.clear();

    if( p->page->pageType() == SPage::WindowedPage )
        p->page->show();
    else
        p->page->setEnabled( true );
}

SApplication *ScriptRunner::run( const QString & command )
{
    QString     app_str;

    QVariantList arguments;
    int pointer = 0;
    bool on_dc = false;
    bool dc_exist_flag = false;
    for( int i=0 ; i<command.count() ; i++ )
    {
        QChar ch = command.at(i);
        if( ch == '"' && ( i==0 || command.at(i-1) != '\\') )
            on_dc = !on_dc;

        if( on_dc )
        {
            dc_exist_flag = true;
            continue;
        }

        if( ch != ' ' && i<command.count()-1 )
            continue;

        QString str = command.mid(pointer,i-pointer + (i>=command.count()-1) );
        pointer = i+1;
        if( dc_exist_flag )
            str.remove(0,1).remove(str.size()-2,1);

        if( str.isEmpty() )
            continue;

        if( app_str.isEmpty() )
            app_str = str;
        else
            arguments << str;
    }

    SApplication *app = Silicon::loadApp( app_str , arguments );

    return app;
}

void ScriptRunner::defineVariable( const QString & name , SApplication *value )
{
    if( p->variables.contains(name) )
        p->variables[name] = value;
    else
        p->variables.insert( name , value );
}

void ScriptRunner::appDestroyed( SApplication *app )
{
    p->runned_apps.removeOne( app );
}

ScriptRunner::~ScriptRunner()
{
    delete p;
}

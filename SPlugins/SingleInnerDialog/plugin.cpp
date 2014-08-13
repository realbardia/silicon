#include "plugin.h"
#include "dmanager.h"

SingleInnerDialog::SingleInnerDialog()
    : SPlugin( QIcon(":/plugins/single_inner_dialog/icon.png") ,
          "Single-Inner-Dialog",
          tr("Show Dialogs in Single and Animation Window") )
{
    setConflicts( QStringList() );
    m = 0;
}

bool SingleInnerDialog::startEvent()
{
    if( m != 0 )
        return true;

    m = new DManager();
    return true;
}

bool SingleInnerDialog::stopEvent()
{
    if( m == 0 )
        return true;

    delete m;
    m = 0;

    return true;
}

SingleInnerDialog::~SingleInnerDialog()
{

}

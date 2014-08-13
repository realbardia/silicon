#include "discscanner.h"
#include "mainpage.h"

class DiscScannerPrivate
{
public:
    MainPage *page;
};

DiscScanner::DiscScanner( const SAboutData & parent )
    : SApplication( parent )
{
    p = new DiscScannerPrivate;
    p->page = new MainPage( this );
}

DiscScanner::~DiscScanner()
{
    delete p;
}

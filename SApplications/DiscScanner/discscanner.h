#ifndef DISCSCANNER_H
#define DISCSCANNER_H

#include <SApplication>

class DiscScannerPrivate;
class DiscScanner : public SApplication
{
    Q_OBJECT

public:
    DiscScanner( const SAboutData & parent );
    ~DiscScanner();

private:
    DiscScannerPrivate *p;
};

#endif // DISCSCANNER_H

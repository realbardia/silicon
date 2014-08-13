#ifndef DMANAGER_H
#define DMANAGER_H

#include <SDialogMethod>
#include <SDialog>
#include <SPage>

class DManagerPrivate;
class DManager : public SDialogMethod
{
public:
    DManager();
    ~DManager();

    void insert( SDialog *dialog );

    void remove( SDialog *dialog );
    void remove( SPage *dialog );

    void pageTypeChanged( SPage::PageType type );

    void currentTabChanged( SPage *page );
    void currentDialogChanged( SDialog *dialog );

private:
    DManagerPrivate *p;
};

#endif // DMANAGER_H

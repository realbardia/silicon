#ifndef DIALOGMANAGER_H
#define DIALOGMANAGER_H

#include <QWidget>
#include <QObject>

#include <SPage>

class SDialogMethod;
class SDialog;

class DialogManagerPrivate;
class DialogManager : public QObject
{
    Q_OBJECT
public:
    DialogManager( QWidget *parent = 0 , QObject *methods_parent = 0);
    ~DialogManager();


public slots:
    void setMethod( SDialogMethod *method = 0 );
    void insert( SDialog *dialog );

    void remove( SDialog *dialog );
    void remove( SPage *page );

    void pageTypeChanged( SPage::PageType type );

    void currentTabChanged( SPage *page );
    void dialogVisibilityChanged( SDialog *dialog , bool stt );

private:
    bool dialogIsOnTop( SDialog *dialog );

private:
    DialogManagerPrivate *p;
};

#endif // DIALOGMANAGER_H

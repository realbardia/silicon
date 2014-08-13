#ifndef DISCDETAILS_H
#define DISCDETAILS_H

#include <SPage>
#include <SApplication>

class DiscDetailsPrivate;
class DiscDetails : public SPage
{
    Q_OBJECT
public:
    DiscDetails( SApplication *parent );
    ~DiscDetails();

public slots:
    void addDevice();

private slots:
    void closeRequest( QWidget *widget );

private:
    void init_actions();

private:
    DiscDetailsPrivate *p;
};

#endif // DISCDETAILS_H

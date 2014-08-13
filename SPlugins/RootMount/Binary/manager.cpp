#include "manager.h"
#include <QCoreApplication>

int count = 0;

void Manager::increase()
{
    count++;
}

void Manager::decrease()
{
    count--;
    if( count <= 0 )
        QCoreApplication::quit();
}

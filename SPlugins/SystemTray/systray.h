#ifndef SYSTRAY_H
#define SYSTRAY_H

#include <QSystemTrayIcon>

class SysTrayPrivate;
class SysTray : public QSystemTrayIcon
{
    Q_OBJECT
public:
    SysTray(QObject *parent = 0);
    ~SysTray();
    
signals:
    
public slots:
    void toggleVisiblity();

private slots:
    void activate_changed( QSystemTrayIcon::ActivationReason active );
    
private:
    SysTrayPrivate *p;
};

#endif // SYSTRAY_H

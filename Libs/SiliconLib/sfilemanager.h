#ifndef SFILEMANAGER_H
#define SFILEMANAGER_H

#include <QListWidget>
#include <QDir>

class SFileManager : public QListWidget
{
    Q_OBJECT
public:
    SFileManager(QWidget *parent = 0);

    void goTo( const QString & dir );

signals:

public slots:

private:
    QDir current_dir;
};

#endif // SFILEMANAGER_H

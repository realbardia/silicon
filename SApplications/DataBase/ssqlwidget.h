#ifndef SSQLWIDGET_H
#define SSQLWIDGET_H

#include <QWidget>
#include <STinyFileInfo>
#include <SFileInfo>

class QListWidgetItem;
class SSqlWidgetPrivate;

class SSqlWidget : public QWidget
{
    Q_OBJECT
public:
    SSqlWidget(QWidget *parent = 0);

    void cd( const QString & disc , const QString & dir = QString() );
    const STinyFileInfo & current() const;

    SFileInfoList currentFiles() const;
    SFileInfoList selectedFiles() const;

    void detailsOf( const STinyFileInfo & file );

    void search( const QString & key , SFileInfo::FileCat cat );

public slots:
    void up();

signals:
    void detailsDetected( const SFileInfo & file );
    void directoryChanged( const STinyFileInfo & dir );

private slots:
    void discs_id_detected( const QStringList & list );
    void detected_files_of( const STinyFileInfo & dir , const SFileInfoList & files );

    void item_double_clicked( QListWidgetItem *item );
    void founded( const QString & key , const SFileInfoList & list );

private:
    void init_root_items();

private:
    SSqlWidgetPrivate *p;
};

#endif // SSQLWIDGET_H

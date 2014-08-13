#ifndef SPICTUREWIDGET_PREV_H
#define SPICTUREWIDGET_PREV_H

#include "libsilicon_global.h"

#include <QThread>
#include <QSize>
#include <QImage>

/*! \class
 * The THCoverFlowImageLoader provides a class to read and load
 * picture from file in another thread.
 * This Class is part of MokoTouch and now Edited by
 * Bardia Daneshvar to use in the SPictureWidget class.
 */
class LIBSILICON_EXPORT THCoverFlowImageLoader : public QThread
{

/*
 * This class is part of MokoTouch.
 *
 * MokoTouch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MokoTouch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MokoTouch.  If not, see <http://www.gnu.org/licenses/>.
 */

    Q_OBJECT
public:
    THCoverFlowImageLoader ();
    THCoverFlowImageLoader (const QImage& image);
    THCoverFlowImageLoader (const QString& path);
    ~THCoverFlowImageLoader();

    void setImage( const QImage & image );
    void setPath( const QString & path );

    void setSize( const QSize & size );
    void run (void);

    bool isRunning();

Q_SIGNALS:
    void completed (const QImage& image);

private:
    QString m_path;
    QImage m_image;
    QSize m_size;

    bool run_status;
};

#endif // SPICTUREWIDGET_PREV_H

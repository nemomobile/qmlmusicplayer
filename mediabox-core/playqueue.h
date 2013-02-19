/* This file is part of mediabox-core
 * Copyright (C) 2011 Martin Grimme  <martin.grimme _AT_ gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#ifndef PLAYQUEUE_H
#define PLAYQUEUE_H

#include <QObject>
#include <QList>
#include "contentprovider.h"
#include "file.h"

namespace media
{

/* Class for a play queue with history.
 */
class PlayQueue : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString nowPlaying READ nowPlaying NOTIFY itemChanged);
public:
    explicit PlayQueue(QObject *parent = 0);
    ~PlayQueue();

    void append(content::File::Ptr fptr);
    void replace(content::File::Ptr fptr);
    QString nowPlaying();

    Q_INVOKABLE void clear()
        { queue.clear(); }
    Q_INVOKABLE void appendPath(QString p)
        { append(content::ContentProvider::resolve(p)); }
    Q_INVOKABLE void replacePath(QString p)
        { replace(content::ContentProvider::resolve(p)); }

public slots:
    void previous();
    void next();
    void skip(int n);

private:
    QList<content::File::Ptr> history;
    QList<content::File::Ptr> queue;

signals:
    void itemChanged(content::File::Ptr);
};

}
#endif // PLAYQUEUE_H

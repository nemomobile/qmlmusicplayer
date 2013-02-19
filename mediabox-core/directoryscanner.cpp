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


#include "directoryscanner.h"
#include "mimetype.h"

#include <QDir>
#include <QDirIterator>
#include <QDateTime>
#include <QDebug>

content::DirectoryScanner::DirectoryScanner(QObject *parent)
    : content::AbstractScanner(parent)
{
    connect(&myFsWatcher, SIGNAL(directoryChanged(QString)),
            this, SLOT(handleDirectoryChanged(QString)));
}

void content::DirectoryScanner::setPath(QString path)
{
    if (! myPath.isEmpty())
        myFsWatcher.removePath(myPath);

    myPath = path;
    myFsWatcher.addPath(path);
}

void content::DirectoryScanner::scan()
{
    QDir dir(myPath);
    QDirIterator walker(dir, QDirIterator::Subdirectories);
    while (walker.hasNext())
    {
        QString f = walker.next();

        content::MimeType mimetype(f);
        if (! mimetype.isRecognized()) continue;

        QFileInfo fInfo(f);

        if (! fInfo.isHidden() && fInfo.isFile())
        {
            int mtime =fInfo.lastModified().toTime_t();
            //qDebug() << "discovering" << f;
            emit foundFile(f, mtime);
        }
    }
}

void content::DirectoryScanner::handleDirectoryChanged(const QString path)
{
    qDebug() << "directory changed:" << path;
    scan();
}

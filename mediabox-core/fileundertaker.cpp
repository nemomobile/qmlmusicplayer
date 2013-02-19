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


#include "fileundertaker.h"
#include <QTimer>
#include <QFileInfo>
#include <QDebug>

content::FileUndertaker::FileUndertaker(content::FileIndex *fileIndex)
    : QObject(0)
    , myIndex(fileIndex)
{
}

void content::FileUndertaker::run()
{
    qDebug() << "checking for removed files";
    QString query = "File.Id, File.Path of (all)";
    myFileList = myIndex->query(query);

    //QTimer::singleShot(0, this, SLOT(checkFile()));
    checkFile();
}

void content::FileUndertaker::checkFile()
{
    QList<int> pathids;

    foreach (content::Metadata item, myFileList)
    {
        QString path = QString::fromUtf8(item["File.Path"].toByteArray());
        QFileInfo fInfo(path);

        if (! fInfo.exists())
        {
            pathids << item["File.Id"].toInt();
        }
    }

    myIndex->removeFromIndex(pathids);
}

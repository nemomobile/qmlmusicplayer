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


#include "audioinspector.h"

#include "mediabox-core/datadirectory.h"
#include "mediabox-core/tags.h"

#include <QCryptographicHash>
#include <QFileInfo>
#include <QDir>
#include <QFile>
#include <QDebug>


audio::AudioInspector::AudioInspector(QObject *parent) :
    content::AbstractInspector(parent)
{
}

bool audio::AudioInspector::handlesMimetype(content::MimeType &mimetype)
{
    if (mimetype.matches("audio/*"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void audio::AudioInspector::inspectFile(QString path, content::Metadata &meta)
{
    QFileInfo f(path);

    tags::Tags tags(path);

    QByteArray title;
    QByteArray album;
    QByteArray artist;
    int index = 0;

    if (tags.contains("TITLE"))
        title = tags.get("TITLE").replace('/', '|');
    else
        title = f.completeBaseName().toUtf8();

    if (tags.contains("ALBUM"))
        album = tags.get("ALBUM").replace('/', '|');
    else
        album = f.dir().dirName().toUtf8();

    if (tags.contains("ARTIST"))
        artist = tags.get("ARTIST").replace('/', '|');
    else
        artist = "Unknown Artist";

    if (tags.contains("TRACKNUMBER"))
    {
        QList<QByteArray> parts = tags.get("TRACKNUMBER").split('/');
        index = parts[0].toInt();
    }

    meta["File.Type"] = QVariant(QByteArray("audio"));
    meta["Audio.Title"] = QVariant(title);
    meta["Audio.Album"] = QVariant(album);
    meta["Audio.Artist"] = QVariant(artist);
    meta["Audio.Tracknumber"] = QVariant(index);

    // is there cover art available?
    meta["Audio.CoverFile"] = QVariant(getCover(path, meta, tags).toUtf8());
}

QString audio::AudioInspector::getCover(QString path,
                                        content::Metadata &meta,
                                        tags::Tags &tags)
{
    if (tags.contains("PICTURE") && ! tags.get("PICTURE").isEmpty())
    {
        QByteArray coverKey = meta["Audio.Album"].toByteArray();
        QByteArray md5 = QCryptographicHash::hash(coverKey,
                                                  QCryptographicHash::Md5);
        QFile coverFile(DataDirectory::Covers + "/" + QString(md5.toHex()) + ".jpg");
        if (! coverFile.exists() && coverFile.open(QIODevice::WriteOnly))
        {
            coverFile.write(tags.get("PICTURE"));
            coverFile.close();
        }
        return coverFile.fileName();
    }
    else
    {
        QFileInfo f(path);
        QDir folder = f.dir();
        QStringList filters;
        filters << "*.jpg" << "*.jpeg" << "*.png";
        folder.setNameFilters(filters);

        foreach (QString f, folder.entryList(filters))
        {
            return folder.path() + "/" + f;
        }
    }
    return "";
}

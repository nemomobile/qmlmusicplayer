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


#include "albumsource.h"

#include "fileindex.h"
#include "mimetype.h"

#include <QStringList>
#include <QFileInfo>
#include <QDir>
#include <QtAlgorithms>
#include <QDebug>

static const content::MimeType ALBUM_TYPE("", "application/x-music-folder");

bool albumComparator(const content::Metadata &m1, const content::Metadata &m2)
{
    // hmm, should we convert from UTF8 for comparison, or is it fine this way?
    QByteArray artist1 = m1["Audio.Artist"].toByteArray();
    QByteArray artist2 = m2["Audio.Artist"].toByteArray();
    QByteArray album1 = m1["Audio.Album"].toByteArray();
    QByteArray album2 = m2["Audio.Album"].toByteArray();

    if (artist1 < artist2)
    {
        return false;
    }
    else if (artist1 > artist2)
    {
        return true;
    }
    else
    {
        return album1 > album2;
    }
}

bool trackComparator(const content::Metadata &m1, const content::Metadata &m2)
{
    if (m1.contains("Audio.Tracknumber") && m2.contains("Audio.Tracknumber"))
    {
        return m1["Audio.Tracknumber"].toInt() < m2["Audio.Tracknumber"].toInt();
    }
    else
    {
        return &m1 < &m2;
    }
}

audio::AlbumSource::AlbumSource(content::FileIndex *fileindex, QObject *parent)
    : content::AbstractSource(parent)
    , myFileIndex(fileindex)
{
}

content::File::Ptr audio::AlbumSource::createRoot()
{
    content::File::Ptr fptr(new content::File(content::File::FOLDER));
    fptr->setName("Albums");
    fptr->setPath("/");

    return fptr;
}

content::File::Ptr audio::AlbumSource::createAlbum(QString name, QString cover)
{
    content::File::Ptr fptr(new content::File(content::File::FOLDER));
    fptr->setName(name);
    fptr->setMimetype(ALBUM_TYPE);
    fptr->setPath("/" + name);
    fptr->setImage(cover);

    return fptr;
}

content::File::Ptr audio::AlbumSource::createTrack(QString album,
                                                   QString artist,
                                                   QString track,
                                                   QString resource)
{
    content::File::Ptr fptr(new content::File(content::File::FILE));
    fptr->setName(track);
    fptr->setInfo(artist);
    fptr->setMimetype(content::MimeType(resource));
    fptr->setPath("/" + album + "/" + track);
    fptr->setResource(resource);

    return fptr;
}

content::File::Ptr audio::AlbumSource::resolve(QString path)
{
    content::File::Ptr fptr = myCache[path];
    if (! fptr.isNull())
    {
        return fptr;
    }

    QStringList parts = path.split('/', QString::SkipEmptyParts);
    switch (parts.length())
    {
    case 0:
        return createRoot();

    case 1:
        {
            QString album = parts.at(0);
            return createAlbum(album, "");
        }

    case 2:
        {
            QString album = parts.at(0);
            QString track = parts.at(1);
            QString query = "File.Path of "
                            "((Audio.Album = '%1') and (Audio.Title = '%2'))";
            QList<content::Metadata> result = myFileIndex->query(
                    query.arg(content::FileIndex::quote(album))
                         .arg(content::FileIndex::quote(track))
            );
            QString path = QString::fromUtf8(result.at(0)["File.Path"].toByteArray());
            return createTrack(album, "", track, path);
        }

    default:
        return content::File::Ptr(new content::File);
    }
}

void audio::AlbumSource::list(int cookie, QString path)
{
    myCache.clear();

    if (path == "/")
    {
        // list albums
        QList<content::Metadata> result = myFileIndex->query(
                "Audio.Album, Audio.Artist, Audio.CoverFile of "
                ""
                " (File.Type = 'audio')"
                "");

        // sort the albums by artist/name
        qSort(result.begin(), result.end(), albumComparator);

        foreach (content::Metadata entry, result)
        {
            QString album = QString::fromUtf8(entry["Audio.Album"].toByteArray());
            //QString artist = QString::fromUtf8(entry["Audio.Artist"].toByteArray());
            QString cover = QString::fromUtf8(entry["Audio.CoverFile"].toByteArray());

            // we're not interested in broken albums with emtpy names
            if (album.trimmed().isEmpty())
                continue;

            content::File::Ptr fptr = createAlbum(album, cover);
            myCache["/"] = fptr;
            emit newFile(cookie, fptr);
        }
    }
    else
    {
        // list tracks
        QString album = path.mid(1);
        QString query = "Audio.Title, Audio.Artist, Audio.Tracknumber, "
                        "File.Path of (Audio.Album = '%1')";
        QList<content::Metadata> result = myFileIndex->query(
               query.arg(content::FileIndex::quote(album))
        );

        // sort the result by track number
        qSort(result.begin(), result.end(), trackComparator);

        foreach (content::Metadata entry, result)
        {
            QString title = QString::fromUtf8(entry["Audio.Title"].toByteArray());
            QString artist = QString::fromUtf8(entry["Audio.Artist"].toByteArray());
            QString path = QString::fromUtf8(entry["File.Path"].toByteArray());
            content::MimeType mimetype(path);

            content::File::Ptr fptr = createTrack(album, artist, title, path);
            myCache[path] = fptr;
            emit newFile(cookie, fptr);
        }

    }
    emit newFile(cookie, content::File::Ptr(new content::File));
}

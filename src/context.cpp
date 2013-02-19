/* This file is part of Music Shelf
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


#include "context.h"

#include "mediabox-core/albumsource.h"
#include "mediabox-core/audioinspector.h"
#include "mediabox-core/contentprovider.h"
#include "mediabox-core/datadirectory.h"
#include "mediabox-core/directoryscanner.h"
#include "mediabox-core/fileindex.h"
#include "mediabox-core/fileundertaker.h"
#include "mediabox-core/mimetype.h"

#include <QSettings>
#include <QVariant>
#include <QDir>
#include <QTimer>
#include <QDebug>

static const QString GROUP_GENERAL("General");
static const QString KEY_MUSIC_FOLDER("musicFolder");

Context::Context(QObject *parent)
    : QObject(parent)
    , myContentProvider(new content::ContentProvider)
    , myFileIndex(new content::FileIndex)
    , myDirectoryScanner(new content::DirectoryScanner)
    , myIsReady(true)
{
    mySettings = new QSettings(DataDirectory::Path + "/settings.ini",
                               QSettings::IniFormat, this);

    qDebug() << "setting up MIME type mappings";
    // TODO: read those from a mapping file
    content::MimeType::registerType("audio/mpeg", "mp3");
    content::MimeType::registerType("audio/x-vorbis+ogg", "ogg");
    content::MimeType::registerType("audio/x-vorbis+ogg", "oga");
    content::MimeType::registerType("audio/x-flac", "flac");
    content::MimeType::registerType("audio/x-wav", "wav");

    qDebug() << "registering content scanners";

    qDebug() << " - DirectoryScanner";
    myFileIndex->registerScanner(myDirectoryScanner);
    myFileIndex->registerInspector(new audio::AudioInspector);

    qDebug() << "setting up content sources";
    qDebug() << " - AlbumSource";
    myContentProvider->registerSource("albums",
                                      new audio::AlbumSource(myFileIndex));

    qDebug() << "setting up file undertaker";
    myFileUndertaker = new content::FileUndertaker(myFileIndex);
    myFileUndertaker->run();
}

void Context::scan()
{
    myIsReady = false;
    emit readyChanged();
    QTimer::singleShot(500, this, SLOT(doScan()));
}

void Context::doScan()
{
    myDirectoryScanner->setPath(QDir::homePath() + "/" + musicFolder());
    myFileIndex->scan();
    myContentProvider->updateModels();
    myIsReady = true;
    emit readyChanged();
}

void Context::clearIndex()
{
    myIsReady = false;
    emit readyChanged();
    myFileIndex->clear();
    myContentProvider->updateModels();
    myIsReady = true;
    emit readyChanged();
}

QString Context::musicFolder()
{
    mySettings->beginGroup(GROUP_GENERAL);
    QString folder = mySettings->value(KEY_MUSIC_FOLDER,
                                       QVariant("Music")).toString();
    mySettings->endGroup();
    return folder;
}

void Context::setMusicFolder(QString path)
{
    mySettings->beginGroup(GROUP_GENERAL);
    mySettings->setValue(KEY_MUSIC_FOLDER, QVariant(path));
    mySettings->endGroup();
    emit musicFolderChanged();
}

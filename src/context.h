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


#ifndef CONTEXT_H
#define CONTEXT_H

#include <QObject>
#include <QString>
#include <QStringList>

namespace content
{
    class FileIndex;
    class FileUndertaker;
    class ContentProvider;
    class DirectoryScanner;
}

class QSettings;

class Context : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString licenseText READ licenseText);
    Q_PROPERTY(QString musicFolder READ musicFolder WRITE setMusicFolder
                                   NOTIFY musicFolderChanged);
    Q_PROPERTY(bool isReady READ isReady NOTIFY readyChanged);
public:
    explicit Context(QObject *parent = 0);
    content::ContentProvider* contentProvider()
    {
        return myContentProvider;
    }

    Q_INVOKABLE void scan();
    Q_INVOKABLE void clearIndex();

private:
    QString licenseText();
    QString musicFolder();
    void setMusicFolder(QString path);

    bool isReady() { return myIsReady; }

    content::ContentProvider *myContentProvider;
    content::FileIndex *myFileIndex;
    content::FileUndertaker *myFileUndertaker;
    content::DirectoryScanner *myDirectoryScanner;
    QSettings *mySettings;
    bool myIsReady;

private slots:
    void doScan();

signals:
    void musicFolderChanged();
    void readyChanged();
};

#endif // CONTEXT_H

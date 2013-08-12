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


#include "contentmodel.h"
#include "contentprovider.h"
#include "localsource.h"

#include <QVariant>
#include <QString>
#include <QHash>
#include <QByteArray>
#include <QDebug>
#include <QAbstractItemModel>

content::ContentModel::ContentModel(content::ContentProvider *cp,
                                    QObject*)
    : QAbstractListModel(0 /* no parent*/)
    , myContentProvider(cp)
    , myContentCookie(0)
{
    myContentProvider->registerSource("file", new content::LocalSource);

    connect(myContentProvider, SIGNAL(newFile(int,content::File::Ptr)),
            this, SLOT(handleNewFile(int,content::File::Ptr)));
}

QHash<int, QByteArray> content::ContentModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";
    roles[InfoRole] = "info";
    roles[PreviewRole] = "preview";
    roles[ImageRole] = "image";
    roles[PathRole] = "path";
    roles[ResourceRole] = "resource";
    return roles;
}

content::ContentModel::~ContentModel()
{
    clearFiles();
}

void content::ContentModel::clearFiles()
{
    foreach (content::File::Ptr fptr, myFiles)
    {
        fptr.clear();
    }
    myFiles.clear();
    countChanged();
}

int content::ContentModel::rowCount(const QModelIndex&) const
{
    return myFiles.length();
}

QVariant content::ContentModel::data(const QModelIndex &index, int role) const
{
    if (! index.isValid()) return QVariant();
    content::File::Ptr fptr = myFiles.at(index.row());

    switch (role)
    {
    case Qt::DisplayRole:
        return QVariant(fptr->name());
    case InfoRole:
        return QVariant(fptr->info());
    case PreviewRole:
        return QVariant(fptr->preview());
    case ImageRole:
        return QVariant(fptr->image());
    case PathRole:
        return QVariant(fptr->path());
    case ResourceRole:
        return QVariant(fptr->resource());
    default:
        return QVariant();
    }
}

int content::ContentModel::count()
{
    return myFiles.length();
}

QVariant content::ContentModel::get(int row)
{
    if (row < myFiles.length())
    {
        content::File::Ptr fptr = myFiles.at(row);
        return QVariant(fptr->name());
    }
    else
    {
        return QVariant();
    }
}

QVariant content::ContentModel::getPath(int row)
{
    if (row < myFiles.length())
    {
        content::File::Ptr fptr = myFiles.at(row);
        return QVariant(fptr->path());
    }
    else
    {
        return QVariant();
    }
}

QString content::ContentModel::path()
{
    return myPath;
}

void content::ContentModel::setPath(QString path)
{
    qDebug() << "setting model path:" << path;
    myPath = path;
    emit pathChanged();
    update();
}

void content::ContentModel::update()
{
    beginResetModel();
    clearFiles();
    endResetModel();

    myContentCookie = myContentProvider->cookie();
    myContentProvider->list(myContentCookie, QUrl(myPath));
}

void content::ContentModel::handleNewFile(int cookie, content::File::Ptr f)
{
    if (cookie == myContentCookie && f->type() != content::File::INVALID)
    {
        int begin = myFiles.length();
        int end = myFiles.length() + 1;
        beginInsertRows(QModelIndex(), begin, end);
        myFiles.append(f);
        endInsertRows();
        countChanged();
    }
}

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


#include "foldermodel.h"
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QHash>
#include <QByteArray>

FolderModel::FolderModel(QObject *parent) :
    QAbstractListModel(parent)
{
    QDir home = QDir::home();
    foreach (QFileInfo finfo, home.entryInfoList())
    {
        if (finfo.isDir() && ! finfo.isHidden())
        {
            myFolders << finfo.fileName();
        }
    }

    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "name";
    setRoleNames(roles);
}

int FolderModel::rowCount(const QModelIndex&) const
{
    return myFolders.size();
}

QVariant FolderModel::data(const QModelIndex &index, int) const
{
    if (index.row() < myFolders.size())
    {
        return QVariant(myFolders.at(index.row()));
    }
    else
    {
        return QVariant();
    }
}

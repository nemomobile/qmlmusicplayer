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


#include "datadirectory.h"
#include <QDir>
#include <QStandardPaths>
#include <QDebug>

QString DataDirectory::Path;
QString DataDirectory::Covers;

void DataDirectory::initialize()
{
    QDir dataDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    qDebug() << "Using data location:" << dataDir.path();
    if (! dataDir.exists("mediaboxcore"))
    {
        bool ok = dataDir.mkpath("mediaboxcore");
        if (! ok)
            qCritical() << "could not create data directory";
    }

    dataDir.cd("mediaboxcore");
    if (! dataDir.exists("covers"))
    {
        bool ok = dataDir.mkdir("covers");
        if (! ok)
            qCritical() << "could not create covers directory";
    }
    dataDir.cd("covers");
    Covers = dataDir.path();

    dataDir.cdUp();
    Path = dataDir.path();
}

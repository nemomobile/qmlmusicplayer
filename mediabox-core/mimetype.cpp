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


#include "mimetype.h"
#include <QMap>
#include <QStringList>
#include <QDebug>

static QMap<QString, QString> myMapping;
static const QString UNKNOWN("application/x-unknown");

content::MimeType::MimeType()
    : myType(UNKNOWN)
{
}

content::MimeType::MimeType(QString filename)
{
    int idx = filename.lastIndexOf('.');
    myType = myMapping[filename.mid(idx + 1).toLower()];
    if (myType.isEmpty())
    {
        myType = "application/x-unknown";
    }
}

content::MimeType::MimeType(QString, QString name)
    : myType(name)
{
}

void content::MimeType::registerType(QString mimetype, QString extension)
{
    myMapping[extension] = mimetype;
    qDebug() << " -" << mimetype << "for extension" << extension;
}

bool content::MimeType::isRecognized()
{
    return myType != UNKNOWN;
}

bool content::MimeType::matches(const QString name)
{
    QStringList parts1 = myType.split('/');
    QStringList parts2 = name.split('/');

    bool matchesFirst = false;
    bool matchesSecond = false;

    if (parts2.at(0) == "*" || parts1.at(0) == parts2.at(0))
    {
        matchesFirst = true;
    }
    if (parts2.at(1) == "*" || parts1.at(1) == parts2.at(1))
    {
        matchesSecond = true;
    }

    return (matchesFirst && matchesSecond);
}

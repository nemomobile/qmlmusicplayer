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


#include "localsource.h"
#include "mimetype.h"
#include <QDir>
#include <QStringList>
#include <QDebug>

content::LocalSource::LocalSource(QObject *parent) :
    AbstractSource(parent)
{
}

content::File::Ptr content::LocalSource::resolve(QString)
{
    return content::File::Ptr(new content::File);
}

void content::LocalSource::list(int cookie, QString path)
{
    QDir dir(path);
    QStringList files = dir.entryList();

    foreach (QString f, files)
    {
        if (f.startsWith(".")) continue;

        content::File::Ptr fptr(new content::File(content::File::FILE));
        fptr->setName(f);
        fptr->setInfo("A file");
        fptr->setMimetype(content::MimeType(f));

        emit newFile(cookie, fptr);
    }
    emit newFile(cookie, content::File::Ptr(new content::File));
}

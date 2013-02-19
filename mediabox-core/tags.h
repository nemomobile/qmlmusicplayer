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


#ifndef TAGS_H
#define TAGS_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QByteArray>
#include <cstdio>

namespace tags
{

/* Class for parsing meta tags of files.
 */
class Tags
{
public:
    Tags(QString path);
    ~Tags();

    QStringList keys();
    QByteArray get(QString key);
    bool contains(QString key);

private:
    void initMapping();
    void initGenres();
    void parse(FILE *fd);

    QMap<QString, QByteArray> myTags;
};

}
#endif // TAGS_H

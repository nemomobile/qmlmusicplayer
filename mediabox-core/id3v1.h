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


#ifndef ID3V1_H
#define ID3V1_H

#include <cstdio>
#include <QMap>
#include <QString>
#include <QByteArray>

namespace tags
{

class Id3v1
{
public:
    Id3v1(FILE *fd, QMap<QString, QByteArray> &tags);
private:
    void readTagSoup(FILE *fd, char **soup);
    void parseTagSoup(char *soup);

    void setKey(QString key, char *s, int size);
    void setIntegerKey(QString key, unsigned char s);

    QMap<QString, QByteArray> &myTags;
};

}
#endif // ID3V1_H

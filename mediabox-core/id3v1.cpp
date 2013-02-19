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


#include "id3v1.h"
#include <QDebug>

tags::Id3v1::Id3v1(FILE *fd, QMap<QString, QByteArray> &tags)
    : myTags(tags)
{
    char *soup;
    readTagSoup(fd, &soup);
    parseTagSoup(soup);
    free(soup);
}

void tags::Id3v1::readTagSoup(FILE *fd, char **soup)
{
    fseek(fd, -128, SEEK_END);

    *soup = (char*) malloc(128);
    fread(*soup, 1, 128, fd);
}

void tags::Id3v1::parseTagSoup(char *soup)
{
    setKey("TITLE", soup + 3, 30);
    setKey("ARTIST", soup + 3, 30);
    setKey("ALBUM", soup + 3, 30);
    setKey("YEAR", soup + 93, 4);

    if (soup[125] == 0 && soup[126] != 0)
    {
        // ID3v1.1
        setKey("COMMENT", soup + 97, 28);
        setIntegerKey("TRACKNUMBER", soup[126]);
    }
    else
    {
        // ID3v1
        setKey("COMMENT", soup + 97, 30);
    }
    setIntegerKey("GENRE", soup[127]);
}

void tags::Id3v1::setKey(QString key, char *s, int size)
{
    myTags[key] = QByteArray(s, size);
}

void tags::Id3v1::setIntegerKey(QString key, unsigned char s)
{
    myTags[key] = QByteArray::number(s);
}

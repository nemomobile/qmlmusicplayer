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


#include "vorbis.h"
#include <cstdlib>
#include <QDebug>

int strfind(const char *haystack, const char *needle, int size, int needleSize)
{
    for (int pos = 0; pos < size - needleSize; pos++)
    {
        bool match = true;
        for (int i = 0; i < needleSize; i++)
        {
            if (haystack[pos + i] != needle[i])
            {
                match = false;
                break;
            }
        }

        if (match)
            return pos;
    }
    return -1;
}

int readInt(FILE *fd, int size)
{
    int value = 0;
    unsigned char buf[size];
    int readSize = fread(buf, 1, size, fd);
    for (int i = 0; i < readSize; i++)
    {
        value = (value << 8) + buf[i];
    }
    return value;
}

tags::Vorbis::Vorbis(FILE *fd, QMap<QString, QByteArray> &tags)
    : myTags(tags)
{
    char *soup;
    int size = 0;

    char buf[4];
    fread(buf, 1, 4, fd);

    if (buf[0] == 'f' && buf[1] == 'L' &&
        buf[2] == 'a' && buf[3] == 'C')
    {
        // it's FLAC
        size = readFlacTagSoup(fd, &soup);
    }
    else
    {
        char header[1024];
        char mark[] = { 3, 'v', 'o', 'r', 'b', 'i', 's' };

        fseek(fd, 0, SEEK_SET);
        fread(header, 1, 1024, fd);

        int pos = strfind(header, mark, 1024, 7);
        if (pos != -1)
        {
            // it's Ogg Vorbis
            fseek(fd, pos + 7, SEEK_SET);
            size = readVorbisTagSoup(fd, &soup);
        }
    }

    if (size > 0)
        parseTagSoup(soup, size);
}

int tags::Vorbis::readFlacTagSoup(FILE *fd, char **soup)
{
    // skip STREAMINFO block
    int btype = readInt(fd, 1);
    int size = readInt(fd, 3);
    char streamInfo[size];
    fread(streamInfo, 1, size, fd);

    // read VORBIS_COMMENT block
    btype = readInt(fd, 1);
    if (btype & 0x04)
    {
        size = readInt(fd, 3);
        if (size < 50000)
        {
            *soup = (char*) malloc(size);
            fread(*soup, 1, size, fd);
        }
        else
        {
            // most likely not a comment block
            size = 0;
        }
    }
    else
    {
        size = 0;
    }

    return size;

    /*
# skip STREAMINFO block
btype = fd.read(1)
size = _read_int(fd, 3)
block = fd.read(size)

# read VORBIS_COMMENT block
btype = fd.read(1)
if (ord(btype) & 0x04):
    size = _read_int(fd, 3)
    if (size < 50000):
        # most likely not a comment block
        soup = fd.read(size)
    else:
        soup = ""
else:
    soup = ""

return soup
    */
}

int tags::Vorbis::readVorbisTagSoup(FILE *fd, char **soup)
{
    *soup = (char*) malloc(1024 * 1024);  // ought to be large enough

    int offset = 0;
    int cnt = 0;
    while (true)
    {
        char block[512];
        if (fread(block, 1, 512, fd) == 0 || cnt > 10)
        {
            break;
        }

        memcpy(*soup + offset, block, 512);
        offset += 512;

        char mark[] = { 5, 'v', 'o', 'r', 'b', 'i', 's' };
        int pos = strfind(*soup, mark, offset + 512, 7);
        if (pos != -1)
        {
            return pos;
        }

        cnt++;
    }

    return 0;
}

void tags::Vorbis::parseTagSoup(char *soup, int size)
{
    char nullTriple[] = { 0, 0, 0 };
    // iterate over entries

    int offset = 1; // skip the first
    while (soup[offset] == 0) offset++;

    qDebug() << QByteArray(soup, size);

    while (offset < size)
    {
        int entrySize = 0;
        int pos = strfind(soup + offset, nullTriple, size - offset, 3);
        if (pos != -1)
        {
            entrySize = pos - 1; // skip the last
        }
        else
        {
            entrySize = size - offset;
        }

        qDebug() << QByteArray(soup + offset, entrySize);
        int equalsPos = strfind(soup + offset, "=", entrySize, 1);
        if (equalsPos != -1)
        {
            int keySize = equalsPos;
            int valueSize = entrySize - keySize - 1;

            char key[keySize];
            char value[valueSize];
            memcpy(key, soup + offset, keySize);
            memcpy(value, soup + offset + keySize + 1, valueSize);

            myTags[QString(QByteArray(key, keySize)).toUpper()] =
                    QByteArray(value, valueSize);
        }

        offset += entrySize + 4;
    }
}

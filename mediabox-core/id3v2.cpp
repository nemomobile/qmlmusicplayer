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


#include "id3v2.h"
#include <QTextCodec>
#include <QByteArray>
#include <QStringList>
#include <QDebug>

const unsigned char FLAG_UNSYNCHRONISATION = 1 << 7;
const unsigned char FLAG_EXTENDED_HEADER =   1 << 6;
const unsigned char FLAG_EXPERIMENTAL =      1 << 5;
const unsigned char FLAG_FOOTER_PRESENT =    1 << 4;

struct tags::Params
{
    int keyLength;
    int sizeLength;
    bool syncSafe;
    bool hasFlags;
    int flagCompressed;
    int flagEncrypted;
    int flagInGroup;
    QStringList encodings;
};

tags::Id3v2::Id3v2(FILE *fd, revision rev, QMap<QString, QByteArray> &tags,
                   QMap<QString, QString> &keyMapping)
    : myTags(tags)
    , myKeyMapping(keyMapping)
{
    Params params;

    switch (rev)
    {
    case REV2:
        params.keyLength = 3;
        params.sizeLength = 3;
        params.syncSafe = false;
        params.hasFlags = false;
        params.flagCompressed = 0;
        params.flagEncrypted = 0;
        params.flagInGroup = 0;
        break;
    case REV3:
        params.keyLength = 4;
        params.sizeLength = 4;
        params.syncSafe = false;
        params.hasFlags = true;
        params.flagCompressed = 0x0080;
        params.flagEncrypted = 0x0040;
        params.flagInGroup = 0x0020;
        params.encodings << "latin1" << "utf-16" << "utf_16_be" << "utf-8";
        break;
    case REV4:
        params.keyLength = 4;
        params.sizeLength = 4;
        params.syncSafe = true;
        params.hasFlags = true;
        params.flagCompressed = 0x0008;
        params.flagEncrypted = 0x0004;
        params.flagInGroup = 0x0002;
        params.encodings << "latin1" << "utf-16" << "utf_16_be" << "utf-8";
        break;
    }


    char *soup;
    unsigned char flags;
    int size = readTagSoup(fd, &soup, &flags);
    parseTagSoup(soup, size, params);
    free(soup);
}

bool tags::Id3v2::isString(QString &key)
{
    return (key == "COMMENT" ||
            key == "COPYRIGHT" ||
            key == "TITLE" ||
            key == "ARTIST" ||
            key == "ALBUM" ||
            key == "TRACKNUMBER" ||
            key == "GENRE" ||
            key == "YEAR");
}

int tags::Id3v2::readTagSoup(FILE *fd, char **soup, unsigned char *flags)
{
    fseek(fd, 3, SEEK_CUR);

    unsigned char vmajor, vrev;
    fread(&vmajor, 1, 1, fd);
    fread(&vrev, 1, 1, fd);
    fread(flags, 1, 1, fd);

    unsigned char sizeBytes[4];
    fread(sizeBytes, 1, 4, fd);
    int size = (sizeBytes[0] << 21) +
               (sizeBytes[1] << 14) +
               (sizeBytes[2] << 7) +
               sizeBytes[3];

    // don't continue with empty tags
    if (size < 10)
    {
        return 0;
    }

    int extSize = 0;
    if (*flags & FLAG_EXTENDED_HEADER)
    {
        unsigned char extBytes[4];
        fread(extBytes, 1, 4, fd);
        extSize = (extBytes[0] << 21) +
                  (extBytes[1] << 14) +
                  (extBytes[2] << 7) +
                  extBytes[1];
        if (extSize > 4)
        {
            unsigned char nil[extSize - 4];
            fread(nil, 1, extSize - 4, fd);
        }
    }

    *soup = (char*) malloc(size - 10);
    fread(*soup, 1, size - 10, fd);

    return size - 10;
}

bool tags::Id3v2::readFrame(char *soup, int size, int &pos, Params &params)
{
    if (pos > size - 10 || (soup[pos] == 0 && soup[pos + 1] == 0))
        return false;

    char key[params.keyLength];
    memcpy(key, soup + pos, params.keyLength);
    pos += params.keyLength;
    //qDebug() << "key" << key;

    uint valueSize = 0;
    if (params.syncSafe)
    {
        valueSize = (soup[pos] << 21) +
                    (soup[pos + 1] << 14) +
                    (soup[pos + 2] << 7) +
                    soup[pos + 3];
    }
    else
    {
        for (int i = 0; i < params.sizeLength; ++i)
        {
            uchar b = soup[pos + i];
            valueSize = (valueSize << 8) + b;
        }
    }

    if (valueSize > 0xffffff)
        return false;

    pos += params.sizeLength;

    int flags = 0;
    if (params.hasFlags)
    {
        flags = (soup[pos] << 8) + soup[pos + 1];
        pos += 2;
    }

    char valueBuf[valueSize];
    QString keyString(QByteArray(key, params.keyLength));
    if (myKeyMapping.contains(keyString))
        keyString = myKeyMapping[keyString];
    QByteArray valueString;
    if (flags & params.flagCompressed)
    {
        qDebug() << "compressed data :(";
        valueString = "<compressed>";
    }
    else
    {
        memcpy(valueBuf, soup + pos, valueSize);
        if (! params.encodings.isEmpty() && valueBuf[0] < 5 && isString(keyString))
        {
            // try to decode string
            //qDebug() << "decoding" << valueBuf[0];
            QString encoding = params.encodings.at(valueBuf[0]);
            QTextCodec *codec = QTextCodec::codecForName(qPrintable(encoding));
            QTextCodec *codec2 = QTextCodec::codecForName("utf-8");
            valueString = codec2->fromUnicode(codec->toUnicode(valueBuf + 1, valueSize - 1));

        }
        else
        {
            valueString = QByteArray(valueBuf, valueSize);
        }
    }
    pos += valueSize;

    if (keyString == "PICTURE")
    {
        valueString = parseApic(valueString);
    }

    myTags[keyString] = valueString;
    return true;
}

void tags::Id3v2::parseTagSoup(char *soup, int size, Params &params)
{
    int pos = 0;
    while (true)
    {
        if (! readFrame(soup, size, pos, params))
            break;
    }
}

QByteArray tags::Id3v2::parseApic(const QByteArray &data)
{
    qDebug() << data;
    int idx = data.indexOf((char) 0x00, 1);
    QByteArray mimeType = data.mid(1, idx - 1);
    int pictureType = data.at(idx + 1);
    qDebug() << "APIC Picture Type" << pictureType;
    qDebug() << "APIC MIME type" << mimeType;
    idx = data.indexOf((char) 0x00, idx + 2);
    if (idx != -1)
    {
        while (idx < data.length() - 1 && data.at(idx) == 0x00)
            idx++;

        // TODO: make sure, we really have JPEG/JFIF there
        qDebug() << "APIC" << idx;// << data.mid(idx);
        QByteArray picData = data.mid(idx);
        return picData;
    }
    else
    {
        return "";
    }
}

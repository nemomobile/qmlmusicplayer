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


#include "tags.h"
#include "id3v1.h"
#include "id3v2.h"
#include "vorbis.h"
#include <cstring>
#include <QDebug>


static QMap<QString, QString> keyMapping;
static QList<QByteArray> genreList;


tags::Tags::Tags(QString path)
{
    initMapping();
    initGenres();

    FILE *fd = fopen(qPrintable(path), "rb");
    if (fd)
    {
        qDebug() << "parsing tags of" << path;
        parse(fd);
        fclose(fd);
    }
    else
    {
        qDebug() << "error opening file" << path;
    }
}

tags::Tags::~Tags()
{
}

void tags::Tags::initMapping()
{
    if (keyMapping.isEmpty())
    {
        keyMapping["APIC"] = "PICTURE";
        keyMapping["COMM"] = "COMMENT";
        keyMapping["TCOP"] = "COPYRIGHT";
        keyMapping["TDOR"] = "DATE";
        keyMapping["TIT2"] = "TITLE";
        keyMapping["TLEN"] = "LENGTH";
        keyMapping["TPE1"] = "ARTIST";
        keyMapping["TALB"] = "ALBUM";
        keyMapping["TOAL"] = "ALBUM";
        keyMapping["TYER"] = "YEAR";
        keyMapping["TCON"] = "GENRE";
        keyMapping["TRCK"] = "TRACKNUMBER";

        keyMapping["COM"] = "COMMENT";
        keyMapping["PIC"] = "PICTURE";
        keyMapping["TAL"] = "ALBUM";
        keyMapping["TP1"] = "ARTIST";
        keyMapping["TT2"] = "TITLE";
        keyMapping["TRK"] = "TRACKNUMBER";
        keyMapping["TYE"] = "YEAR";
        keyMapping["TCO"] = "GENRE";
    }
}

void tags::Tags::initGenres()
{
    if (genreList.isEmpty())
    {
        // the official weird, unsorted, incomplete list of genres for ID3 tags
        genreList << "Blues"
                  << "Classic Rock"
                  << "Country"
                  << "Dance"
                  << "Disco"
                  << "Funk"
                  << "Grunge"
                  << "Hip-Hop"
                  << "Jazz"
                  << "Metal"
                  << "New Age"
                  << "Oldies"
                  << "Other"
                  << "Pop"
                  << "R&B"
                  << "Rap"
                  << "Reggae"
                  << "Rock"
                  << "Techno"
                  << "Industrial"

                  << "Alternative"
                  << "Ska"
                  << "Death Metal"
                  << "Pranks"
                  << "Soundtrack"
                  << "Euro-Techno"
                  << "Ambient"
                  << "Trip-Hop"
                  << "Vocal"
                  << "Jazz + Funk"
                  << "Fusion"
                  << "Trance"
                  << "Classical"
                  << "Instrumental"
                  << "Acid"
                  << "House"
                  << "Game"
                  << "Sound Clip"
                  << "Gospel"
                  << "Noise"

                  << "Alt Rock"
                  << "Bass"
                  << "Soul"
                  << "Punk"
                  << "Space"
                  << "Meditative"
                  << "Instrumental Pop"
                  << "Instrumental Rock"
                  << "Ethnic"
                  << "Gothic"
                  << "Darkwave"
                  << "Techno-Industrial"
                  << "Electronic"
                  << "Pop-Folk"
                  << "Eurodance"
                  << "Dream"
                  << "Southern Rock"
                  << "Comedy"
                  << "Cult"
                  << "Gangsta Rap"

                  << "Top 40"
                  << "Christian Rap"
                  << "Pop / Funk"
                  << "Jungle"
                  << "Native American"
                  << "Cabaret"
                  << "New Wave"
                  << "Psychedelic"
                  << "Rave"
                  << "Showtunes"
                  << "Trailer"
                  << "Lo-Fi"
                  << "Tribal"
                  << "Acid Punk"
                  << "Acid Jazz"
                  << "Polka"
                  << "Retro"
                  << "Musical"
                  << "Rock'n Roll"
                  << "Hard Rock"

                  << "Folk"
                  << "Folk / Rock"
                  << "National Folk"
                  << "Swing"
                  << "Fast-Fusion"
                  << "Bebop"
                  << "Latin"
                  << "Revival"
                  << "Celtic"
                  << "Bluegrass"
                  << "Avantgarde"
                  << "Gothic Rock"
                  << "Progressive Rock"
                  << "Psychedelic Rock"
                  << "Symphonic Rock"
                  << "Slow Rock"
                  << "Big Band"
                  << "Chorus"
                  << "Easy Listening"
                  << "Acoustic"

                  << "Humour"
                  << "Speech"
                  << "Chanson"
                  << "Opera"
                  << "Chamber Music"
                  << "Sonata"
                  << "Symphony"
                  << "Booty Bass"
                  << "Primus"
                  << "Porn Groove"
                  << "Satire"
                  << "Slow Jam"
                  << "Club"
                  << "Tango"
                  << "Samba"
                  << "Folklore"
                  << "Ballad"
                  << "Power Ballad"
                  << "Rhythmic Soul"
                  << "Freestyle"

                  << "Duet"
                  << "Punk Rock"
                  << "Drum Solo"
                  << "A Capella"
                  << "Euro-House"
                  << "Dance Hall"
                  << "Goa"
                  << "Drum & Bass"
                  << "Club-House"
                  << "Hardcore"
                  << "Terror"
                  << "Indie"
                  << "BritPop"
                  << "Negerpunk"
                  << "Polsk Punk"
                  << "Beat"
                  << "Christian Gangsta Rap"
                  << "Heavy Metal"
                  << "Black Metal"
                  << "Crossover"

                  << "Contemporary Christian"
                  << "Christian Rock"
                  << "Merengue"
                  << "Salsa"
                  << "Thrash Metal"
                  << "Anime"
                  << "JPop"
                  << "Synthpop";

        while (genreList.length() < 256)
            genreList << "Unknown";

    }
}

QStringList tags::Tags::keys()
{
    return myTags.keys();
}

QByteArray tags::Tags::get(QString key)
{
    return myTags[key];
}

bool tags::Tags::contains(QString key)
{
    return myTags.contains(key);
}

void tags::Tags::parse(FILE *fd)
{
    char tagType[3];
    fread(tagType, 1, 3, fd);

    char major, minor;
    fread(&major, 1, 1, fd);
    fread(&minor, 1, 1, fd);
    rewind(fd);

    if (strncmp(tagType, "Ogg", 3) == 0)
    {
        qDebug() << "detected Ogg";
        tags::Vorbis(fd, myTags);
    }
    else if (strncmp(tagType, "fLa", 3) == 0)
    {
        qDebug() << "detected FLAC";
        tags::Vorbis(fd, myTags);
    }
    else if (strncmp(tagType, "ID3", 3) == 0)
    {
        qDebug() << "detected ID3";
        switch (major)
        {
        case 2:
            qDebug() << "rev 2";
            tags::Id3v2(fd, tags::Id3v2::REV2, myTags, keyMapping);
            break;
        case 3:
            qDebug() << "rev 3";
            tags::Id3v2(fd, tags::Id3v2::REV3, myTags, keyMapping);
            break;
        case 4:
            qDebug() << "rev 4";
            tags::Id3v2(fd, tags::Id3v2::REV4, myTags, keyMapping);
            break;
        default:
            qDebug() << "rev 4?";
            tags::Id3v2(fd, tags::Id3v2::REV4, myTags, keyMapping);
            break;
        }
    }
    else
    {
        qDebug() << "possibly ID3v1";
        tags::Id3v1(fd, myTags);
    }

    // check if genre is numeric and has to be resolved
    if (myTags.contains("GENRE") && myTags["GENRE"].toInt() > 0)
    {
        int genreNumber = myTags["GENRE"].toInt();
        myTags["GENRE"] = genreList.at(genreNumber);
    }

    qDebug() << myTags;
}

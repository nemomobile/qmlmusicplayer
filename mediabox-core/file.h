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


#ifndef FILE_H
#define FILE_H

#include <QSharedPointer>
#include <QString>
#include "mimetype.h"

namespace content
{

/* Lightweight object representing a single unit of content. File objects are
 * moved around as shared pointers, so that they get deleted automatically
 * when falling out of scope everywhere.
 * Do not derive from this class as it's final.
 */
class File
{
public:
    typedef QSharedPointer<content::File> Ptr;

    enum Type { INVALID, FOLDER, FILE };
    File(Type type = INVALID);
    ~File();
    Type type() const { return myType; }

    QString name() const { return myName; }
    void setName(QString name) { myName = name; }

    QString info() const { return myInfo; }
    void setInfo(QString info) { myInfo = info; }

    MimeType mimetype() const { return myMimetype; }
    void setMimetype(const MimeType &mimetype) { myMimetype = mimetype; }

    QString path() const { return myPath; }
    void setPath(QString path) { myPath = path; }

    QString resource() const { return myResource; }
    void setResource(QString resource) { myResource = resource; }

    QString preview() const { return myPreview; }
    void setPreview(QString preview) { myPreview = preview; }

    QString image() const { return myImage; }
    void setImage(QString image) { myImage = image; }

    int index() const { return myIndex; }
    void setIndex(int index) { myIndex = index; }

    bool operator<(File &other) { return myIndex < other.myIndex; }

private:
    Type myType;
    MimeType myMimetype;
    QString myName;
    QString myInfo;
    QString myPath;
    QString myResource;

    QString myPreview;
    QString myImage;

    int myIndex;
};

}
#endif // FILE_H

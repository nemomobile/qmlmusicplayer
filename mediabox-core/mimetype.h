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


#ifndef MIMETYPE_H
#define MIMETYPE_H

#include <QString>

namespace content
{

class MimeType
{
public:
    MimeType();
    MimeType(QString filename);
    MimeType(QString nil, QString name);
    MimeType(const MimeType &other) { myType = other.myType; }

    static void registerType(QString mimetype, QString extension);
    bool isRecognized();

    QString name() const { return myType; }
    void setName(QString name) { myType = name; }

    bool equals(const MimeType &other) { return name() == other.name(); }
    bool matches(const QString name);
private:
    QString myType;
};

}
#endif // MIMETYPE_H

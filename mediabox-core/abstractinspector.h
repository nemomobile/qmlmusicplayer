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


#ifndef ABSTRACTINSPECTOR_H
#define ABSTRACTINSPECTOR_H

#include <QObject>
#include <QString>
#include "metadata.h"
#include "mimetype.h"

namespace content
{

/* Abstract base class for inspecting a file's meta data.
 */
class AbstractInspector : public QObject
{
    Q_OBJECT
public:
    explicit AbstractInspector(QObject *parent = 0);

    virtual bool handlesMimetype(content::MimeType &mimetype) = 0;
    virtual void inspectFile(QString path, Metadata &meta) = 0;

signals:

public slots:

};

}
#endif // ABSTRACTINSPECTOR_H

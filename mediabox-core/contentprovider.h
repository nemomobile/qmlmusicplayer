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


#ifndef CONTENTPROVIDER_H
#define CONTENTPROVIDER_H

#include <QObject>
#include <QMap>
#include <QUrl>
#include <QList>

#include "file.h"

namespace content
{

class AbstractSource;
class ContentModel;

class ContentProvider : public QObject
{
    Q_OBJECT
public:
    explicit ContentProvider(QObject *parent = 0);
    ~ContentProvider();

    ContentModel* createModel();
    void updateModels();
    void registerSource(QString prefix, content::AbstractSource *source);
    static content::File::Ptr resolve(QString uri);
    void list(int cookie, QUrl uri);
    int cookie() { return myCookieCounter++; }

private:
    //typedef QMap<QString, content::AbstractSource*> SourceMap;
    //SourceMap mySources;
    QList<content::ContentModel*> myModels;
    int myCookieCounter;

signals:
    void newFile(int cookie, content::File::Ptr file);
    void contentsUpdated();

public slots:

private slots:
    void handleNewFile(int cookie, content::File::Ptr file);

};

}
#endif // CONTENTPROVIDER_H

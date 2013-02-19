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


#include "contentprovider.h"
#include "abstractsource.h"
#include "contentmodel.h"
#include <QDebug>

static QMap<QString, content::AbstractSource*> mySources;


content::ContentProvider::ContentProvider(QObject *parent)
    : QObject(parent)
    , myCookieCounter(0)
{
}

content::ContentProvider::~ContentProvider()
{
    /*
    for (SourceMap::iterator iter = mySources.begin();
         iter != mySources.end(); iter++)
    {
        delete iter.value();
    }
    */

    foreach (content::ContentModel *model, myModels)
    {
        delete model;
    }
}

/* Registers a content source for a prefix. The content source is then owned
 * by the ContentProvider and must not be deleted.
 */
void content::ContentProvider::registerSource(QString prefix,
                                         content::AbstractSource *source)
{
    mySources[prefix] = source;

    connect(source, SIGNAL(newFile(int,content::File::Ptr)),
            this, SLOT(handleNewFile(int,content::File::Ptr)));
}

/* Creates and returns a new content model. The model is owned by the provider
 * and must not be destroyed.
 */
content::ContentModel* content::ContentProvider::createModel()
{
    content::ContentModel *model = new content::ContentModel(this, this);
    myModels.append(model);
    return model;
}

/* Updates all content models owned by the provider.
 */
void content::ContentProvider::updateModels()
{
    foreach (content::ContentModel *model, myModels)
    {
        model->update();
    }
}

/* Resolves the given path and returns a File object.
 */
content::File::Ptr content::ContentProvider::resolve(QString uri)
{
    // parse URI into prefix and path
    int idx = uri.indexOf("://");
    QString prefix = uri.left(idx);
    QString path = uri.mid(idx + 3);

    content::AbstractSource *src = mySources[prefix];
    qDebug() << "resolving" << uri << "with" << src;

    if (src)
    {
        return src->resolve(path);
    }
    else
    {
        return content::File::Ptr(new content::File(content::File::INVALID));
    }
}

/* Lists the given URI by emitting a newFile signal for each file.
 * Terminates by emitting a file object of type invalid.
 */
void content::ContentProvider::list(int cookie, QUrl uri)
{
    // parse URI into prefix and path
    QString prefix = uri.scheme();
    QString path = uri.path();

    content::AbstractSource *src = mySources[prefix];
    qDebug() << "listing" << path << "with" << src;

    if (src)
    {
        src->list(cookie, path);
    }
}

/* Handles newFile signals from the content sources.
 */
void content::ContentProvider::handleNewFile(int cookie, content::File::Ptr file)
{
    emit newFile(cookie, file);
}

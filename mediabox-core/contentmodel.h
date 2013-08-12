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


#ifndef CONTENTMODEL_H
#define CONTENTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "file.h"

class QVariant;
class QString;

namespace content
{

class ContentProvider;

/* Convenience class for browsing content sources using a list model compatible
 * with Qt and QML.
 */
class ContentModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count
               READ count
               NOTIFY countChanged);
    Q_PROPERTY(QString path
               READ path
               WRITE setPath
               NOTIFY pathChanged);

public:
    enum { PreviewRole = Qt::UserRole,
           InfoRole,
           ImageRole,
           PathRole,
           ResourceRole };

    explicit ContentModel(content::ContentProvider *cp, QObject *parent = 0);
    ~ContentModel();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE QVariant get(int row);
    Q_INVOKABLE QVariant getPath(int row);
    Q_INVOKABLE void update();
    int count();
    QHash<int, QByteArray> roleNames() const;

    QString path();
    void setPath(QString path);

private:
    void clearFiles();

    content::ContentProvider *myContentProvider;
    QString myPath;
    int myContentCookie;
    QList<content::File::Ptr> myFiles;

signals:
    void pathChanged();
    void countChanged();

public slots:

private slots:
    void handleNewFile(int cookie, content::File::Ptr f);

};

}

#endif // CONTENTMODEL_H

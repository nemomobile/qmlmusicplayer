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


#ifndef FILEINDEX_H
#define FILEINDEX_H

#include <QObject>
#include <QSqlDatabase>
#include <QMap>
#include <QSet>
#include <QString>
#include <QByteArray>
#include <QVariant>
#include <QList>
#include "file.h"
#include "abstractinspector.h"
#include "abstractscanner.h"
#include "datadirectory.h"
#include "mimetype.h"
#include "metadata.h"
#include "messagebus.h"

namespace content
{

/* Class for accessing and maintaining a file index. Our storage backend is a
 * SQLite database.
 */
class FileIndex : public QObject
{
    Q_OBJECT
public:
    explicit FileIndex(QObject *parent = 0);
    ~FileIndex();

    void registerInspector(content::AbstractInspector *inspector);
    void registerScanner(content::AbstractScanner *scanner);

    void clear();
    void scan();
    void removeFromIndex(int pathid);
    void removeFromIndex(QList<int> pathids);

    static QString quote(QString s);
    QList<content::Metadata> query(QString query);

private:
    enum ValueType { INT, FLOAT, STRING };

    void buildTypeMapping();

    content::Metadata inspectFile(QString path, content::MimeType &mimetype);
    int newPathId();
    bool insertTuple(int pathid, QString key, QVariant value);
    void addToIndex(QString path, content::MimeType &mimetype, int mtime,
                    content::Metadata data);

    QString selectColumn(QString key);
    QSet<int> lookup(QString key, QByteArray value);
    QSet<int> all();

    void skipWhitespace(QString input, int &pos);
    QString readToken(QString input, int &pos);
    QByteArray readConstant(QString input, int &pos);
    QSet<int> parseCondition(QString input, int &pos);
    QSet<int> parseExpression(QString input, int &pos);

    static QByteArray encode(const QByteArray& input);
    static QByteArray decode(const QByteArray& input);

    QSqlDatabase myConnection;
    QMap<QString, int> typeMapping;
    com::MessageBus myMessageBus;

    QList<content::AbstractScanner*> myScanners;
    QList<content::AbstractInspector*> myInspectors;

signals:
    void finished();

public slots:
    void discoverFile(QString path, int mtime);

};

}
#endif // FILEINDEX_H

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


#include "fileindex.h"
#include "datadirectory.h"
#include <QStringList>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDir>
#include <QTime>
#include <QDebug>

content::FileIndex::FileIndex(QObject *parent)
    : QObject(parent)
    , myConnection(QSqlDatabase::addDatabase("QSQLITE"))
    , myMessageBus(this)
{
    myConnection.setDatabaseName(DataDirectory::Path + "/fileindex.db");
    myConnection.open();

    QSqlQuery query(myConnection);
    QStringList queries;
    queries << "CREATE TABLE IF NOT EXISTS paths ("
               "  pathid   INT,"
               "  path     TEXT,"
               "  mtime    INT"
               ")";

    queries << "CREATE TABLE IF NOT EXISTS metadata ("
               "  pathid       INT,"
               "  type         INT,"
               "  key          TEXT,"
               "  stringvalue  TEXT,"
               "  intvalue     INT,"
               "  floatvalue   FLOAT"
               ")";

    queries << "CREATE INDEX IF NOT EXISTS idx_metadata_pathid "
               "ON metadata(pathid)";

    queries << "CREATE INDEX IF NOT EXISTS idx_metadata_key "
               "ON metadata(key)";

    queries << "CREATE INDEX IF NOT EXISTS idx_metadata_stringvalue "
               "ON metadata(stringvalue)";

    queries << "CREATE INDEX IF NOT EXISTS idx_metadata_intvalue "
               "ON metadata(intvalue)";

    queries << "CREATE INDEX IF NOT EXISTS idx_metadata_floatvalue "
               "ON metadata(floatvalue)";

    foreach (QString q, queries)
    {
        qDebug() << "executing statement:" << q;
        if (! query.exec(q))
        {
            qDebug() << "failed to execute query:" << query.lastError();
            break;
        }
    }

    buildTypeMapping();
}

content::FileIndex::~FileIndex()
{
    myConnection.close();

    foreach (content::AbstractScanner *scanner, myScanners)
    {
        delete scanner;
    }
}

/* Builds the type mapping for keys and their type.
 */
void content::FileIndex::buildTypeMapping()
{
    QSqlQuery query(myConnection);
    if (query.exec("SELECT DISTINCT key, type FROM metadata"))
    {
        while (query.next())
        {
            typeMapping[query.value(0).toString()] = query.value(1).toInt();
        }
    }
}

/* Registers the given scanner. The scanner is then owned by the FileIndex and
 * must not be deleted.
 */
void content::FileIndex::registerScanner(content::AbstractScanner *scanner)
{
    myScanners.append(scanner);
    connect(scanner, SIGNAL(foundFile(QString,int)),
            this, SLOT(discoverFile(QString,int)));
}

/* Registers the given inspector. The inspector is then owned by the FileIndex
 * and must not be deleted.
 */
void content::FileIndex::registerInspector(content::AbstractInspector *insp)
{
    myInspectors.append(insp);
}

/* Clears the complete index.
 */
void content::FileIndex::clear()
{
    QSqlQuery query(myConnection);
    if (query.exec("BEGIN TRANSACTION") &&
        query.exec("DELETE FROM metadata") &&
        query.exec("DELETE FROM paths") &&
        query.exec("END TRANSACTION"))
    {
        // ok
    }
    else
    {
        qDebug() << "failed to execute query:" << query.lastError();
    }
}

void content::FileIndex::scan()
{
    foreach (content::AbstractScanner *scanner, myScanners)
    {
        // it's way faster as a transaction because the DB doesn't have to
        // sync inbetween
        QSqlQuery query(myConnection);
        query.exec("BEGIN TRANSACTION");
        scanner->scan();
        query.exec("COMMIT");
    }
    myMessageBus.emitMessage("CONTENT_EV_FINISHED_SCANNING", QVariant());
}

/* Discovers the given file and adds it to the index, or updates it.
 */
void content::FileIndex::discoverFile(QString path, int mtime)
{
    QByteArray escapedPath = encode(path.toUtf8());

    // search for file in index
    QSqlQuery query(QString("SELECT pathid, mtime "
                            "FROM paths "
                            "WHERE path = '%1'").arg(QString(escapedPath)),
                    myConnection);
    content::MimeType mimetype(path);

    if (query.exec())
    {
        bool ok = query.next();

        if (! ok)
        {
            // file is new
            qDebug() << "New:" << path;

            content::Metadata meta = inspectFile(path, mimetype);
            meta["File.Path"] = QVariant(path.toUtf8());

            addToIndex(path, mimetype, mtime, meta);

            // guess MIME type and call file inspector
            // insert fields into DB
        }
        else if (mtime > query.value(1).toInt())
        {
            // file is updated
            qDebug() << "Updated:" << path;

            int pathid = query.value(0).toInt();
            content::Metadata meta;

            removeFromIndex(pathid);
            addToIndex(path, mimetype, mtime, meta);
            // retrieve MIME type and call file inspector
            // replace fields in DB
        }
        else
        {
            // file was unchanged
            //qDebug() << "Unchanged:" << path;
        }
    }
    else
    {
        qDebug() << "query failed:" << query.lastError();
    }
}

int content::FileIndex::newPathId()
{
    QSqlQuery query(myConnection);
    if (query.exec("SELECT max(pathid) FROM paths"))
    {
        if (query.next())
        {
            return query.value(0).toInt() + 1;
        }
    }
    else
    {
        qDebug() << "error:" << query.lastError();
    }

    return 0;
}

bool content::FileIndex::insertTuple(int pathid, QString key, QVariant value)
{
    QSqlQuery query(myConnection);

    QString q = "INSERT INTO metadata (pathid, type, key, %1value) "
            "VALUES (%2, %3, '%4', %5)";

    QByteArray escapedValue;
    bool ok = false;
    qDebug() << "INSERT" << key << value;

    switch (value.type())
    {
    case QVariant::Int:
        ok = query.exec(q.arg("int")
                        .arg(pathid)
                        .arg(INT)
                        .arg(key)
                        .arg(value.toInt()));
        typeMapping[key] = INT;
        break;

    case QVariant::Double:
        ok = query.exec(q.arg("float")
                        .arg(pathid)
                        .arg(FLOAT)
                        .arg(key)
                        .arg(value.toDouble()));
        typeMapping[key] = FLOAT;
        break;

    case QVariant::ByteArray:
        escapedValue = "'" + encode(value.toByteArray()) + "'";
        qDebug() << "escaped:" << escapedValue;
        ok = query.exec(q.arg("string")
                        .arg(pathid)
                        .arg(STRING)
                        .arg(key)
                        .arg(QString(escapedValue)));
        qDebug() << "query ok" << ok << q.arg("string")
                    .arg(pathid)
                    .arg(STRING)
                    .arg(key)
                    .arg(QString(escapedValue));
        typeMapping[key] = STRING;
        break;

    case QVariant::String:
        Q_ASSERT_X(false,
                   "FileIndex::insertTuple",
                   "meta data values must not be QString");
        break;

    default:
        break;
    }

    if (! ok)
    {
        qDebug() << "error:" << q << query.lastError();
    }

    return ok;
}

void content::FileIndex::addToIndex(QString path, content::MimeType&,
                                    int mtime, content::Metadata data)
{
    int pathid = newPathId();
    QSqlQuery query;
    QByteArray escapedPath = encode(path.toUtf8());

    QString q = "INSERT INTO paths (pathid, path, mtime) "
            "VALUES (%1, '%2', %3)";

    if (! query.exec(q.arg(pathid)
                     .arg(QString(escapedPath))
                     .arg(mtime)))
    {
        qDebug() << "error:" << q.arg(pathid)
                    .arg(QString(escapedPath))
                    .arg(mtime) << query.lastError();
    }

    data["File.Id"] = QVariant(pathid);
    for (content::Metadata::const_iterator iter = data.begin();
         iter != data.end(); iter++)
    {
        bool ok = insertTuple(pathid, iter.key(), iter.value());
        if (! ok) break;
    }
}

void content::FileIndex::removeFromIndex(int pathid)
{
    QString q1 = "DELETE FROM paths WHERE pathid = %1";
    QString q2 = "DELETE FROM metadata WHERE pathid = %1";
    qDebug() << "deleting" << pathid << " from index";

    QSqlQuery query(myConnection);
    if (! query.exec(q1.arg(pathid)))
    {
        qDebug() << "error:" << query.lastError();
        return;
    }

    if (! query.exec(q2.arg(pathid)))
    {
        qDebug() << "error:" << query.lastError();
        return;
    }
}

void content::FileIndex::removeFromIndex(QList<int> pathids)
{
    QStringList ids;
    foreach (int pathid, pathids)
        ids << QString::number(pathid);

    QString q1 = "DELETE FROM paths WHERE pathid IN (%1)";
    QString q2 = "DELETE FROM metadata WHERE pathid IN (%1)";

    QSqlQuery query(myConnection);

    if (! query.exec(q1.arg(ids.join(","))))
    {
        qDebug() << "error:" << query.lastError();
        return;
    }

    if (! query.exec(q2.arg(ids.join(","))))
    {
        qDebug() << "error:" << query.lastError();
        return;
    }
}

QString content::FileIndex::selectColumn(QString key)
{
    if (typeMapping.contains(key))
    {
        int type = typeMapping[key];
        switch (type)
        {
        case INT:
            return "intvalue";
        case FLOAT:
            return "floatvalue";
        case STRING:
            return "stringvalue";
        default:
            qDebug() << "unknown type:" << type;
            return "stringvalue";
        }
    }
    else
    {
        qDebug() << "unknown key:" << key;
        return "stringvalue";
    }
}

QSet<int> content::FileIndex::lookup(QString key, QByteArray value)
{
    QSet<int> result;

    QString column = selectColumn(key);
    QString predicate = column + " = " + value;

    QString q("SELECT pathid FROM metadata WHERE key = '%1' AND %2");
    QSqlQuery query(myConnection);
    if (query.exec(q.arg(key).arg(predicate)))
    {
        while (query.next())
        {
            result.insert(query.value(0).toInt());
        }
    }

    return result;
}

QSet<int> content::FileIndex::all()
{
    QSet<int> result;

    QString q("SELECT DISTINCT pathid FROM metadata");
    QSqlQuery query(myConnection);
    if (query.exec(q))
    {
        while (query.next())
        {
            result.insert(query.value(0).toInt());
        }
    }

    return result;
}

content::Metadata content::FileIndex::inspectFile(QString path,
                                                  content::MimeType &mimetype)
{
    content::Metadata meta;

    foreach (content::AbstractInspector *inspector, myInspectors)
    {
        if (inspector->handlesMimetype(mimetype))
        {
            inspector->inspectFile(path, meta);
        }
    }

    return meta;
}

QString content::FileIndex::quote(QString s)
{
    s.replace('\'', "@QUOTE@");
    return s;
}

/* Queries the file index.
 */
QList<content::Metadata> content::FileIndex::query(QString query)
{
    QList<content::Metadata> result;

    // split query into attributes and condition
    int idx = query.indexOf(" of ");
    if (idx != -1 )
    {
        QString attributes = query.left(idx);
        QString condition = query.mid(idx + 4);

        attributes = attributes.replace(' ', "");
        QStringList attrList = attributes.split(',', QString::SkipEmptyParts);

        qDebug() << "executing index query:" << query;
        //qDebug() << "attributes:" << attrList;
        //qDebug() << "condition:" << condition;
        QTime t = QTime::currentTime();

        // retrieve pathids for condition
        int pos = 0;
        QSet<int> pathids = parseCondition(condition, pos);

        // look up attributes and build up list of metadata
        QSqlQuery sqlQuery(myConnection);

        QStringList ids;
        foreach (int pathid, pathids)
        {
            ids << QString::number(pathid);
        }

        QString q = "SELECT pathid, key, intvalue, floatvalue, stringvalue "
                    "FROM metadata "
                    "WHERE pathid IN (%1) AND key IN (%2) ORDER BY pathid";

        if (sqlQuery.exec(q.arg(ids.join(","))
                           .arg("'" + attrList.join("','") + "'")))
        {
            content::Metadata meta;
            int prevPathid = -1;
            while (sqlQuery.next())
            {
                int pid = sqlQuery.value(0).toInt();
                if (pid != prevPathid)
                {
                    if (prevPathid != -1 && ! result.contains(meta))
                        result.append(meta);
                    meta = content::Metadata();
                }
                QString key = sqlQuery.value(1).toString();
                QString column = selectColumn(key);

                if (column == "intvalue")
                {
                    int v = sqlQuery.value(2).toInt();
                    meta[key] = QVariant(v);
                }
                else if (column == "floatvalue")
                {
                    double v = sqlQuery.value(3).toDouble();
                    meta[key] = QVariant(v);
                }
                else if (column == "stringvalue")
                {
                    QByteArray value = sqlQuery.value(4).toByteArray();
                    meta[key] = QVariant(decode(value));
                }

                prevPathid = pid;
            }
            if (! result.contains(meta) && ! meta.isEmpty())
                result.append(meta);
        }

        int responseTime = t.msecsTo(QTime::currentTime());
        qDebug() << "got" << pathids.size() << "items in" << responseTime << "ms";
    }

    return result;
}

void content::FileIndex::skipWhitespace(QString input, int &pos)
{
    while (pos < input.length() && input.at(pos) == ' ') pos++;
}

QString content::FileIndex::readToken(QString input, int &pos)
{
    QString token;
    skipWhitespace(input, pos);
    while (input.at(pos) != '(' &&
           input.at(pos) != ')' &&
           input.at(pos) != ' ' &&
           pos < input.length())
    {
        token.append(input.at(pos));
        pos++;
    }

    return token;
}

QByteArray content::FileIndex::readConstant(QString input, int &pos)
{
    QByteArray value;
    skipWhitespace(input, pos);
    int begin = pos;
    if (input.at(pos) == '\'')
    {
        pos++;
        while (pos < input.length())
        {
            if (input.at(pos) == '\\')
            {
                pos++;
            }
            else if (input.at(pos) == '\'')
            {
                pos++;
                break;
            }
            pos++;
        }
        value = "'" + encode(input.mid(begin + 1, pos - begin - 2)
                                  .replace("@QUOTE@", "'")
                                  .toUtf8()) +
                "'";
    }
    else
    {
        while (pos < input.length() && input.at(pos) != ' ')
            pos++;
        value = input.mid(begin, pos - begin).trimmed().toUtf8();
    }

    return value;
}

QSet<int> content::FileIndex::parseCondition(QString input, int &pos)
{
    QSet<int> result;

    //qDebug() << "parse cond" << input << pos;
    skipWhitespace(input, pos);
    if (input.at(pos) == '(')
    {
        pos++;
        QSet<int> set1 = parseCondition(input, pos);
        QString op = readToken(input, pos);
        QSet<int> set2;
        if (! op.isEmpty())
        {
            set2 = parseCondition(input, pos);
        }

        skipWhitespace(input, pos);
        // skip ')'
        pos++;

        // evaluate
        if (op == "and")
        {
            result = set1.intersect(set2);
        }
        else if (op == "or")
        {
            result = set1.unite(set2);
        }
        else if (op.isEmpty())
        {
            result = set1;
        }
        else
        {
            // error, invalid operator
            //setError("invalid operator in '" + input + "'");
        }
    }
    else
    {
        result = parseExpression(input, pos);
    }

    //qDebug() << result;
    return result;
}

QSet<int> content::FileIndex::parseExpression(QString input, int &pos)
{
    QSet<int> result;

    QString lvalue = readToken(input, pos);
    if (lvalue == "all")
    {
        // select all
        result = all();
    }
    else
    {
        QString op = readToken(input, pos);
        QByteArray rvalue = readConstant(input, pos);

        if (op == "=")
        {
            result = lookup(lvalue, rvalue);
        }
        else
        {
            // error, invalid operator
            //setError("Invalid operator in '" + input + "'");
        }
    }

    return result;
}


QByteArray content::FileIndex::encode(const QByteArray &input)
{
    return input.toPercentEncoding("", "'&").replace('%', '&');
}

QByteArray content::FileIndex::decode(const QByteArray &input)
{
    QByteArray s = input;
    return QByteArray::fromPercentEncoding(s.replace('&', '%'));
}

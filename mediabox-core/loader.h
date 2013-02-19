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


#ifndef LOADER_H
#define LOADER_H

#include <QObject>
#include <QString>
#include <QPluginLoader>
#include <QDebug>
#include "plugin.h"

namespace com
{

class Loader : public QObject
{
    Q_OBJECT
public:
    explicit Loader(QString path)
        : QObject(0)
        , myPath(path)
    { }
    template<class P>
    P* get() {

        QPluginLoader loader(myPath);
        QObject *obj = loader.instance();
        qDebug() << "loaded external component" << obj;
        if (obj)
        {
            qDebug() << "Loaded Plugin" << myPath;
            com::Plugin *plugin = qobject_cast<com::Plugin*>(obj);
            if (plugin)
            {
                qDebug() << "cast plugin" << plugin << plugin->getObject();
                return (P*) plugin->getObject();
            }
        }
        else
        {
            qDebug() << "LOAD ERROR:" << loader.errorString();
        }

        return 0;

    }

private:
    QString myPath;
};

}
#endif // LOADER_H

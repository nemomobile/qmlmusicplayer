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


#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QVariant>

namespace com
{

/* Class for an application-wide message bus.
 */
class MessageBus : public QObject
{
    Q_OBJECT
public:
    explicit MessageBus(QObject *target, QObject *parent = 0);
    void emitMessage(QString name, QVariant payload);

private:
    QObject *myTarget;

private slots:
    void handleMessage(QObject *source, QString name, QVariant payload);


public slots:

};

}
#endif // MESSAGEBUS_H

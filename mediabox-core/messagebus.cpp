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


#include "messagebus.h"
#include "messagehub.h"
#include <QMetaObject>
#include <QMetaMethod>
#include <QDebug>

static com::MessageHub myMessageHub;


com::MessageBus::MessageBus(QObject *target, QObject *parent)
    : QObject(parent)
    , myTarget(target)
{
    connect(&myMessageHub, SIGNAL(message(QObject*,QString,QVariant)),
            this, SLOT(handleMessage(QObject*,QString,QVariant)));
}

void com::MessageBus::emitMessage(QString name, QVariant payload)
{
    myMessageHub.emitMessage(this, name, payload);
}

void com::MessageBus::handleMessage(QObject *source,
                                    QString name, QVariant payload)
{
    if (source != this)
    {
        int idx = myTarget->metaObject()->indexOfMethod(
                qPrintable("handle_" + name + "(QVariant)"));
        if (idx != -1)
        {
            QMetaMethod method = myTarget->metaObject()->method(idx);
            method.invoke(myTarget, Qt::DirectConnection,
                          Q_ARG(QVariant, payload));
        }
    }
}

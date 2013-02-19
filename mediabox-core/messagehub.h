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


#ifndef MESSAGEHUB_H
#define MESSAGEHUB_H

#include <QObject>
#include <QString>
#include <QVariant>

namespace com
{

/* Hub of the message bus communication. This class is used internally by the
 * MessageBus class.
 */
class MessageHub : public QObject
{
    Q_OBJECT
public:
    explicit MessageHub();
    void emitMessage(QObject *source, QString name, QVariant payload);

signals:
    void message(QObject *source, QString name, QVariant payload);

};

}
#endif // MESSAGEHUB_H

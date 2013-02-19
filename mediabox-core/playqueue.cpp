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


#include "playqueue.h"

media::PlayQueue::PlayQueue(QObject *parent) :
    QObject(parent)
{
}

media::PlayQueue::~PlayQueue()
{

}

void media::PlayQueue::append(content::File::Ptr fptr)
{
    queue.append(fptr);
}

void media::PlayQueue::replace(content::File::Ptr fptr)
{
    queue.clear();
    queue.append(fptr);
}

void media::PlayQueue::previous()
{
    if (! history.isEmpty())
    {
        content::File::Ptr fptr = history.takeLast();
        queue.prepend(fptr);
        emit itemChanged(fptr);
    }
}

void media::PlayQueue::next()
{
    if (! queue.isEmpty())
    {
        content::File::Ptr fptr = queue.takeFirst();
        history.append(fptr);
        if (! queue.isEmpty())
            emit itemChanged(queue.first());
    }
}

void media::PlayQueue::skip(int n)
{
    for (int i = 0; i < n && ! queue.isEmpty(); i++)
    {
        content::File::Ptr fptr = queue.takeFirst();
        history.append(fptr);
    }
    if (! queue.isEmpty())
        emit itemChanged(queue.first());
}

QString media::PlayQueue::nowPlaying()
{
    if (! queue.isEmpty())
        return queue.first()->path();
    else
        return "";
}

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


#include "player.h"
#include <QUrl>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMessage>
#include <QDebug>

media::Player::Player(QObject *parent)
    : QObject(parent)
    , myBackend(0)
    , myIsPlaying(false)
    , myDuration(0)
    , myPosition(0)
    , myVolume(50)
{
}

media::Player::~Player()
{
    for (QMap<QString, media::AbstractBackend*>::iterator iter = myBackends.begin();
             iter != myBackends.end(); iter++)
    {
        delete *iter;
    }
}

/* Inhibits the device from entering standby mode.
 */
void media::Player::inhibitStandby()
{
    qDebug() << "inhibiting standby";
    QDBusInterface inhibitor("org.freedesktop.PowerManagement",
                             "/org/freedesktop/PowerManagement/Inhibit",
                             "org.freedesktop.PowerManagement.Inhibit");
    QDBusReply<quint32> reply = inhibitor.call("Inhibit", "MusicShelf",
                                               "Playing Music");
    qDebug() << "reply:" << reply << reply.isValid();
    qDebug() << "error:" << reply.error();

    if (reply.isValid())
    {
        myInhibitionCookie = reply.value();
        qDebug() << "inhibitor cookie:" << reply.value();
    }
}

/* Allows the device to enter standby mode.
 */
void media::Player::uninhibitStandby()
{
    qDebug() << "allowing standby";
    QDBusInterface inhibitor("org.freedesktop.PowerManagement",
                             "/org/freedesktop/PowerManagement/Inhibit",
                             "org.freedesktop.PowerManagement.Inhibit");
    QDBusReply<bool> hasInhibit = inhibitor.call("HasInhibit");
    qDebug() << "have inhibit" << hasInhibit;
    if (hasInhibit)
    {
        qDebug() << "uninhibiting with cookie" << myInhibitionCookie;
        inhibitor.call("UnInhibit", myInhibitionCookie);
    }
    hasInhibit = inhibitor.call("HasInhibit");
    qDebug() << "have inhibit" << hasInhibit;
}

/* Loads the given backend object. The backend is owned by the player.
 */
void media::Player::registerBackend(QString name, media::AbstractBackend *backend)
{
    // connect the signals
    connect(backend, SIGNAL(eofReached()),
            this, SIGNAL(eofReached()));
    connect(backend, SIGNAL(stateChanged(media::AbstractBackend::State)),
            this, SLOT(handleStateChanged(media::AbstractBackend::State)));
    connect(backend, SIGNAL(positionChanged(int,int)),
            this, SLOT(handlePositionChanged(int,int)));
    connect(backend, SIGNAL(errorOccured(media::ErrorCode,QString)),
            this, SIGNAL(errorOccured(media::ErrorCode,QString)));
    connect(backend, SIGNAL(volumeChanged(int)),
            this, SLOT(handleVolumeChanged(int)));

    myBackends[name] = backend;
}

/* Selects the named backend for playing.
 */
void media::Player::selectBackend(QString name)
{
    if (myBackends.contains(name))
    {
        myBackend = myBackends[name];
    }
}

void media::Player::load(content::File::Ptr fptr)
{
    qDebug() << "using player backend" << myBackend;
    if (myBackend)
    {
        myBackend->setVolume(myVolume);
        myBackend->load(fptr->resource(), false);
    }
    myUri = fptr->resource();
    emit uriChanged();
}

void media::Player::loadPath(QString path)
{
    qDebug() << "loading media:" << path;
    if (myBackend)
        myBackend->load(path, false);
}

void media::Player::play()
{
    if (myBackend)
        myBackend->play();
}

void media::Player::pause()
{
    if (myBackend)
    {
        if (myIsPlaying)
            myBackend->stop();
        else
            myBackend->play();
    }
}

void media::Player::stop()
{
    if (myBackend)
        myBackend->stop();
}

void media::Player::seek(int millisecs)
{
    if (myBackend)
    {
        myBackend->seek(millisecs);
        emit positionChanged(millisecs);
    }
}

void media::Player::setVolume(int vol)
{
    if (myBackend)
    {
        myBackend->setVolume(vol);
        myVolume = vol;
        emit volumeChanged(vol);
    }
}

int media::Player::volume()
{
    return myVolume;
}

void media::Player::handleStateChanged(media::AbstractBackend::State state)
{
    switch (state)
    {
    case media::AbstractBackend::NONE:
        break;
    case media::AbstractBackend::LOADING:
        qDebug() << "[LOADING]";
        break;
    case media::AbstractBackend::PLAYING:
        qDebug() << "[PLAYING]";
        myIsPlaying = true;
        emit isPlayingChanged();
        break;
    case media::AbstractBackend::STOPPED:
        qDebug() << "[STOPPED]";
        myIsPlaying = false;
        emit isPlayingChanged();
        break;
    }
}

void media::Player::handlePositionChanged(int pos, int duration)
{
    if (pos != myPosition)
    {
        myPosition = pos;
        emit positionChanged(pos);
    }
    if (duration != myDuration)
    {
        myDuration = duration;
        emit durationChanged(duration);
    }
}

void media::Player::handleVolumeChanged(int vol)
{
    myVolume = vol;
    emit volumeChanged(vol);
}

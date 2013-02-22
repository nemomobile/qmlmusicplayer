/* This file is part of Nemo QML Music Player
 *
 * Based on: phononbackend.h
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

#include "mobilitybackend.h"

#include <QUrl>

MobilityBackend::MobilityBackend()
    : media::AbstractBackend()
    , player()
{
    connect(&player, SIGNAL(positionChanged(qint64)),
            this, SLOT(handlePositionChanged(qint64)));

    connect(&player, SIGNAL(durationChanged(qint64)),
            this, SLOT(handleDurationChanged(qint64)));

    connect(&player, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(handleStateChanged(QMediaPlayer::State)));

    connect(&player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(handleMediaStatusChanged(QMediaPlayer::MediaStatus)));
}

MobilityBackend::~MobilityBackend()
{
}

void MobilityBackend::load(QString path, bool)
{
    player.setMedia(QUrl::fromLocalFile(path));
    player.play();
}

bool MobilityBackend::isEof()
{
    return player.mediaStatus() == QMediaPlayer::EndOfMedia;
}

void MobilityBackend::play()
{
    player.play();
}

void MobilityBackend::stop()
{
    player.pause();
}

void MobilityBackend::seek(int millisecs)
{
    player.setPosition(millisecs);
}

void MobilityBackend::setVolume(int vol)
{
}

void MobilityBackend::handlePositionChanged(qint64 pos)
{
    emit positionChanged(pos, player.duration());
}

void MobilityBackend::handleDurationChanged(qint64 duration)
{
    emit positionChanged(player.position(), duration);
}

void MobilityBackend::handleStateChanged(QMediaPlayer::State state)
{
    switch (state)
    {
    case QMediaPlayer::PlayingState:
        emit stateChanged(PLAYING);
        break;
    case QMediaPlayer::PausedState:
        emit stateChanged(STOPPED);
        break;
    case QMediaPlayer::StoppedState:
        emit stateChanged(NONE);
        break;
    }
}

void MobilityBackend::handleMediaStatusChanged(QMediaPlayer::MediaStatus mediaStatus)
{
    switch (mediaStatus)
    {
    case QMediaPlayer::EndOfMedia:
        emit eofReached();
        break;
    }
}

void MobilityBackend::handleMetaDataChanged()
{

}

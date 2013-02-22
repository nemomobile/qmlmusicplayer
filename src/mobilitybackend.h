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


#ifndef MOBILITYBACKEND_H
#define MOBILITYBACKEND_H

#include "mediabox-core/abstractbackend.h"
#include <QMediaPlayer>

class MobilityBackend : public media::AbstractBackend {
    Q_OBJECT
public:
    MobilityBackend();
    ~MobilityBackend();

    virtual void load(QString path, bool withVideo);
    virtual bool isEof();
    virtual void play();
    virtual void stop();
    virtual void seek(int millisecs);
    virtual void setVolume(int vol);

private:
    QMediaPlayer player;

private slots:
    void handlePositionChanged(qint64 pos);
    void handleDurationChanged(qint64 duration);
    void handleStateChanged(QMediaPlayer::State state);
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus mediaStatus);
    void handleMetaDataChanged();
};

#endif // MOBILITYBACKEND_H

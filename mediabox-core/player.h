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


#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QMap>
#include <QString>
#include "file.h"
#include "abstractbackend.h"

namespace media
{

class Player : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isPlaying
               READ isPlaying
               NOTIFY isPlayingChanged);
    Q_PROPERTY(int position
               READ position
               WRITE seek
               NOTIFY positionChanged);
    Q_PROPERTY(int duration
               READ duration
               NOTIFY durationChanged);
    Q_PROPERTY(int volume
               READ volume
               WRITE setVolume
               NOTIFY volumeChanged);
    Q_PROPERTY(QString uri
               READ currentUri
               NOTIFY uriChanged);

public:
    explicit Player(QObject *parent = 0);
    ~Player();

    void registerBackend(QString name, AbstractBackend *backend);
    void selectBackend(QString name);

    Q_INVOKABLE void loadPath(QString path);
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    void seek(int millisecs);
    void setVolume(int vol);
    int volume();

    int duration() { return myDuration; }
    int position() { return myPosition; }
    int isPlaying() { return myIsPlaying; }

signals:
    void uriChanged();
    void isPlayingChanged();
    void positionChanged(int pos);
    void durationChanged(int duration);
    void volumeChanged(int vol);
    void errorOccured(media::ErrorCode err, QString message);
    void eofReached();

private:
    QString currentUri() { return myUri; }
    void inhibitStandby();
    void uninhibitStandby();

    quint32 myInhibitionCookie;
    QMap<QString, AbstractBackend*> myBackends;
    AbstractBackend *myBackend;
    QString myUri;
    bool myIsPlaying;
    int myDuration;
    int myPosition;
    int myVolume;

public slots:
    void load(content::File::Ptr fptr);

private slots:
    void handleStateChanged(media::AbstractBackend::State state);
    void handlePositionChanged(int pos, int duration);
    void handleVolumeChanged(int vol);
};

}
#endif // PLAYER_H

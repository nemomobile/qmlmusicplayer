/* This file is part of Music Shelf
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


#include "phononbackend.h"
#include <QTimer>
#include <QUrl>
#include <QDebug>

PhononBackend::PhononBackend()
    : media::AbstractBackend()
    , myMediaObject(0)
    , myDuration(0)
{
    myMediaObject = Phonon::createPlayer(Phonon::MusicCategory);
    myMediaObject->setTickInterval(1000);

    connect(myMediaObject, SIGNAL(finished()),
            this, SIGNAL(eofReached()));

    connect(myMediaObject, SIGNAL(tick(qint64)),
            this, SLOT(handlePositionChanged(qint64)));
    connect(myMediaObject, SIGNAL(totalTimeChanged(qint64)),
            this, SLOT(handleDurationChanged(qint64)));

    connect(myMediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)),
            this, SLOT(handleStateChanged(Phonon::State)));
    connect(myMediaObject, SIGNAL(metaDataChanged()),
            this, SLOT(handleMetaDataChanged()));

#ifdef FOR_WETAB
    // there is a bug with phonon on the WeTab. you cannot seek in MP3 files
    // unless you have previously loaded a seekable file of another format.
    // so we load a NULL Ogg file now.
    myMediaObject->setCurrentSource(QUrl::fromLocalFile("/opt/org.pycage.musicshelf/null.oga"));
    myMediaObject->seek(0);
#endif
}

PhononBackend::~PhononBackend()
{
    if (myMediaObject)
    {
        myMediaObject->setCurrentSource(QString());
        myMediaObject->deleteLater();
        myMediaObject = 0;
    }
}

void PhononBackend::load(QString path, bool)
{
    if (myMediaObject)
    {
        qDebug() << "loading into backend" << path;
        myMediaObject->setCurrentSource(path);
        //myMediaObject->play();

        QTimer::singleShot(100, this, SLOT(handleTimer()));
    }
}

bool PhononBackend::isEof()
{
    return false;
}

void PhononBackend::play()
{
    if (myMediaObject)
        myMediaObject->play();
}

void PhononBackend::stop()
{
    if (myMediaObject)
        myMediaObject->pause();
}

void PhononBackend::seek(int millisecs)
{
    qDebug() << "seeking to" << millisecs;
    if (myMediaObject)
    {
        qDebug() << "is seekable" << myMediaObject->isSeekable();
        myMediaObject->seek(millisecs);
        myMediaObject->play();
    }
}

void PhononBackend::setVolume(int vol)
{
    foreach (Phonon::Path path, myMediaObject->outputPaths())
    {
        Phonon::AudioOutput* sink = dynamic_cast<Phonon::AudioOutput*>(path.sink());
        if (sink)
            sink->setVolume(vol / 100.0);
    }
}

void PhononBackend::handleTimer()
{
    if (myMediaObject)
    {
        myMediaObject->seek(0);
        myMediaObject->play();
    }
}

void PhononBackend::handlePositionChanged(qint64 pos)
{
    emit positionChanged(pos, myDuration);
}

void PhononBackend::handleDurationChanged(qint64 duration)
{
    myDuration = duration;
    emit positionChanged(0, duration);
}

void PhononBackend::handleStateChanged(Phonon::State state)
{
    switch (state)
    {
    case Phonon::PlayingState:
        emit stateChanged(PLAYING);
        break;
    case Phonon::PausedState:
        emit stateChanged(STOPPED);
        break;
    case Phonon::StoppedState:
        emit stateChanged(NONE);
        break;
    case Phonon::LoadingState:
        emit stateChanged(LOADING);
        break;
    case Phonon::BufferingState:
        break;
    case Phonon::ErrorState:
        qDebug() << "Phonon error:" << myMediaObject->errorString();
        break;
    }
}

void PhononBackend::handleMetaDataChanged()
{

}

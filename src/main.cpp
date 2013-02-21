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


#include <QApplication>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QDir>
#include <QDesktopServices>
#include <QDebug>
#include <QDesktopWidget>
#include <QDeclarativeView>

#include "context.h"
#include "foldermodel.h"
#include "phononbackend.h"

#include "mediabox-core/abstractbackend.h"
#include "mediabox-core/contentmodel.h"
#include "mediabox-core/contentprovider.h"
#include "mediabox-core/datadirectory.h"
#include "mediabox-core/player.h"
#include "mediabox-core/playqueue.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setOrganizationDomain("org.pycage");
    QApplication::setOrganizationName("pycage");
    app.setApplicationName("Music Shelf");
    DataDirectory::initialize();

    QDeclarativeView win;

    Context context;

    qDebug() << "setting up player backends";
    media::Player *player = new media::Player;
    QDir plugins = QFileInfo(argv[0]).absoluteDir();
    /*
    media::AbstractBackend *phonon = com::Loader(
                plugins.absoluteFilePath("libQMediaBackend.so"))
            .get<media::AbstractBackend>();
    */
    qDebug() << " - PhononBackend";
    media::AbstractBackend *phonon = new PhononBackend;
    if (phonon)
    {
        player->registerBackend("phonon", phonon);
        player->selectBackend("phonon");
    }


    qDebug() << "setting up playing queue";
    media::PlayQueue *playQueue = new media::PlayQueue;
    QApplication::connect(playQueue, SIGNAL(itemChanged(content::File::Ptr)),
                          player, SLOT(load(content::File::Ptr)));
    QApplication::connect(player, SIGNAL(eofReached()),
                          playQueue, SLOT(next()));

    FolderModel folderModel;

    // export into QML world
    win.rootContext()->setContextProperty("context", &context);
    win.rootContext()->setContextProperty("folderModel", &folderModel);
    win.rootContext()->setContextProperty("albumModel",
                                      context.contentProvider()->createModel());
    win.rootContext()->setContextProperty("tracksModel",
                                      context.contentProvider()->createModel());
    win.rootContext()->setContextProperty("player", player);
    win.rootContext()->setContextProperty("playQueue", playQueue);

    win.setSource(QUrl("qrc:/qml/main.qml"));
    win.setResizeMode(QDeclarativeView::SizeRootObjectToView);

#if defined(QML_MEDIA_PLAYER_DESKTOP_BUILD)
    QDesktopWidget *desktop = app.desktop();
    if (desktop->height() < 1000) {
        win.scale(.8, .8);
        win.resize(win.size() * .8);
    }
    win.show();
#else
    win.showFullScreen();
#endif

    return app.exec();
}

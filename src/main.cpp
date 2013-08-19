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


#include <QGuiApplication>
#include <QQuickView>
#include <QtQml>
#include <QQmlComponent>
#include <QQmlContext>
#include <QDir>
#include <QDesktopServices>
#include <QDebug>

#include "context.h"
#include "foldermodel.h"
#include "mobilitybackend.h"

#include "mediabox-core/abstractbackend.h"
#include "mediabox-core/contentmodel.h"
#include "mediabox-core/contentprovider.h"
#include "mediabox-core/datadirectory.h"
#include "mediabox-core/player.h"
#include "mediabox-core/playqueue.h"

#ifdef HAS_BOOSTER
#include <MDeclarativeCache>
#endif

#ifdef HAS_BOOSTER
Q_DECL_EXPORT
#endif
int main(int argc, char *argv[])
{
    QGuiApplication* application;
    QQuickView* view;
    
    DataDirectory::initialize();
#ifdef HAS_BOOSTER
    application = MDeclarativeCache::qApplication(argc, argv);
    view = MDeclarativeCache::qQuickView();
#else
    qWarning() << Q_FUNC_INFO << "Warning! Running without booster. This may be a bit slower.";
    QGuiApplication stackApp(argc, argv);
    QQuickView stackView;
    application = &stackApp;
    view = &stackView;
#endif
    QGuiApplication::setOrganizationDomain("org.nemomobile");
    QGuiApplication::setOrganizationName("nemomobile");
    application->setApplicationName("Music Player");
    Context context;

    media::Player player;
    MobilityBackend backend;
    player.registerBackend("mobility", &backend);
    player.selectBackend("mobility");

    qDebug() << "setting up playing queue";
    media::PlayQueue *playQueue = new media::PlayQueue;
    QGuiApplication::connect(playQueue, SIGNAL(itemChanged(content::File::Ptr)),
                          &player, SLOT(load(content::File::Ptr)));
    QGuiApplication::connect(&player, SIGNAL(eofReached()),
                          playQueue, SLOT(next()));

    FolderModel folderModel;

    // export into QML world
    view->rootContext()->setContextProperty("context", &context);
    view->rootContext()->setContextProperty("folderModel", &folderModel);
    view->rootContext()->setContextProperty("albumModel",
                                      context.contentProvider()->createModel());
    view->rootContext()->setContextProperty("tracksModel",
                                      context.contentProvider()->createModel());
    view->rootContext()->setContextProperty("player", &player);
    view->rootContext()->setContextProperty("playQueue", playQueue);

    view->setSource(QUrl("qrc:/qml/main.qml"));
    view->setResizeMode(QQuickView::SizeRootObjectToView);

#if defined(QML_MEDIA_PLAYER_DESKTOP_BUILD)
    QScreen *desktop = QGuiApplication::primaryScreen();
    if (desktop->physicalSize().height() < 1000) {
        win.scale(.8, .8);
        win.resize(win.size() * .8);
    }
    view->show();
#else
    view->showFullScreen();
#endif

    return application->exec();
}

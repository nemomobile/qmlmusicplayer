
QT += core gui qml quick sql dbus multimedia

TARGET = qmlmediaplayer
TEMPLATE = app

# Uncomment the next line to build for Desktop (development)
#DEFINES += QML_MEDIA_PLAYER_DESKTOP_BUILD

SOURCES += $$files(src/*.cpp)
HEADERS += $$files(src/*.h)
SOURCES += $$files(mediabox-core/*.cpp)
HEADERS += $$files(mediabox-core/*.h)

RESOURCES += qml/qml.qrc

target.path = /opt/qmlmediaplayer/bin

desktop.path = /usr/share/applications
desktop.files = data/qmlmediaplayer.desktop

icon.path = /usr/share/pixmaps
icon.files = data/qmlmediaplayer.png

INSTALLS += target desktop icon
CONFIG += link_pkgconfig
packagesExist(qdeclarative5-boostable) {
    message("Building with qdeclarative5-boostable support")
    DEFINES += HAS_BOOSTER
    PKGCONFIG += qdeclarative5-boostable
} else {
    warning("qdeclarative5-boostable not available; startup times will be slower")
}


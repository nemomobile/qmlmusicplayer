#-------------------------------------------------
#
# Project created by QtCreator 2011-03-05T11:04:18
#
#-------------------------------------------------

QT       += core gui declarative opengl sql phonon dbus

TARGET = musicshelf
CONFIG += console
CONFIG -= app_bundle
CONFIG += for_handset

TEMPLATE = app


SOURCES += \
    src/main.cpp \
    src/context.cpp \
    src/foldermodel.cpp \
    src/phononbackend.cpp \
    src/window.cpp \
    mediabox-core/audioinspector.cpp \
    mediabox-core/albumsource.cpp \
    mediabox-core/messagehub.cpp \
    mediabox-core/messagebus.cpp \
    mediabox-core/mimetype.cpp \
    mediabox-core/localsource.cpp \
    mediabox-core/fileundertaker.cpp \
    mediabox-core/fileindex.cpp \
    mediabox-core/file.cpp \
    mediabox-core/datadirectory.cpp \
    mediabox-core/contentprovider.cpp \
    mediabox-core/contentmodel.cpp \
    mediabox-core/abstractsource.cpp \
    mediabox-core/abstractscanner.cpp \
    mediabox-core/abstractinspector.cpp \
    mediabox-core/playqueue.cpp \
    mediabox-core/player.cpp \
    mediabox-core/directoryscanner.cpp \
    mediabox-core/vorbis.cpp \
    mediabox-core/tags.cpp \
    mediabox-core/id3v2.cpp \
    mediabox-core/id3v1.cpp

RESOURCES += \
    res.qrc \
    qml/qml.qrc

HEADERS += \
    src/context.h \
    src/foldermodel.h \
    src/phononbackend.h \
    src/window.h \
    mediabox-core/audioinspector.h \
    mediabox-core/albumsource.h \
    mediabox-core/plugin.h \
    mediabox-core/messagehub.h \
    mediabox-core/messagebus.h \
    mediabox-core/loader.h \
    mediabox-core/mimetype.h \
    mediabox-core/metadata.h \
    mediabox-core/localsource.h \
    mediabox-core/fileundertaker.h \
    mediabox-core/fileindex.h \
    mediabox-core/file.h \
    mediabox-core/datadirectory.h \
    mediabox-core/contentprovider.h \
    mediabox-core/contentmodel.h \
    mediabox-core/abstractsource.h \
    mediabox-core/abstractscanner.h \
    mediabox-core/abstractinspector.h \
    mediabox-core/playqueue.h \
    mediabox-core/player.h \
    mediabox-core/errors.h \
    mediabox-core/abstractbackend.h \
    mediabox-core/directoryscanner.h \
    mediabox-core/vorbis.h \
    mediabox-core/tags.h \
    mediabox-core/id3v2.h \
    mediabox-core/id3v1.h

OTHER_FILES += \
    qml/volume.png \
    qml/SettingsMenu.qml \
    qml/SettingsIndexing.qml \
    qml/SettingsAbout.qml \
    qml/settings_2.png \
    qml/settings_1.png \
    qml/Scrollbar.qml \
    qml/ScreenRow.qml \
    qml/ProgressBar.qml \
    qml/previous_2.png \
    qml/previous_1.png \
    qml/PlayerControls.qml \
    qml/play_2.png \
    qml/play_1.png \
    qml/pause_2.png \
    qml/pause_1.png \
    qml/panel.png \
    qml/NowPlaying.qml \
    qml/nocover.png \
    qml/next_2.png \
    qml/next_1.png \
    qml/license.js \
    qml/ImageButton.qml \
    qml/CoverFlow.qml \
    qml/Cover.qml \
    qml/ComboBox.qml \
    qml/Button.qml \
    qml/back.png \
    qml/AlbumView.qml \
    null.oga \
    qml-wetab/window-close.png \
    qml-wetab/main.qml \
    qml-wetab/Frame.qml \
    qml-fremantle/main.qml \
    qml-fremantle/Frame.qml \
    qml-handset/main.qml \
    qml-handset/Frame.qml \
    xdg/48x48/org-pycage-musicshelf.png \
    qml-handset/config.js \
    qml-wetab/config.js \
    xdg/64x64/org-pycage-musicshelf.png \
    org.pycage.musicshelf.changes \
    qml-harmattan/main.qml \
    qml-harmattan/config.js \
    qml-harmattan/MainPage.qml \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog \
    xdg/org.pycage.musicshelf.desktop \
    xdg/org.pycage.musicshelf-harmattan.desktop


for_wetab {
    DEFINES += FOR_WETAB
    RESOURCES += qml-wetab/qml-wetab.qrc

    target.path = /opt/org.pycage.musicshelf
    INSTALLS += target

    phononworkaround.path = /opt/org.pycage.musicshelf
    phononworkaround.files = null.oga
    INSTALLS += phononworkaround

    xdg-applications.path = /usr/share/applications
    xdg-applications.files = xdg/org.pycage.musicshelf.desktop
    INSTALLS += xdg-applications

    xdg-icon.path = /usr/share/pixmaps
    xdg-icon.files = xdg/64x64/org-pycage-musicshelf.png
    INSTALLS += xdg-icon
}

# the frame rate on Fremantle is really bad and OpenGL does not work properly.
# the N900's hardware woule be capable though, as it runs fine on MeeGo CE
for_fremantle {
    DEFINES += FOR_FREMANTLE
    RESOURCES += qml-fremantle/qml-fremantle.qrc

    xdg-applications.path = /usr/share/applications
    xdg-applications.files = xdg/org.pycage.musicshelf.desktop
    INSTALLS += xdg-applications

    xdg-icon.path = /usr/share/pixmaps
    xdg-icon.files = xdg/64x64/org-pycage-musicshelf.png
    INSTALLS += xdg-icon
}

for_handset {
    DEFINES += FOR_HANDSET
    RESOURCES += qml-handset/qml-handset.qrc

    target.path = /opt/org.pycage.musicshelf
    INSTALLS += target

    xdg-applications.path = /usr/share/applications
    xdg-applications.files = xdg/org.pycage.musicshelf.desktop
    INSTALLS += xdg-applications

    xdg-icon.path = /usr/share/pixmaps
    xdg-icon.files = xdg/48x48/org-pycage-musicshelf.png
    INSTALLS += xdg-icon
}

for_harmattan {
    DEFINES += FOR_HARMATTAN
    RESOURCES += qml-harmattan/qml-harmattan.qrc

    # enable booster
    CONFIG += qdeclarative-boostable
    QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
    QMAKE_LFLAGS += -pie -rdynamic

    # Please do not modify the following two lines. Required for deployment.
    include(deployment.pri)
    qtcAddDeployment()
}

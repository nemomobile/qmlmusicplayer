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


import QtQuick 2.0
import "config.js" as Config

Item {
    signal back()

    clip: true

    Rectangle {
        anchors.fill: parent
        color: '#dd000000'
        opacity: btnBack.visible
        Behavior on opacity { PropertyAnimation { duration: 500 } }
    }

    Image {
        id: btnBack
        source: "back.png"
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        visible: false

        MouseArea {
            anchors.fill: parent
            onClicked: {
                back();
            }
        }
    }

    ListView {
        id: trackList
        opacity: 0.0
        visible: opacity > 0.1
        width: parent.width / 2
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: btnBack.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        clip: true

        model: tracksModel

        Scrollbar {
            scrollTarget: parent
        }

        delegate: Column {
            width: trackList.width - 16
            x: 16
            height: 64

            Text {
                text: name
                font.pixelSize: Config.FONT_SIZE_NORMAL
                color: (resource == player.uri) ? "orange" : "white"
            }

            Text {
                text: info
                font.pixelSize: Config.FONT_SIZE_SMALL
                color: "grey"
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    console.log("clicked track" + tracksModel.count);
                    var amount = index;
                    playQueue.clear();
                    for (var i = 0; i < tracksModel.count; i++) {
                        var path = tracksModel.getPath(i);
                        console.log("index" + i);
                        playQueue.appendPath("albums://" + path);
                    }
                    playQueue.skip(amount);
                }
            }
        }

        Behavior on opacity {
            NumberAnimation { duration: 300 }
        }
    }

    states: [
        State {
            name: "active"
            PropertyChanges {
                target: btnBack
                visible: true
            }
            PropertyChanges {
                target: trackList
                opacity: 1.0
            }
        }
    ]
}

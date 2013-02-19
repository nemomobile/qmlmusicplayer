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


import Qt 4.7
import "config.js" as Config

Rectangle {
    color: "#e0000000"
    border.color: "#a99d97"
    border.width: 3
    radius: 5

    ListModel {
        id: menuModel

        ListElement {
            label: "Music Index"
        }

        ListElement {
            label: "About"
        }
    }

    ListView {
        id: menuList
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 12
        width: parent.width / 4
        spacing: 32
        clip: true

        model: menuModel

        delegate: Text {
            font.pixelSize: Config.FONT_SIZE_NORMAL
            color: (index == menuList.currentIndex) ? "orange" : "white"
            text: label

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    menuList.currentIndex = index;
                }
            }
        }

        Scrollbar {
            scrollTarget: parent
        }
    }

    ScreenRow {
        anchors.top: parent.top
        anchors.left: menuList.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 12
        clip: true

        selection: menuList.currentIndex

        screen0: SettingsIndexing { }
        screen1: SettingsAbout { }
    }
}

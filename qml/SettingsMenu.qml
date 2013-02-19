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
    color: "black"

    Row {
        id: menuList
        property int currentIndex: 0

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 30
        spacing: 32

        Repeater {
            model: ListModel {
                ListElement { label: "Music Index" }
                ListElement { label: "About" }
            }

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
        }
    }

    ScreenRow {
        anchors.top: menuList.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: 20
        clip: true

        selection: menuList.currentIndex

        screen0: SettingsIndexing { }
        screen1: SettingsAbout { }
    }
}

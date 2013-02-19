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
import "license.js" as License
import "config.js" as Config

Item {
    anchors.fill: parent

    Column {
        id: infoBox
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 24

        Text {
            color: "orange"
            width: parent.width
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: Config.FONT_SIZE_NORMAL
            text: "Music Shelf version 1.0.1"
        }

        Text {
            color: "white"
            width: parent.width
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: Config.FONT_SIZE_SMALL
            text: "a MeeGo music player, and winner of the Qt WeTab App Challenge\n" +
                  "\n" +
                  "Copyright © 2011 Martin Grimme"
        }
    }

    Item {
        id: licenseBox
        anchors.top: infoBox.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 24
        height: parent.height / 2
        clip: true

        Flickable {
            id: licenseFlickable
            anchors.fill: parent
            contentHeight: license.height

            Text {
                id: license
                color: "#a99d97"
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                wrapMode: Text.WordWrap
                textFormat: Text.PlainText
                text: License.license
                font.family: "Monospace"
                font.pixelSize: 18
            }
        }
    }
}

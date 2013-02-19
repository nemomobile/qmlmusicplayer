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

Rectangle {
    property alias text: label.text
    property int fontSize: 32

    signal clicked()

    id: button
    radius: 5
    color: buttonMa.pressed ? "white" : "#a99d97"
    border.color: buttonMa.pressed ? "white" : "#a99d97"
    border.width: 3

    Text {
        id: label
        anchors.centerIn: parent
        color: "black"
        font.pixelSize: button.fontSize
    }

    MouseArea {
        id: buttonMa
        anchors.fill: parent
        onClicked: {
            parent.clicked();
        }
    }
}
